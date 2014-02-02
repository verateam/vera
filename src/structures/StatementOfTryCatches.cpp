//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfTryCatches.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>
#include <functional>

#define IS_NOT_TOKEN "the first element of the collection is not a token of 'try' type."
#define WITHOUT_CONDITIONAL_ARGUMENTS "The statement not contain conditional arguments."
#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME  "try"
#define CATCH_TOKEN_NAME  "catch"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME  "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME  "rightbrace"
#define SEMICOLON_TOKEN_NAME  "semicolon"
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

StatementOfTryCatches::StatementOfTryCatches(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_TRYCATCHES;
  initialize(it, end);
}

void
StatementOfTryCatches::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);

  ++it;

  addEachInvalidToken(it, end);

  IS_EQUAL_RETURN(it, end);

  parseScope(it, end);
  Tokens::TokenSequence::const_iterator   itMatched  = end;
  do
  {
    itMatched = std::find_if(it, end, IsValidTokenForStatement());

    IS_EQUAL_BREAK(itMatched, end);

    if (itMatched->name_.compare(CATCH_TOKEN_NAME) == 0)
    {
      Statement& current = add();

      addEachInvalidToken(it, end);
      StatementOfCatch branch(current, it, end);
    }
    else
    {
      break;
    }

  }
  while (itMatched != end && it != end);

}

bool StatementOfTryCatches::isValid(Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  const Token& token = *it;

  return (token.name_.compare(TOKEN_NAME) == 0);
}

bool
StatementOfTryCatches::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{

  StatementOfTryCatches builder(statement.add(), it, end);

  return true;
}

} // Vera namespace
} // Structures namespace
