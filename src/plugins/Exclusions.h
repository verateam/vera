//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXCLUSIONS_H_INCLUDED
#define EXCLUSIONS_H_INCLUDED

#include <string>
#include "../structures/SourceFiles.h"
#include "Rules.h"
#include <stdexcept>


namespace Vera
{
namespace Plugins
{

class ExclusionError : public std::runtime_error
{
public:
    ExclusionError(const std::string & msg) : std::runtime_error(msg) {}
};

class Exclusions
{
public:
    typedef std::string ExclusionFileName;

    static void setExclusions(const ExclusionFileName & name);
    static void setExclusionsTcl(const ExclusionFileName & name);
    static void setExclusionsRegex(const ExclusionFileName & name);

    static bool isExcluded(const Structures::SourceFiles::FileName & name);
    static bool isExcluded(const Structures::SourceFiles::FileName & name,
        int lineNumber, const Rules::RuleName & currentRule, const std::string & msg);
};

} // namespace Plugins

} // namespace Vera

#endif // EXCLUSIONS_H_INCLUDED
