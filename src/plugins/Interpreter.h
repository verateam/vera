//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#include <stdexcept>


namespace Vera
{
namespace Plugins
{


class ScriptError : public std::runtime_error
{
public:
    ScriptError(const std::string & msg) : std::runtime_error(msg) {}
};


class Interpreter
{
public:
    enum ScriptType { rule, transformation };

    typedef std::string DirectoryName;
    typedef std::string ScriptName;

    static void execute(const DirectoryName & root,
        ScriptType type, const ScriptName & name);
};

} // namespace Plugins

} // namespace Vera

#endif // INTERPRETER_H_INCLUDED
