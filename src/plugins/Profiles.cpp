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

using namespace std;
using namespace Vera;
using namespace Plugins;
using namespace Tcl;


namespace // unnamed
{

typedef vector<Rules::RuleName> RuleNameCollection;

RuleNameCollection getListOfScriptNames(const Profiles::ProfileName & profile)
{
    RuleNameCollection allRules;

    // name of the profile is also the name of the profile file

    const RootDirectory::DirectoryName veraRoot = RootDirectory::getRootDirectory();

    string fileName(veraRoot + "/profiles/");
    fileName += profile;

    ifstream profileFile(fileName.c_str());
    if (profileFile.is_open() == false)
    {
        ostringstream ss;
        ss << "cannot open profile description for profile " << profile;
        throw ProfileError(ss.str());
    }

    interpreter interp;

    interp.eval(profileFile);
    const object ruleList = interp.eval("set rules");

    const size_t ruleListLength = ruleList.length(interp);
    for (size_t i = 0; i != ruleListLength; ++i)
    {
        const Rules::RuleName rName = ruleList.at(interp, i).get();
        allRules.push_back(rName);
    }

    return allRules;
}

} // unnamed namespace


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
