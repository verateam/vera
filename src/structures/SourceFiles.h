//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SOURCEFILES_H_INCLUDED
#define SOURCEFILES_H_INCLUDED

#include <string>
#include <set>
#include <stdexcept>


namespace Vera
{
namespace Structures
{


class SourceFileError : public std::runtime_error
{
public:
    SourceFileError(const std::string & msg) : std::runtime_error(msg) {}
};

class SourceFiles
{
public:
    typedef std::string FileName;
    typedef std::set<FileName> FileNameSet;
    typedef FileNameSet::const_iterator iterator;

    static void addFileName(const FileName & name);

    static bool empty();
    static int count();

    static const FileNameSet & getAllFileNames();
};

} // namespace Structures

} // namespace Vera

#endif // SOURCEFILES_H_INCLUDED
