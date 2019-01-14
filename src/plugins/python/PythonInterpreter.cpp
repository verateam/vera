//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifdef VERA_PYTHON

#include "PythonInterpreter.h"
#include "../Exclusions.h"
#include "../Reports.h"
#include "../Parameters.h"
#include "../../structures/SourceFiles.h"
#include "../../structures/SourceLines.h"
#include "../../structures/Tokens.h"
#include <fstream>
#include <iterator>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <boost/python.hpp>
#include <vector>
#include <sstream>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace Vera
{
namespace Plugins
{

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
      py::return_value_policy<py::copy_const_reference>());

  py::def("getAllLines", &Structures::SourceLines::getAllLines,
      py::return_value_policy<py::reference_existing_object>());
};

void PythonInterpreter::execute(const std::string & fileName)
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
        PyObject* exc;
        PyObject* val;
        PyObject* tb;
        PyErr_Fetch(&exc, &val, &tb);
        py::handle<> hexc(exc);
        py::handle<> hval(py::allow_null(val));
        py::handle<> htb(py::allow_null(tb));
        py::object traceback(py::import("traceback"));
        py::list formatted_list =
          py::extract<py::list>(traceback.attr("format_exception_only")(hexc, hval));
        formatted_list.extend(traceback.attr("format_tb")(htb));
        py::str formatted = py::str("").join(formatted_list).strip();
        throw std::runtime_error(py::extract<std::string>(formatted));
    }
    Py_Finalize();
}

}
}
#endif
