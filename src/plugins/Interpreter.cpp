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
#include "../plugins/Messages.h"
#include "../structures/SourceFiles.h"
#include "../structures/SourceLines.h"
#include "../structures/Tokens.h"
#include "../structures/Statements.h"
#include "../structures/Document.h"
#include "cpptcl-1.1.4/cpptcl.h"
#include <fstream>
#include <iterator>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>

#ifdef VERA_PYTHON
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
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

Interpreter::Interpreter()
{
#ifdef VERA_PYTHON
  Py_Initialize();
#endif
}

#ifdef VERA_PYTHON
boost::shared_ptr<struct Startup> modules;
#endif

Interpreter::~Interpreter()
{
#ifdef VERA_PYTHON
  modules.reset(static_cast<struct Startup*>(NULL));
  Py_Finalize();
#endif
}

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
    std::string tclFileName = fileName + name;
    if (boost::algorithm::ends_with(tclFileName, ".tcl") == false)
    {
      tclFileName += ".tcl";
    }
    if (boost::filesystem::exists(tclFileName))
    {
        executeTcl(root, type, tclFileName);
        return;
    }
#ifdef VERA_PYTHON
    // then python
    std::string pyFileName = fileName + name;
    if (boost::algorithm::ends_with(pyFileName, ".py") == false)
    {
      pyFileName += ".py";
    }
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

static bool initialize_ = false;

#define REQUEST_REGISTER(T) \
{\
   boost::python::type_info info = boost::python::type_id<T >(); \
   const boost::python::converter::registration* reg = boost::python::converter::registry::query(info); \
}

#define REGISTER_CONVERTER(T,policy) \
{\
   boost::python::type_info info = boost::python::type_id<T >(); \
   const boost::python::converter::registration* reg = boost::python::converter::registry::query(info); \
   if (reg == NULL)  \
   {\
    to_tuple_mapping<T >();\
    from_python_sequence<T,policy>();\
   }\
   else if ((*reg).m_to_python == NULL)\
   {\
    to_tuple_mapping<T >();\
    from_python_sequence<T,policy>();\
   }\
}

