//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ROOTDIRECTORY_H_INCLUDED
#define ROOTDIRECTORY_H_INCLUDED

#include <string>


namespace Vera
{
namespace Plugins
{


class RootDirectory
{
public:
    typedef std::string DirectoryName;

    static void setRootDirectory(const DirectoryName & name);

    static DirectoryName getRootDirectory();
};

} // namespace Plugins

} // namespace Vera

#endif // ROOTDIRECTORY_H_INCLUDED
