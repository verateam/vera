//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfOperator.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define TOKEN_NAME  "operator"
#define LESS_TOKEN_NAME       "less"
#define GREATER_TOKEN_NAME    "greater"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME "rightbrace"
#define SEMICOLON_TOKEN_NAME  "semicolon"
#define COMMA_TOKEN_NAME  "comma"
#define OROR_TOKEN_NAME       "oror"
#define ANDAND_TOKEN_NAME     "andand"
#define EQUAL_TOKEN_NAME      "equal"
#define NOTEQUAL_TOKEN_NAME   "notequal"
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

StatementOfOperator::StatementOfOperator(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_OPERATOR;
  initialize(it, end);
}

void
StatementOfOperator::initialize(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  push(*it);
  ++it;

  addEachInvalidToken(it, end);
  IS_EQUAL_RETURN(it, end)

  if (IsTokenWithName(LEFTPAREN_TOKEN_NAME)(*it))
  {
    push(*it);
    IS_EQUAL_RETURN(it, end)
    ++it;
    push(*it);
    ++it;
  }

  for (;it < end; ++it)
  {
    if (IsTokenWithName(LEFTPAREN_TOKEN_NAME)(*it))
    {
      break;
    }

    if (IsTokenWithName(SEMICOLON_TOKEN_NAME)(*it))
     {
       break;
     }
    push(*it);
  }

}

//TODO fail
bool
StatementOfOperator::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  return IsTokenWithName(TOKEN_NAME)(*it);
}

bool
StatementOfOperator::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  StatementOfOperator builder(statement, it, end);

  return true;
}

bool
StatementOfOperator::requiredContinue()
{
  return true;
}

} // Vera namespace
} // Structures namespace
