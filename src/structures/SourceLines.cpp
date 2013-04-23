//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "SourceLines.h"
#include "Tokens.h"
#include "../plugins/Reports.h"
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>


namespace // unnamed
{

typedef std::map<Vera::Structures::SourceFiles::FileName,
    Vera::Structures::SourceLines::LineCollection> SourceFileCollection;

SourceFileCollection sources_;

} // unnamed namespace


namespace Vera
{
namespace Structures
{

const SourceLines::LineCollection & SourceLines::getAllLines(const SourceFiles::FileName & name)
{
    const SourceFileCollection::const_iterator it = sources_.find(name);
    if (it != sources_.end())
    {
        return it->second;
    }
    else
    {
        // lazy load of the source file
        loadFile(name);
        return sources_[name];
    }
}

void SourceLines::loadFile(const SourceFiles::FileName & name)
{
    std::ifstream file(name.c_str());
    if (file.is_open() == false)
    {
        std::ostringstream ss;
        ss << "cannot open source file " << name;
        throw SourceFileError(ss.str());
    }

    LineCollection & lines = sources_[name];

    std::string line;
    Tokens::FileContent fullSource;
    while (getline(file, line))
    {
        lines.push_back(line);
        fullSource += line;

        // built-in rule
        if (file.eof())
        {
            Plugins::Reports::add(name, static_cast<int>(lines.size()),
                "no newline at end of file");
        }
        else
        {
            fullSource += '\n';
        }
    }

    Tokens::parse(name, fullSource);
}

int SourceLines::getLineCount(const SourceFiles::FileName & name)
{
    return static_cast<int>(getAllLines(name).size());
}

const std::string & SourceLines::getLine(const SourceFiles::FileName & name, int lineNumber)
{
    const LineCollection & lines = getAllLines(name);
    if (lineNumber < 1 || lineNumber > static_cast<int>(lines.size()))
    {
        std::cerr << "Requested wrong line number: " << lineNumber << '\n';
        std::cerr << "lines.size in " << name << " is " << static_cast<int>(lines.size()) << '\n';
        throw SourceFileError("requested line number is out of range");
    }

    return lines[lineNumber - 1];
}

}
}
