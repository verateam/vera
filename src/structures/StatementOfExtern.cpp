//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfExtern.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define TOKEN_NAME  "EXTERN"
#define STRINGLIT_TOKEN_NAME  "stringlit"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME  "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME  "rightbrace"
#define SEMICOLON_TOKEN_NAME  "semicolon"
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

StatementOfExtern::StatementOfExtern(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  initialize(it, end);
}

void
StatementOfExtern::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = getCurrentStatement();

  push(*it);
  ++it;

  addEachInvalidToken(it, end);

  IS_EQUAL_RETURN(it, end);

  push(*it);
  ++it;

  IS_EQUAL_RETURN(it, end);

  addEachInvalidToken(it, end);

  IS_EQUAL_RETURN(it, end);

  parseScope(it, end);
}

const Statement&
StatementOfExtern::getStatementScope()
{
  if (getCurrentStatement().statementSequence_.size() == 0)
  {
    throw StatementsError(WITHOUT_STATEMENT_SCOPE);
  }

  return getCurrentStatement().statementSequence_[0];
}

bool
StatementOfExtern::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  if (it->name_.compare(TOKEN_NAME) != 0)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it+1,
    end,
    IsValidTokenForStatement());

  IS_EQUAL_RETURN_FALSE(itMatched, end)


  if (IsTokenWithName(STRINGLIT_TOKEN_NAME)(*itMatched) == false)
  {
    return false;
  }

  itMatched = std::find_if(itMatched+1,
   end,
   IsValidTokenForStatement());

  IS_EQUAL_RETURN_FALSE(itMatched, end)

  return IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*itMatched);
}

bool
StatementOfExtern::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end) == true)
  {
    StatementOfExtern builder(StatementsBuilder(statement).add(), it, end);
  }

  return successful;
}

} // Vera namespace
} // Structures namespace
