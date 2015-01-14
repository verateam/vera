//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PYTHON_INTERPRETER_H_INCLUDED
#define PYTHON_INTERPRETER_H_INCLUDED

#ifdef VERA_PYTHON

#include <stdexcept>
#include "../Interpreter.h"

namespace Vera
{
namespace Plugins
{

class PythonInterpreter
{
public:
    typedef Interpreter::ScriptType ScriptType;
    typedef std::string DirectoryName;
    typedef std::string ScriptName;

    static void execute(const ScriptName & name);
};

} // namespace Plugins

} // namespace Vera

#endif

#endif // INTERPRETER_H_INCLUDED
