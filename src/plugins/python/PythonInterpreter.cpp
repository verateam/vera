//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifdef VERA_PYTHON

#include "PythonInterpreter.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iterator>
#include "../../structures/SourceFiles.h"
#include "../../structures/SourceLines.h"
#include "../../structures/Tokens.h"
#include "../Exclusions.h"
#include "../Parameters.h"
#include "../Reports.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <sstream>
#include <vector>

namespace Vera {
namespace Plugins {

namespace py = boost::python;

// Structures::SourceFiles::getAllFileNames() returns a std::set that is not
// easily wrapped with boost python. It also lack the filtering of the excluded
// files
std::vector<std::string> pyGetSourceFileNames() {
  std::vector<std::string> res;

  const Vera::Structures::SourceFiles::FileNameSet& files =
      Vera::Structures::SourceFiles::getAllFileNames();

  typedef Vera::Structures::SourceFiles::FileNameSet::const_iterator iterator;
  const iterator end = files.end();
  for (iterator it = files.begin(); it != end; ++it) {
    const Vera::Structures::SourceFiles::FileName& name = *it;

    if (Vera::Plugins::Exclusions::isExcluded(name) == false) {
      res.push_back(name);
    }
  }

  return res;
}

// vector_indexing_suite is not doing all the job - we have to do the conversion
// from the python sequence by hand
template <typename T>
struct py_seq_to_std_vector {
  py_seq_to_std_vector() {
    py::converter::registry::push_back(&convertible, &construct,
                                       py::type_id<std::vector<T> >());
  }
  static void* convertible(PyObject* object) {
    if (PySequence_Check(object)) {
      return object;
    }
    return NULL;
  }
  static void construct(PyObject* object,
                        py::converter::rvalue_from_python_stage1_data* data) {
    typedef py::converter::rvalue_from_python_storage<std::vector<T> >
        storage_type;
    void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;
    std::vector<T>& v = *(new (storage) std::vector<T>());
    int length = PySequence_Size(object);
    v.reserve(length);
    for (int i = 0; i < length; i++) {
      v.push_back(py::extract<T>(PySequence_GetItem(object, i)));
    }
    data->convertible = storage;
  }
};

BOOST_PYTHON_MODULE(vera) {
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
      .def(
          py::vector_indexing_suite<Structures::SourceLines::LineCollection>());

  py::def("getTokens", &Structures::Tokens::getTokens);

  py::def("report", &Plugins::Reports::add);

  py::def("getParameter", &Plugins::Parameters::get);

  py::def("getSourceFileNames", &pyGetSourceFileNames);

  py::def("getLineCount", &Structures::SourceLines::getLineCount);

  py::def("isBinary", &Structures::SourceLines::isBinary);

  py::def("getLine", &Structures::SourceLines::getLine,
          py::return_value_policy<py::copy_const_reference>());

  py::def("getAllLines", &Structures::SourceLines::getAllLines,
          py::return_value_policy<py::reference_existing_object>());
};

void PythonInterpreter::execute(const std::string& fileName) {
  try {
    // Note: Boost Python (as of ver 1.69) does not support Py_Finalize().
    // Thus, a previous call to this method may have already
    // initialized python.
    // See Boost Python documentation for more information:
    // https://www.boost.org/doc/libs/1_69_0/libs/python/doc/html/
    // tutorial/tutorial/embedding.html
    if (not Py_IsInitialized()) {
      PyImport_AppendInittab("vera", &PyInit_vera);
      Py_Initialize();
    }

    PyRun_SimpleString("import sys");
    boost::format fmt = boost::format("sys.path.append(\"%1%\")") %  boost::filesystem::path(fileName).parent_path().string();
    PyRun_SimpleString(fmt.str().c_str());

    py::object main_module = py::import("__main__");
    py::object main_namespace = main_module.attr("__dict__");
    main_namespace["vera"] = py::import("vera");

    py::exec_file(fileName.c_str(), main_namespace, main_namespace);
  } catch (py::error_already_set const&) {
    // exception handling based on
    // https://thejosephturner.com/blog/post/embedding-python-in-c-applications-with-boostpython-part-2/
    PyObject* type_ptr = NULL;
    PyObject* value_ptr = NULL;
    PyObject* traceback_ptr = NULL;

    PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);
    std::string ret("Unfetchable Python error");
    if (type_ptr != NULL) {
      py::handle<> h_type(type_ptr);
      py::str type_pstr(h_type);
      py::extract<std::string> e_type_pstr(type_pstr);
      if (e_type_pstr.check()) {
        ret = e_type_pstr();
      } else {
        ret = "Unknown exception type";
      }
    }
    if (value_ptr != NULL) {
      py::handle<> h_val(value_ptr);
      py::str a(h_val);
      py::extract<std::string> returned(a);
      if (returned.check()) {
        ret += ": " + returned();
      } else {
        ret += std::string(": Unparseable Python error: ");
      }
    }
    if (traceback_ptr != NULL) {
      py::handle<> h_tb(traceback_ptr);
      py::object tb(py::import("traceback"));
      py::object fmt_tb(tb.attr("format_tb"));
      py::object tb_list(fmt_tb(h_tb));
      py::object tb_str(py::str("\n").join(tb_list));
      py::extract<std::string> returned(tb_str);
      if (returned.check()) {
        ret += ": " + returned();
      } else {
        ret += std::string(": Unparseable Python traceback");
      }
    }

    throw std::runtime_error(ret);

    //////////////////////
  }
}

}  // namespace Plugins
}  // namespace Vera
#endif
