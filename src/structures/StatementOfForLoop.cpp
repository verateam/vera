//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfForLoop.h"

#include <vector>
#include <map>
#include <algorithm>

#define IS_NOT_TOKEN "the first element of the collection is not a token of type 'for'."
#define TOKEN_NAME  "for"

namespace Vera
{
namespace Structures
{

StatementOfForLoop::StatementOfForLoop(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, statement_(statement)
{

  const Token& token = *it;

  if (token.name_.compare(TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_TOKEN);
  }

  setCurrentStatement(add());

  builder(getCurrentStatement(), it, end);
}

} // Vera namespace
} // Structures namespace
