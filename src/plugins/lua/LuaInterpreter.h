//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef LUA_INTERPRETER_H_INCLUDED
#define LUA_INTERPRETER_H_INCLUDED

#ifdef VERA_LUA

#include <stdexcept>
#include "../Interpreter.h"

namespace Vera
{
namespace Plugins
{


class LuaInterpreter
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
