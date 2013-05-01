//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Interpreter.h"
#include "Exclusions.h"
#include "Reports.h"
#include "Parameters.h"
#include "../structures/SourceFiles.h"
#include "../structures/SourceLines.h"
#include "../structures/Tokens.h"
#include "cpptcl-1.1.4/cpptcl.h"
#include <fstream>
#include <iterator>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#ifdef VERA_PYTHON
#include <boost/python.hpp>
#include <vector>
#include <sstream>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#endif


namespace // unnamed
{

// helper global pointer
// - for those functions that might modify the interpreter's state

Tcl::interpreter *pInter;

void report(const std::string & fileName, int lineNumber, const std::string & message)
{
    Vera::Plugins::Reports::add(fileName, lineNumber, message);
}

std::string getParameter(const std::string & name, const std::string & defaultValue)
{
    return Vera::Plugins::Parameters::get(name, defaultValue);
}

Tcl::object getSourceFileNames()
{
    Tcl::object obj;

    const Vera::Structures::SourceFiles::FileNameSet & files =
            Vera::Structures::SourceFiles::getAllFileNames();

    typedef Vera::Structures::SourceFiles::FileNameSet::const_iterator iterator;
    const iterator end = files.end();
    for (iterator it = files.begin(); it != end; ++it)
    {
        const Vera::Structures::SourceFiles::FileName & name = *it;

        if (Vera::Plugins::Exclusions::isExcluded(name) == false)
        {
            obj.append(*pInter, Tcl::object(name));
        }
    }

    return obj;
}

int getLineCount(const std::string & sourceName)
{
    return Vera::Structures::SourceLines::getLineCount(sourceName);
}

std::string getLine(const std::string & sourceName, int lineNumber)
{
    return Vera::Structures::SourceLines::getLine(sourceName, lineNumber);
}

Tcl::object getAllLines(const std::string & sourceName)
{
    Tcl::object obj;

    const Vera::Structures::SourceLines::LineCollection & lines =
            Vera::Structures::SourceLines::getAllLines(sourceName);

    typedef Vera::Structures::SourceLines::LineCollection::const_iterator iterator;
    const iterator end = lines.end();
    for (iterator it = lines.begin(); it != end; ++it)
    {
        obj.append(*pInter, Tcl::object(*it));
    }

    return obj;
}

Tcl::object getTokens(const std::string & sourceName, int fromLine, int fromColumn,
    int toLine, int toColumn, const Tcl::object & filter)
{
    Vera::Structures::Tokens::FilterSequence filterSeq;

    size_t filterLength = filter.length(*pInter);
    for (size_t i = 0; i != filterLength; ++i)
    {
        filterSeq.push_back(filter.at(*pInter, i).get());
    }

    Vera::Structures::Tokens::TokenSequence tokenSeq =
        Vera::Structures::Tokens::getTokens(sourceName,
        fromLine, fromColumn, toLine, toColumn, filterSeq);

    Tcl::object ret;
    Vera::Structures::Tokens::TokenSequence::iterator it = tokenSeq.begin();
    Vera::Structures::Tokens::TokenSequence::iterator end = tokenSeq.end();
    for ( ; it != end; ++it)
    {
        Tcl::object singleToken;
        singleToken.append(*pInter, Tcl::object(it->value_));
        singleToken.append(*pInter, Tcl::object(it->line_));
        singleToken.append(*pInter, Tcl::object(it->column_));
        singleToken.append(*pInter, Tcl::object(it->name_));

        ret.append(*pInter, singleToken);
    }

    return ret;
}

void registerCommands(Tcl::interpreter & inter)
{
    pInter = &inter;

    // commands related to source files and plain source code
    inter.def("report", report);
    inter.def("getParameter", getParameter);
    inter.def("getSourceFileNames", getSourceFileNames);
    inter.def("getLineCount", getLineCount);
    inter.def("getLine", getLine);
    inter.def("getAllLines", getAllLines);
    inter.def("getTokens", getTokens);
}

} // unnamed namespace

