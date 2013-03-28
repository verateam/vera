//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TRANSFORMATIONS_H_INCLUDED
#define TRANSFORMATIONS_H_INCLUDED

#include <string>


namespace Vera
{
namespace Plugins
{


class Transformations
{
public:
    typedef std::string TransformationName;

    static void executeTransformation(const TransformationName & name);
};

} // namespace Plugins

} // namespace Vera

#endif // TRANSFORMATIONS_H_INCLUDED
