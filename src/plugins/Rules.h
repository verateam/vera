//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RULES_H_INCLUDED
#define RULES_H_INCLUDED

#include <string>


namespace Vera
{
namespace Plugins
{

class Rules
{
public:
    typedef std::string RuleName;

    static void executeRule(const RuleName & name);

    static RuleName getCurrentRule();
};

} // namespace Plugins

} // namespace Vera

#endif // RULES_H_INCLUDED
