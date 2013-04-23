//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "RootDirectory.h"


namespace // unnamed
{

Vera::Plugins::RootDirectory::DirectoryName root_;

} // unnamed namespace

namespace Vera
{
namespace Plugins
{

void RootDirectory::setRootDirectory(const DirectoryName & name)
{
    root_ = name;
}

RootDirectory::DirectoryName RootDirectory::getRootDirectory()
{
    return root_;
}

}
}
