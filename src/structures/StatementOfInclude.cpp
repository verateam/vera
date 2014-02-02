//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfInclude.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>


#define TOKEN_NAME  "pp_hheader"
#define HEADER_TOKEN_NAME  "pp_qheader"

#define IS_EQUAL_RETURN(left, right) \
  {\
    if (left == right) \
    { \
      return;\
    }\
  }
#define IS_EQUAL_BREAK(left, right) \
  {\
    if (left == right) \
    { \
      break;\
    }\
  }

#define IS_EQUAL_RETURN_FALSE(left, right) \
  {\
    if (left == right) \
    { \
      return false;\
    }\
  }

namespace Vera
{
namespace Structures
{

StatementOfInclude::StatementOfInclude(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_INCLUDE;
  initialize(it, end);
}

void
StatementOfInclude::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = getCurrentStatement();

  push(*it);

  ++it;
}

bool
StatementOfInclude::isValid(
    Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  return it->name_.compare(TOKEN_NAME) == 0 || it->name_.compare(HEADER_TOKEN_NAME) == 0;
}

bool
StatementOfInclude::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  StatementOfInclude builder(statement.add(), it, end);

  return true;
}

} // Vera namespace
} // Structures namespace
