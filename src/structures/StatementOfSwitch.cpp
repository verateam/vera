//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfSwitch.h"
#include "StatementOfCases.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define IS_NOT_TOKEN "the first element of the collection is not a token of 'switch' type."
#define WITHOUT_CONDITIONAL_ARGUMENTS "The statement not contain conditional arguments."
#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME  "switch"
#define CASE_TOKEN_NAME  "case"
#define DEFAULT_TOKEN_NAME  "default"
#define COLON_TOKEN_NAME  "colon"
#define IDENTIFIER_TOKEN_NAME  "identifier"
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

StatementOfSwitch::StatementOfSwitch(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{

  const Token& token = *it;

  if (token.name_.compare(TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_TOKEN);
  }

  initialize(it, end);
}

void
StatementOfSwitch::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  getCurrentStatement().tokenSequence_.push_back(*it);
  ++it;

  if (parseArguments(it, end) == false)
  {
    return;
  }

  IS_EQUAL_RETURN(it, end);
  ++it;

  parseScope(it, end);
}


void
StatementOfSwitch::parseScope(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  IS_EQUAL_RETURN(it, end)

  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it,
    end,
    IsValidTokenForStatement());

  IS_EQUAL_RETURN(itMatched, end)

  if (IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*itMatched) == false)
  {
    return;
  }

  Statement& current = add();
  addEachInvalidToken(it, end, current.tokenSequence_);

  current.tokenSequence_.push_back(*it);

  for (++it; it != end; ++it)
  {
    addEachInvalidToken(it, end, current.tokenSequence_);

    IS_EQUAL_BREAK(it, end)

    while (IsTokenWithName(CASE_TOKEN_NAME)(*it) == true || IsTokenWithName(DEFAULT_TOKEN_NAME)(*it) == true)
    {
      StatementsBuilder partial(current);
      StatementOfCases(partial.add(), it, end);
      IS_EQUAL_BREAK(it, end)
      addEachInvalidToken(it, end, current.tokenSequence_);
      IS_EQUAL_BREAK(it, end)
      continue;
    }

    IS_EQUAL_BREAK(it, end)

    addEachInvalidToken(it, end, current.tokenSequence_);

    if (IsTokenWithName(RIGHTBRACE_TOKEN_NAME)(*it) == true)
    {
      current.tokenSequence_.push_back(*it);
      //++it;
      break;
    }

    builder(current, it, end);

    IS_EQUAL_BREAK(it, end)
  }

  return;
}

const Statement&
StatementOfSwitch::getArgument()
{
  if (getCurrentStatement().statementSequence_.size() == 0)
  {
    throw StatementsError(WITHOUT_CONDITIONAL_ARGUMENTS);
  }

  return getCurrentStatement().statementSequence_[0];
}

const Statement&
StatementOfSwitch::getStatementScope()
{
  if (getCurrentStatement().statementSequence_.size() < 2)
  {
    throw StatementsError(WITHOUT_STATEMENT_SCOPE);
  }

  return getCurrentStatement().statementSequence_[1];
}

} // Vera namespace
} // Structures namespace