BOOST_PYTHON_MODULE(vera)
{
  if (initialize_)
     return;

  py_seq_to_std_vector<std::string>();


  py::class_<Structures::Token>("Token")
    .def(py::init<>())
    .def(py::init<std::string, int, int, std::string>())
    .add_property("value", &Structures::Token::value_)
    .add_property("line", &Structures::Token::line_)
    .add_property("column", &Structures::Token::column_)
    .add_property("type", &Structures::Token::name_);

  py::class_<Structures::Statement>("Statement", py::no_init)
    .def(py::init<const Structures::Token&>())
    .def("getParent", &Structures::Statement::getParent,
       py::return_value_policy<py::reference_existing_object>())
    .add_property("id", &Structures::Statement::id_)
    .add_property("statements", &Structures::Statement::statementSequence_)
    .add_property("type", &Structures::Statement::type_)
    .add_property("parentId", &Structures::Statement::parentId_)
    .def("getParent", &Structures::Statement::getParent,
        py::return_internal_reference<>())
    .def("getToken", &Structures::Statement::getToken,
        py::return_value_policy<py::reference_existing_object>());

  py::class_<Structures::Document, boost::noncopyable>("Document", py::no_init)
    .def("__init__", py::make_constructor(Vera::Structures::Document::create))
    .def("parse", &Structures::Document::parse)
    .def("getRoot", &Structures::Document::getRoot,
        py::return_value_policy<py::reference_existing_object>());

  py::enum_<Structures::Statement::TypeItem>("TypeItem")
    .value("TYPE_ITEM_TOKEN", Structures::Statement::TYPE_ITEM_TOKEN)
    .value("TYPE_ITEM_STATEMENT", Structures::Statement::TYPE_ITEM_STATEMENT)
    .value("TYPE_ITEM_ROOT", Structures::Statement::TYPE_ITEM_ROOT)
    .value("TYPE_ITEM_STATEMENT_OF_IF",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_IF)
    .value("TYPE_ITEM_STATEMENT_OF_FORLOOP",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_FORLOOP)
    .value("TYPE_ITEM_STATEMENT_OF_WHILELOOP",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_WHILELOOP)
    .value("TYPE_ITEM_STATEMENT_OF_TRYCATCHES",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_TRYCATCHES)
    .value("TYPE_ITEM_STATEMENT_OF_CATCH",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_CATCH)
    .value("TYPE_ITEM_STATEMENT_OF_DOWHILELOOP",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_DOWHILELOOP)
    .value("TYPE_ITEM_STATEMENT_OF_WHILE",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_WHILE)
    .value("TYPE_ITEM_STATEMENT_OF_ELSE",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_ELSE)
    .value("TYPE_ITEM_STATEMENT_OF_SWITCH",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_SWITCH)
    .value("TYPE_ITEM_STATEMENT_OF_NAMESPACE",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_NAMESPACE)
    .value("TYPE_ITEM_STATEMENT_OF_STRUCT",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_STRUCT)
    .value("TYPE_ITEM_STATEMENT_OF_ACCESSMODIFIERS",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_ACCESSMODIFIERS)
    .value("TYPE_ITEM_STATEMENT_OF_DEFAULT",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_DEFAULT)
    .value("TYPE_ITEM_STATEMENT_OF_CASE",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_CASE)
    .value("TYPE_ITEM_STATEMENT_OF_HERITAGE",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_HERITAGE)
    .value("TYPE_ITEM_STATEMENT_OF_DECLARATION_LIST",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_DECLARATION_LIST)
    .value("TYPE_ITEM_STATEMENT_OF_INITIALIZER_LIST",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_INITIALIZER_LIST)
    .value("TYPE_ITEM_STATEMENT_OF_OPERATORTERNARIO",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_OPERATORTERNARIO)
    .value("TYPE_ITEM_STATEMENT_OF_EXTERN",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_EXTERN)
    .value("TYPE_ITEM_STATEMENT_OF_TEMPLATEPARAMETERS",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_TEMPLATEPARAMETERS)
    .value("TYPE_ITEM_STATEMENT_OF_ENUM",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_ENUM)
    .value("TYPE_ITEM_STATEMENT_OF_PARENSARGUMENTS",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_PARENSARGUMENTS)
    .value("TYPE_ITEM_STATEMENT_OF_BRACKETSARGUMENTS",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_BRACKETSARGUMENTS)
    .value("TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS)
    .value("TYPE_ITEM_STATEMENT_OF_PREPROCESSORLINE",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_PREPROCESSORLINE)
    .value("TYPE_ITEM_STATEMENT_OF_UNION",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_UNION)
    .value("TYPE_ITEM_STATEMENT_OF_DEFINE",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_DEFINE)
    .value("TYPE_ITEM_STATEMENT_OF_CLASS",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_CLASS)
    .value("TYPE_ITEM_STATEMENT_OF_TYPEDEF",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_TYPEDEF)
    .value("TYPE_ITEM_STATEMENT_OF_TYPEDEF_SIGNATURE",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_TYPEDEF_SIGNATURE)
    .value("TYPE_ITEM_STATEMENT_OF_TYPEDEF_STRUCT",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_TYPEDEF_STRUCT)
    .value("TYPE_ITEM_STATEMENT_OF_TYPEDEF_UNION",
       Structures::Statement::TYPE_ITEM_STATEMENT_OF_TYPEDEF_UNION)
    .value("TYPE_ITEM_STATEMENT_OF_TYPEDEF_ENUM",
          Structures::Statement::TYPE_ITEM_STATEMENT_OF_TYPEDEF_ENUM)
    .value("TYPE_ITEM_STATEMENT_OF_INCLUDE",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_INCLUDE)
    .value("TYPE_ITEM_STATEMENT_OF_FUNCTION",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_FUNCTION)
    .value("TYPE_ITEM_STATEMENT_OF_OPERATOR",
       Structures::Statement::TYPE_ITEM_STATEMENT_OF_OPERATOR)
    .value("TYPE_ITEM_STATEMENT_OF_TEMPLATE",
        Structures::Statement::TYPE_ITEM_STATEMENT_OF_TEMPLATE);

  py::register_ptr_to_python<boost::shared_ptr<Structures::Document> >();

  py::class_<Structures::Tokens::TokenSequence>("TokenVector")
        .def(py::vector_indexing_suite<Structures::Tokens::TokenSequence>());

  py::class_<Structures::Statement::StatementSequence>("StatementVector")
        .def(py::vector_indexing_suite<Structures::Statement::StatementSequence>());

  py::class_<Structures::SourceLines::LineCollection>("StringVector")
          .def(py::vector_indexing_suite<Structures::SourceLines::LineCollection>());

  py::def("getStatement", &Structures::StatementsBuilder::create);

  py::def("getEachTokenFromFile", &Structures::Tokens::getEachTokenFromFile);

  py::def("getTokens", &Structures::Tokens::getTokens);

  py::def("report", &Plugins::Reports::add);

  py::def("getParameter", &Plugins::Parameters::get);

  py::def("getSourceFileNames", &pyGetSourceFileNames);

  py::def("getLineCount", &Structures::SourceLines::getLineCount);

  py::def("getLine", &Structures::SourceLines::getLine, 
      py::return_value_policy<py::reference_existing_object>());

  py::def("getAllLines", &Structures::SourceLines::getAllLines, 
      py::return_value_policy<py::reference_existing_object>());
  initialize_ = true;
};

struct Startup
{
    Startup()
    : main_module(py::import("__main__"))
    , main_namespace(main_module.attr("__dict__"))
    {

    }

    void exec(const std::string& fileName)
    {
      py::exec_file(fileName.c_str(), main_namespace, main_namespace);
    }

    py::object main_module ;
    py::object main_namespace;
};

void Interpreter::executePython(const DirectoryName & root, 
    ScriptType type, const std::string & fileName)
{
    try
    {
      PyImport_AppendInittab("vera", initvera);

      if (!modules)
      {
         modules = boost::make_shared<struct Startup>();
      }

      std::stringstream out;

      out << "exec: "<<fileName<<std::endl;

      Plugins::Message::get_mutable_instance().show(out.str());
      modules->exec(fileName);

    }
    catch (py::error_already_set const&)
    {
        PyErr_Print();
    }
}
#endif

}
}
