//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "SourceFiles.h"

using namespace std;
using namespace Vera;
using namespace Vera::Structures;


namespace // unnamed
{

SourceFiles::FileNameSet files_;

} // unnamed namespace


void SourceFiles::addFileName(const FileName & name)
{
    files_.insert(name);
}

bool SourceFiles::empty()
{
    return files_.empty();
}

const SourceFiles::FileNameSet & SourceFiles::getAllFileNames()
{
    return files_;
}
