//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfEnum.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME  "enum"
#define IDENTIFIER_TOKEN_NAME  "identifier"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME  "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME  "rightbrace"
#define SEMICOLON_TOKEN_NAME  "semicolon"
#define COMMA_TOKEN_NAME  "comma"
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

StatementOfEnum::StatementOfEnum(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, variables_(NULL)
{
  initialize(it, end);
}

bool
StatementOfEnum::parseName(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);
  ++it;
  return true;
}

void
StatementOfEnum::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);
  ++it;

  Statement& current = getCurrentStatement();

  addEachInvalidToken(it, end);

  IS_EQUAL_RETURN(it, end);
}

bool
StatementOfEnum::parseScope(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  addEachInvalidToken(it, end);
  IS_EQUAL_RETURN_FALSE(it, end);

  Tokens::TokenSequence::const_iterator endScope = std::find_if(it+1,
      end,
      EndsWithCorrectPattern(LEFTBRACE_TOKEN_NAME, RIGHTBRACE_TOKEN_NAME));

  StatementsBuilder::parseScope(it, end);
  IS_EQUAL_RETURN_FALSE(it, end);
  addEachInvalidToken(it, end);

  return true;
}

bool
StatementOfEnum::parseVariablesFromScopeToSemicolon(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  Statement& current = getCurrentStatement();

  addEachInvalidToken(it, end);

  if (StatementsBuilder::parseVariablesFromScopeToSemicolon(it, end))
  {
    variables_ = &current.statementSequence_.back();
    successful = true;
  }

  return successful;
}


const Statement&
StatementOfEnum::getStatementScope()
{
  if (getCurrentStatement().statementSequence_.size() == 0)
  {
    throw StatementsError(WITHOUT_STATEMENT_SCOPE);
  }

  return getCurrentStatement().statementSequence_[0];
}

bool
StatementOfEnum::isValidWithName(Tokens::TokenSequence::const_iterator it,
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

  if (IsTokenWithName(IDENTIFIER_TOKEN_NAME)(*itMatched) == false)
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
StatementOfEnum::isValidWithoutName(Tokens::TokenSequence::const_iterator it,
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

  return IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*itMatched);
}

bool
StatementOfEnum::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  return isValidWithName(it, end);
}

bool
StatementOfEnum::create(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end) == true)
  {
    StatementOfEnum branch(StatementsBuilder(statement).add(), it, end);

    branch.parseName(it, end);
    branch.parseScope(it, end);
    branch.parseVariablesFromScopeToSemicolon(it, end);
  }

  return successful;
}

} // Vera namespace
} // Structures namespace
