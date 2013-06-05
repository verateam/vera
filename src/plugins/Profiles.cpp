//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Profiles.h"
#include "Rules.h"
#include "RootDirectory.h"
#include "cpptcl-1.1.4/cpptcl.h"
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <cerrno>


namespace // unnamed
{

typedef std::vector<Vera::Plugins::Rules::RuleName> RuleNameCollection;

RuleNameCollection getListOfScriptNames(const Vera::Plugins::Profiles::ProfileName & profile)
{
    RuleNameCollection allRules;

    // name of the profile is also the name of the profile file

    const Vera::Plugins::RootDirectory::DirectoryName veraRoot =
            Vera::Plugins::RootDirectory::getRootDirectory();

    std::string fileName(veraRoot + "/profiles/");
    fileName += profile;

    std::ifstream profileFile(fileName.c_str());
    if (profileFile.is_open() == false)
    {
        std::ostringstream ss;
        ss << "Cannot open profile description for profile '" << profile
            << "': "<< strerror(errno);
        throw Vera::Plugins::ProfileError(ss.str());
    }

    Tcl::interpreter interp;
    interp.eval(profileFile);
    if (profileFile.bad())
    {
        throw std::ios::failure(
            "Cannot read from " + fileName + ": " + strerror(errno));
    }

    const Tcl::object ruleList = interp.eval("set rules");

    const size_t ruleListLength = ruleList.length(interp);
    for (size_t i = 0; i != ruleListLength; ++i)
    {
        const Vera::Plugins::Rules::RuleName rName = ruleList.at(interp, i).get();
        allRules.push_back(rName);
    }

    return allRules;
}

} // unnamed namespace


namespace Vera
{
namespace Plugins
{

void Profiles::executeProfile(const ProfileName & profile)
{
    const RuleNameCollection scripts = getListOfScriptNames(profile);

    typedef RuleNameCollection::const_iterator iterator;
    const iterator end = scripts.end();
    for (iterator it = scripts.begin(); it != end; ++it)
    {
        Rules::executeRule(*it);
    }
}

}
}
