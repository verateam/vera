//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfBracketsArguments.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>


#define LEFTBRACKET_TOKEN_NAME  "leftbracket"
#define RIGHTBRACKET_TOKEN_NAME "rightbracket"

#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."

#define IS_EQUAL_RETURN(left, right) \
  {\
    if (left == right) \
    { \
      return;\
    }\
  }
#define IS_EQUAL_RETURN_FALSE(left, right) \
  {\
    if (left == right) \
    { \
      return false;\
    }\
  }
#define IS_EQUAL_BREAK(left, right) \
  {\
    if (left == right) \
    { \
      break;\
    }\
  }

namespace Vera
{
namespace Structures
{

StatementOfBracketsArguments::StatementOfBracketsArguments(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_BRACKETSARGUMENTS;
  initialize(it, end);
}

void
StatementOfBracketsArguments::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Tokens::TokenSequence::const_iterator first;
  Tokens::TokenSequence::const_iterator second;

  Tokens::TokenSequence::const_iterator endMatched = std::find_if(it+1,
    end, EndsWithCorrectPattern(LEFTBRACKET_TOKEN_NAME, RIGHTBRACKET_TOKEN_NAME));

  push(*it);
  ++it;
  addEachInvalidToken(it, endMatched);

  while (it < endMatched)
  {
    builder(it, endMatched);

    IS_EQUAL_BREAK(it, endMatched);
    ++it;
    addEachInvalidToken(it, endMatched);
  }

  IS_EQUAL_RETURN(it, end);

  push(*it);

  if (it < end);
  {
    ++it;
  }
}

bool
StatementOfBracketsArguments::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  return (it->name_.compare(LEFTBRACKET_TOKEN_NAME)  == 0);
}

bool
StatementOfBracketsArguments::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = statement.add();
  StatementOfBracketsArguments builder(current, it, end);
  
  return true;
}

} // Vera namespace
} // Structures namespace