namespace Vera
{
namespace Plugins
{

void Interpreter::execute(const DirectoryName & root,
    ScriptType type, const ScriptName & name)
{
    std::string fileName = root + "/scripts/";
    switch (type)
    {
    case rule:
        fileName += "rules/";
        break;
    case transformation:
        fileName += "transformations/";
        break;
    }

    // first look at tcl rules
    std::string tclFileName = fileName + name + ".tcl";
    if (boost::filesystem::exists(tclFileName))
    {
        executeTcl(root, type, tclFileName);
        return;
    }
#ifdef VERA_PYTHON
    // then python
    std::string pyFileName = fileName + name + ".py";
    if (boost::filesystem::exists(pyFileName))
    {
        executePython(root, type, pyFileName);
        return;
    }
#endif
    std::ostringstream ss;
    ss << "cannot open script " << name;
    throw ScriptError(ss.str());
}

void Interpreter::executeTcl(const DirectoryName & root,
    ScriptType type, const std::string & fileName)
{
    std::ifstream scriptFile(fileName.c_str());
    if (scriptFile.is_open() == false)
    {
        std::ostringstream ss;
        ss << "cannot open script " << fileName;
        throw ScriptError(ss.str());
    }

    std::string scriptBody;
    scriptBody.assign(std::istreambuf_iterator<char>(scriptFile), std::istreambuf_iterator<char>());

    Tcl::interpreter inter;
    registerCommands(inter);
    try
    {
        inter.eval(scriptBody);
    }
    catch (Tcl::tcl_error & e)
    {
        // rethrow the exception with the name of the rule
        throw Tcl::tcl_error(std::string(e.what()) +
            "\n    (file \"" + fileName + "\" line " +
            boost::lexical_cast<std::string>(inter.get()->errorLine) + ")");
    }
}

#ifdef VERA_PYTHON

namespace py = boost::python;

// Structures::SourceFiles::getAllFileNames() returns a std::set that is not
// easily wrapped with boost python. It also lack the filtering of the excluded
// files
std::vector<std::string> pyGetSourceFileNames()
{
    std::vector<std::string> res;

    const Vera::Structures::SourceFiles::FileNameSet & files =
            Vera::Structures::SourceFiles::getAllFileNames();

    typedef Vera::Structures::SourceFiles::FileNameSet::const_iterator iterator;
    const iterator end = files.end();
    for (iterator it = files.begin(); it != end; ++it)
    {
        const Vera::Structures::SourceFiles::FileName & name = *it;

        if (Vera::Plugins::Exclusions::isExcluded(name) == false)
        {
            res.push_back(name);
        }
    }

    return res;
}

BOOST_PYTHON_MODULE(vera)
{
  py::class_<Structures::Token>("Token")
    .def(py::init<>())
    .def(py::init<std::string, int, int, std::string>())
    // .def("__repr__", &Token::toString)
    .add_property("value", &Structures::Token::value_)
    .add_property("line", &Structures::Token::line_)
    .add_property("column", &Structures::Token::column_)
    .add_property("name", &Structures::Token::name_);

  py::class_<Structures::Tokens::TokenSequence>("TokenVector")
        .def(py::vector_indexing_suite<Structures::Tokens::TokenSequence>());

  py::class_<Structures::SourceLines::LineCollection>("StringVector")
          .def(py::vector_indexing_suite<Structures::SourceLines::LineCollection>());

  py::def("getTokens", &Structures::Tokens::getTokens);

  py::def("report", &Plugins::Reports::add);

  py::def("getParameter", &Plugins::Parameters::get);

  py::def("getSourceFileNames", &pyGetSourceFileNames);

  py::def("getLineCount", &Structures::SourceLines::getLineCount);

  py::def("getLine", &Structures::SourceLines::getLine,
      py::return_value_policy<py::reference_existing_object>());

  py::def("getAllLines", &Structures::SourceLines::getAllLines,
      py::return_value_policy<py::reference_existing_object>());
};

void Interpreter::executePython(const DirectoryName & root,
    ScriptType type, const std::string & fileName)
{
    try
    {
        PyImport_AppendInittab("vera", initvera);
        Py_Initialize();

        py::object main_module = py::import("__main__");
        py::object main_namespace = main_module.attr("__dict__");
        main_namespace["vera"] = py::import("vera");

        py::exec_file(fileName.c_str(), main_namespace, main_namespace);
    }
    catch (py::error_already_set const&)
    {
        PyErr_Print();
    }
    Py_Finalize();
}
#endif

}
}
