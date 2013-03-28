//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Exclusions.h"
#include "Rules.h"
#include "../structures/SourceFiles.h"
#include "cpptcl-1.1.4/cpptcl.h"
#include <fstream>
#include <sstream>

using namespace std;
using namespace Vera;
using namespace Plugins;
using namespace Structures;
using namespace Tcl;


namespace // unnamed
{

typedef set<SourceFiles::FileName> FileNameSet;
typedef map<Rules::RuleName, FileNameSet> ExclusionMap;
ExclusionMap exclusions;

} // unnamed namespace


void Exclusions::setExclusions(const ExclusionFileName & fileName)
{
    ifstream exclusionsFile(fileName.c_str());
    if (exclusionsFile.is_open() == false)
    {
        ostringstream ss;
        ss << "cannot open exclusions file " << fileName;
        throw ExclusionError(ss.str());
    }

    interpreter interp;

    interp.eval(exclusionsFile);

    const object ruleNames = interp.eval("array names ruleExclusions");
    const size_t ruleNamesLength = ruleNames.length(interp);
    for (size_t i = 0; i != ruleNamesLength; ++i)
    {
        const string ruleName = ruleNames.at(interp, i).get();

        const object exceptionList = interp.eval("set ruleExclusions(" + ruleName + ")");
        const size_t exceptionListLength = exceptionList.length(interp);

        FileNameSet files;
        for (size_t j = 0; j != exceptionListLength; ++j)
        {
            const SourceFiles::FileName file = exceptionList.at(interp, j).get();
            files.insert(file);
        }

        exclusions[ruleName] = files;
    }
}

bool Exclusions::isExcluded(const SourceFiles::FileName & name)
{
    const Rules::RuleName currentRule = Rules::getCurrentRule();

    const ExclusionMap::const_iterator eit = exclusions.find(currentRule);
    if (eit != exclusions.end())
    {
        const FileNameSet & files = eit->second;

        const FileNameSet::const_iterator fit =
            files.find(name.substr(name.find_last_of("\\/") + 1));
        return fit != files.end();
    }
    else
    {
        return false;
    }
}
