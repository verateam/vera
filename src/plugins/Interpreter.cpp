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
#include <boost/algorithm/string/predicate.hpp>

#ifdef VERA_PYTHON
#include <boost/python.hpp>
#include <vector>
#include <sstream>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#endif

#ifdef VERA_LUA
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/iterator_policy.hpp>
extern "C"
{
#include "lualib.h"
}
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
    std::string scriptDir = root + "/scripts/";
    std::string scriptDir2 = root + "/";
    switch (type)
    {
    case rule:
        scriptDir += "rules/";
        scriptDir2 += "rules/";
        break;
    case transformation:
        scriptDir += "transformations/";
        scriptDir2 += "transformations/";
        break;
    }

    // first look at tcl rules
    std::string tclName = name;
    if (boost::algorithm::ends_with(tclName, ".tcl") == false)
    {
      tclName += ".tcl";
    }
    if (boost::filesystem::exists(scriptDir + tclName))
    {
        executeTcl(root, type, scriptDir + tclName);
        return;
    }
    else if (boost::filesystem::exists(scriptDir2 + tclName))
    {
        executeTcl(root, type, scriptDir2 + tclName);
        return;
    }
#ifdef VERA_PYTHON
    // then python
    std::string pyName = name;
    if (boost::algorithm::ends_with(pyName, ".py") == false)
    {
      pyName += ".py";
    }
    if (boost::filesystem::exists(scriptDir + pyName))
    {
        executePython(root, type, scriptDir + pyName);
        return;
    }
    else if (boost::filesystem::exists(scriptDir2 + pyName))
    {
        executePython(root, type, scriptDir2 + pyName);
        return;
    }
#endif
#ifdef VERA_LUA
    // then lua
    std::string luaName = name;
    if (boost::algorithm::ends_with(luaName, ".lua") == false)
    {
      luaName += ".lua";
    }
    if (boost::filesystem::exists(scriptDir + luaName))
    {
        executeLua(root, type, scriptDir + luaName);
        return;
    }
    else if (boost::filesystem::exists(scriptDir2 + luaName))
    {
        executeLua(root, type, scriptDir2 + luaName);
        return;
    }
#endif
    std::ostringstream ss;
    ss << "cannot open script " << name;
    throw ScriptError(ss.str());
}

void Interpreter::executeTcl(const DirectoryName & /* root */,
  ScriptType /* type */, const std::string & fileName)
{
    std::ifstream scriptFile(fileName.c_str());
    if (scriptFile.is_open() == false)
    {
        std::ostringstream ss;
        ss << "Cannot open script " << fileName;
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
#if (TCL_MAJOR_VERSION < 8) || (TCL_MAJOR_VERSION == 8) && (TCL_MINOR_VERSION < 6)
        int errorLine = inter.get()->errorLine;
#else
        int errorLine = Tcl_GetErrorLine(inter.get());
#endif
        throw Tcl::tcl_error(std::string(e.what()) +
            "\n    (file \"" + fileName + "\" line " +
            boost::lexical_cast<std::string>(errorLine) + ")");
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

// vector_indexing_suite is not doing all the job - we have to do the conversion
// from the python sequence by hand
template<typename T>
struct py_seq_to_std_vector
{
  py_seq_to_std_vector()
  {
    py::converter::registry::push_back(
      &convertible,
      &construct,
      py::type_id<std::vector<T> >());
  }
  static void* convertible(PyObject* object)
  {
    if (PySequence_Check(object))
    {
      return object;
    }
    return NULL;
  }
  static void construct(PyObject* object, py::converter::rvalue_from_python_stage1_data* data)
  {
    typedef py::converter::rvalue_from_python_storage< std::vector<T> > storage_type;
    void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;
    std::vector<T>& v = *(new (storage) std::vector<T>());
    int length = PySequence_Size(object);
    v.reserve(length);
    for (int i = 0; i < length; i++)
    {
      v.push_back(py::extract<T>(PySequence_GetItem(object, i)));
    }
    data->convertible = storage;
  }
};

BOOST_PYTHON_MODULE(vera)
{
  py_seq_to_std_vector<std::string>();

  py::class_<Structures::Token>("Token")
    .def(py::init<>())
    .def(py::init<std::string, int, int, std::string>())
    // .def("__repr__", &Token::toString)
    .add_property("value", &Structures::Token::value_)
    .add_property("line", &Structures::Token::line_)
    .add_property("column", &Structures::Token::column_)
    .add_property("name", &Structures::Token::name_)
    .add_property("type", &Structures::Token::name_);

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

#ifdef VERA_LUA

}
}

namespace luabind
{

template<typename T>
struct default_converter<std::vector<T> > : native_converter_base<std::vector<T> >
{
    static int compute_score(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TTABLE ? 0 : -1;
    }

    std::vector<T> from(lua_State* L, int index)
    {
        std::vector<T> list;

        for (luabind::iterator i(luabind::object(luabind::from_stack(L, index))), end;
            i != end; ++i)
        {
            list.push_back(luabind::object_cast<T>(*i));
        }

        return list;
    }

    void to(lua_State* L, const std::vector<T>& vector)
    {
        luabind::object list = luabind::newtable(L);

        for (std::size_t i = 0; i < vector.size(); ++i)
        {
            list[i + 1] = vector[i];
        }

        list.push(L);
    }
};

template<typename T>
struct default_converter<std::vector<T> const&>
        : default_converter<std::vector<T> >
{
};

} // namespace luabind

namespace Vera
{
namespace Plugins
{

// Structures::SourceFiles::getAllFileNames() returns a std::set that is not
// easily wrapped with luabind. It also lack the filtering of the excluded
// files
std::vector<std::string> sourceFileNames;
std::vector<std::string> const& luaGetSourceFileNames()
{
    if (sourceFileNames.empty())
    {
        const Vera::Structures::SourceFiles::FileNameSet & files =
                Vera::Structures::SourceFiles::getAllFileNames();

        typedef Vera::Structures::SourceFiles::FileNameSet::const_iterator iterator;
        const iterator end = files.end();
        for (iterator it = files.begin(); it != end; ++it)
        {
            const Vera::Structures::SourceFiles::FileName & name = *it;

            if (Vera::Plugins::Exclusions::isExcluded(name) == false)
            {
                sourceFileNames.push_back(name);
            }
        }
    }
    return sourceFileNames;
}

void Interpreter::executeLua(const DirectoryName & root,
    ScriptType type, const std::string & fileName)
{
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  luabind::open(L);

  luabind::module(L)
  [

      luabind::class_<Structures::Token>("token")
          .def_readonly("value", &Structures::Token::value_)
          .def_readonly("line", &Structures::Token::line_)
          .def_readonly("column", &Structures::Token::column_)
          .def_readonly("name", &Structures::Token::name_)
          .def_readonly("type", &Structures::Token::name_),


      luabind::def("getTokens", &Structures::Tokens::getTokens, luabind::return_stl_iterator),

      luabind::def("report", &Plugins::Reports::add),

      luabind::def("getParameter", &Plugins::Parameters::get),

      luabind::def("getSourceFileNames", &luaGetSourceFileNames, luabind::return_stl_iterator),

      luabind::def("getLineCount", &Structures::SourceLines::getLineCount),

      luabind::def("getLine", &Structures::SourceLines::getLine),

      luabind::def("getAllLines", &Structures::SourceLines::getAllLines,
          luabind::return_stl_iterator)

  ];

  if (luaL_dofile(L, fileName.c_str()))
  {
      throw std::runtime_error(lua_tostring(L, lua_gettop(L)));
  }
  lua_close(L); // TODO: fix leak!
}
#endif

}
}
