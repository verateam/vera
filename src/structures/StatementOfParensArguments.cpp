//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfParensArguments.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME "rightbrace"
#define SEMICOLON_TOKEN_NAME  "semicolon"
#define COMMA_TOKEN_NAME      "comma"
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

StatementOfParensArguments::StatementOfParensArguments(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{

  initialize(it, end);
}

void
StatementOfParensArguments::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Tokens::TokenSequence::const_iterator first;
  Tokens::TokenSequence::const_iterator second;

  struct Statement& current = add();

  current.type_ = Statement::TYPE_ITEM_STATEMENT_OF_PARENSARGUMENTS;

  Tokens::TokenSequence::const_iterator endMatched = std::find_if(it+1,
    end, EndsWithCorrectPattern(LEFTPAREN_TOKEN_NAME, RIGHTPAREN_TOKEN_NAME));

  class StatementsBuilder partial(current);
  partial.push(*it);
  ++it;
  partial.addEachInvalidToken(it, endMatched);

  while (it < endMatched)
  {
    partial.builder(current, it, endMatched);

    IS_EQUAL_BREAK(it, endMatched);
    ++it;

    partial.addEachInvalidToken(it, endMatched);
  }

  IS_EQUAL_RETURN(it, end);

  partial.push(*it);

  if (it < end)
  {
    ++it;
  }
}

const Statement&
StatementOfParensArguments::getStatementScope()
{
  if (getCurrentStatement().statementSequence_.size() == 0)
  {
    throw StatementsError(WITHOUT_STATEMENT_SCOPE);
  }

  return getCurrentStatement().statementSequence_[0];
}

bool
StatementOfParensArguments::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  return (it->name_.compare(LEFTPAREN_TOKEN_NAME)  == 0);
}

bool
StatementOfParensArguments::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end) == true)
  {
    StatementOfParensArguments builder(statement, it, end);
  }

  return successful;
}

} // Vera namespace
} // Structures namespace
