//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfElse.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>
#include <functional>

#define IS_NOT_TOKEN "the first element of the collection is not a token of 'else' type."
#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME  "else"
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

StatementOfElse::StatementOfElse(const Statement& statement)
: StatementsBuilder(const_cast<Statement&>(statement))
{
  const Statement& current = statement.statementSequence_.front();
  const Token& token = current.token_;

  if (token.name_.compare(TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_TOKEN);
  }

}

StatementOfElse::StatementOfElse(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  initialize(it, end);
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_ELSE;
}

void
StatementOfElse::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);

  ++it;
  IS_EQUAL_RETURN(it, end);
  addEachInvalidToken(it, end);
  IS_EQUAL_RETURN(it, end);

  parseScope(it, end);
}

const Statement::StatementSequence&
StatementOfElse::getStatementScope()
{
  return getCurrentStatement().statementSequence_;
}

bool
StatementOfElse::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
 return it->name_.compare(TOKEN_NAME) == 0;
}

bool
StatementOfElse::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end) == true)
  {
    StatementOfElse builder(StatementsBuilder(statement).add(), it, end);
    successful = true;
  }

  return successful;
}

} // Vera namespace
} // Structures namespace
