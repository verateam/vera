//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Transformations.h"
#include "RootDirectory.h"
#include "Interpreter.h"

namespace Vera
{
namespace Plugins
{

void Transformations::executeTransformation(const TransformationName & name)
{
    const RootDirectory::DirectoryName veraRoot = RootDirectory::getRootDirectory();
    Interpreter::execute(veraRoot, Interpreter::transformation, name);
}

}
}
