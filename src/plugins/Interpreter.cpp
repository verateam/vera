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
#include <fstream>
#include <iterator>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "tcl/TclInterpreter.h"
#ifdef VERA_PYTHON
#include "python/PythonInterpreter.h"
#endif
#ifdef VERA_LUA
#include "lua/LuaInterpreter.h"
#endif

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
        TclInterpreter::execute(scriptDir + tclName);
        return;
    }
    else if (boost::filesystem::exists(scriptDir2 + tclName))
    {
        TclInterpreter::execute(scriptDir2 + tclName);
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
        PythonInterpreter::execute(scriptDir + pyName);
        return;
    }
    else if (boost::filesystem::exists(scriptDir2 + pyName))
    {
        PythonInterpreter::execute(scriptDir2 + pyName);
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
        LuaInterpreter::execute(scriptDir + luaName);
        return;
    }
    else if (boost::filesystem::exists(scriptDir2 + luaName))
    {
        LuaInterpreter::execute(scriptDir2 + luaName);
        return;
    }
#endif
    std::ostringstream ss;
    ss << "cannot open script " << name;
    throw ScriptError(ss.str());
}

}
}
