//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Rules.h"
#include "RootDirectory.h"
#include "Interpreter.h"


namespace // unnamed
{

Vera::Plugins::Rules::RuleName currentRule_;

} // unnamed namespace

namespace Vera
{
namespace Plugins
{

void Rules::executeRule(const RuleName & name)
{
    currentRule_ = name;

    const Vera::Plugins::RootDirectory::DirectoryName veraRoot =
            Vera::Plugins::RootDirectory::getRootDirectory();
    Interpreter::execute(veraRoot, Interpreter::rule, name);
}

Rules::RuleName Rules::getCurrentRule()
{
    return currentRule_;
}

}
}
