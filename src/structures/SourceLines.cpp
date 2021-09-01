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
#include <cstring>
#include <cerrno>
#include <boost/algorithm/string.hpp>


namespace // unnamed
{

typedef std::map<Vera::Structures::SourceFiles::FileName,
    Vera::Structures::SourceLines::LineCollection> SourceFileCollection;
typedef std::map<Vera::Structures::SourceFiles::FileName, bool> IsBinaryFileCollection;

SourceFileCollection sources_;
IsBinaryFileCollection areBinaries_;

} // unnamed namespace


namespace Vera
{
namespace Structures
{

const SourceLines::LineCollection & SourceLines::getAllLines(const SourceFiles::FileName & name)
{
    loadFile(name);
    return sources_[name];
}

void SourceLines::loadFile(const SourceFiles::FileName & name)
{
    if (name == "-")
    {
        SourceLines::loadFile(std::cin, name);
    }
    else
    {
        std::ifstream file(name.c_str());
        if (file.is_open() == false)
        {
            std::ostringstream ss;
            ss << "Cannot open source file " << name << ": "
               << strerror(errno);
            throw SourceFileError(ss.str());
        }
        SourceLines::loadFile(file, name);
        if (file.bad())
        {
            throw std::runtime_error(
                "Cannot read from " + name + ": " + strerror(errno));
        }
    }
}

void SourceLines::loadFile(std::istream & file, const SourceFiles::FileName & name)
{
    if (sources_.find(name) != sources_.end())
    {
        return;
    }
    LineCollection & lines = sources_[name];
    bool & isBinary = areBinaries_[name];
    
    std::string line;
    Tokens::FileContent fullSource;

    fullSource.assign( (std::istreambuf_iterator<char>(file) ),
                (std::istreambuf_iterator<char>()    ) );
    if (fullSource.find('\0') != std::string::npos)
    {
        isBinary = true;
        fullSource.clear();
        return;
    }
    boost::split(lines, fullSource, boost::is_any_of("\n"));
    Tokens::parse(name, fullSource);
}

int SourceLines::getLineCount(const SourceFiles::FileName & name)
{
    return static_cast<int>(getAllLines(name).size());
}

bool SourceLines::isBinary(const SourceFiles::FileName & name)
{
    loadFile(name);
    return areBinaries_[name];
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