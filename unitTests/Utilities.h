//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

#include <structures/Tokens.h>
#include <structures/Statements.h>

namespace Testing
{
/**
 * @brief Class which contains useful functions for the unit test.
 */
class Utilities
{
  public:
    Vera::Structures::Tokens::TokenSequence
    getSubCollection(unsigned offset,
      unsigned size,
      const Vera::Structures::Tokens::TokenSequence& collection);
};

} // Testing namespace

#endif // UTILITIES_H_INCLUDED
