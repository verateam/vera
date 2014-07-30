//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Exclusions.h"
#include "Rules.h"
#include "../structures/SourceFiles.h"
#include "tcl/cpptcl-1.1.4/cpptcl.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <cerrno>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH


namespace // unnamed
{

typedef std::set<Vera::Structures::SourceFiles::FileName> FileNameSet;
typedef std::map<Vera::Plugins::Rules::RuleName, FileNameSet> ExclusionMap;
ExclusionMap exclusions;

typedef std::vector<boost::regex> RegexList;
RegexList exclusionRegexs;

} // unnamed namespace

namespace Vera
{
namespace Plugins
{

void Exclusions::setExclusions(const ExclusionFileName & fileName)
{
    try
    {
        setExclusionsTcl(fileName);
    }
    catch (...)
    {
        setExclusionsRegex(fileName);
    }
}

void Exclusions::setExclusionsRegex(const ExclusionFileName & fileName)
{
    std::ifstream exclusionsFile(fileName.c_str());
    if (exclusionsFile.is_open() == false)
    {
        std::ostringstream ss;
        ss << "Cannot open exclusions file " << fileName << ": "
           << strerror(errno);
        throw ExclusionError(ss.str());
    }
    std::string re;
    while (std::getline(exclusionsFile, re))
    {
        // don't process empty lines and lines begining with a #
        if (re.empty() == false && re.substr(0, 1) != "#")
        {
          exclusionRegexs.push_back(boost::regex(re));
        }
    }
    if (exclusionsFile.bad())
    {
        throw std::runtime_error(
            "Cannot read from " + fileName + ": " + strerror(errno));
    }
}

void Exclusions::setExclusionsTcl(const ExclusionFileName & fileName)
{
    std::ifstream exclusionsFile(fileName.c_str());
    if (exclusionsFile.is_open() == false)
    {
        std::ostringstream ss;
        ss << "Cannot open exclusions file " << fileName << ": "
           << strerror(errno);
        throw ExclusionError(ss.str());
    }

    Tcl::interpreter interp;
    interp.eval(exclusionsFile);
    if (exclusionsFile.bad())
    {
        throw std::runtime_error(
            "Cannot read from " + fileName + ": " + strerror(errno));
    }

    const Tcl::object ruleNames = interp.eval("array names ruleExclusions");
    const size_t ruleNamesLength = ruleNames.length(interp);
    for (size_t i = 0; i != ruleNamesLength; ++i)
    {
        const std::string ruleName = ruleNames.at(interp, i).get();

        const Tcl::object exceptionList = interp.eval("set ruleExclusions(" + ruleName + ")");
        const size_t exceptionListLength = exceptionList.length(interp);

        FileNameSet files;
        for (size_t j = 0; j != exceptionListLength; ++j)
        {
            const Structures::SourceFiles::FileName file = exceptionList.at(interp, j).get();
            files.insert(file);
        }

        exclusions[ruleName] = files;
    }
}

bool Exclusions::isExcluded(const Structures::SourceFiles::FileName & name,
  int lineNumber, const Rules::RuleName & currentRule, const std::string & msg)
{
    std::string fmsg = name + ":" + boost::lexical_cast<std::string>(lineNumber)
        + ": " + currentRule + ": " + msg;
    foreach(boost::regex const& re, exclusionRegexs)
    {
        if (boost::regex_search(fmsg, re))
        {
            return true;
        }
    }
    return false;
}

bool Exclusions::isExcluded(const Structures::SourceFiles::FileName & name)
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

}

}
