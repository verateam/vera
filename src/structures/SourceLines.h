//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SOURCELINES_H_INCLUDED
#define SOURCELINES_H_INCLUDED

#include "SourceFiles.h"
#include <vector>
#include <iostream>

namespace Vera
{
namespace Structures
{


class SourceLines
{
public:
    typedef std::vector<std::string> LineCollection;

    static const LineCollection & getAllLines(const SourceFiles::FileName & name);
    static int getLineCount(const SourceFiles::FileName & name);
    static const std::string & getLine(const SourceFiles::FileName & name, int lineNumber);

    static void loadFile(const SourceFiles::FileName & name);
    static void loadFile(std::istream & file, const SourceFiles::FileName & name);
};

} // namespace Structures

} // namespace Vera

#endif // SOURCELINES_H_INCLUDED
