//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfForLoop.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define IS_NOT_TOKEN "the first element of the collection is not a token of 'for' type."
#define WITHOUT_CONDITIONAL_ARGUMENTS "The statement not contain conditional arguments."
#define WITHOUT_STATEMENT_FOR_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME  "for"
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

namespace Vera
{
namespace Structures
{

bool
hasTokenNameLastSentence(const Statement& current, std::string tokenName)
{
  const Statement& partialStatement = current.statementSequence_.back();
  const Token& token = partialStatement.tokenSequence_.back();

  return IsTokenWithName(tokenName)(token) == true;
}

StatementOfForLoop::StatementOfForLoop(Statement& statement,
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
StatementOfForLoop::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  getCurrentStatement().tokenSequence_.push_back(*it);
  ++it;

  if (parseArguments(it, end) == false)
  {
    return;
  }

  IS_EQUAL_RETURN(it, end);

  parseScope(it, end);
}

const Statement&
StatementOfForLoop::getArgumentStatementFromConditionalSentence()
{
  if (getCurrentStatement().statementSequence_.size() == 0)
  {
    throw StatementsError(WITHOUT_CONDITIONAL_ARGUMENTS);
  }

  return getCurrentStatement().statementSequence_[0];
}

const Statement&
StatementOfForLoop::getStatementScope()
{
  if (getCurrentStatement().statementSequence_.size() < 2)
  {
    throw StatementsError(WITHOUT_STATEMENT_FOR_SCOPE);
  }

  return getCurrentStatement().statementSequence_[1];
}

bool
StatementOfForLoop::parseArguments(Tokens::TokenSequence::const_iterator& it,
     Tokens::TokenSequence::const_iterator& end)
{
  IS_EQUAL_RETURN_FALSE(it, end);

  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it,
    end,
    IsValidTokenForStatement());

  IS_EQUAL_RETURN_FALSE(itMatched, end);

  if (IsTokenWithName(LEFTPAREN_TOKEN_NAME)(*itMatched) == false)
  {
    return false;
  }
  addEachInvalidToken(it, end, getCurrentStatement().tokenSequence_);
  Statement& current = add();
  current.tokenSequence_.push_back(*it);
  ++it;

  StatementsBuilder partialBuilder(current);

  if (extractOneStatementArgument(partialBuilder, it, end, SEMICOLON_TOKEN_NAME) == false)
  {
    return false;
  }

  ++it;
  IS_EQUAL_RETURN_FALSE(it, end);

  if (extractOneStatementArgument(partialBuilder, it, end, SEMICOLON_TOKEN_NAME) == false)
  {
    return false;
  }

  ++it;
  IS_EQUAL_RETURN_FALSE(it, end);

  if (extractOneStatementArgument(partialBuilder, it, end, RIGHTPAREN_TOKEN_NAME) == false)
  {
    return false;
  }

  Statement& argument = current.statementSequence_.back();
  Token token = argument.tokenSequence_.back();
  argument.tokenSequence_.pop_back();
  current.tokenSequence_.push_back(token);
  ++it;
  return true;
}

bool
StatementOfForLoop::extractOneStatementArgument(StatementsBuilder& partialBuilder,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end,
  std::string tokenName)
{
  Statement& current = partialBuilder.add();

  //addEachInvalidToken(it, end, current.tokenSequence_);

  IS_EQUAL_RETURN_FALSE(it, end);

  builder(current, it, end);

  IS_EQUAL_RETURN_FALSE(it, end);

  return IsTokenWithName(tokenName)(*it);//hasTokenNameLastSentence(current, tokenName);
}

} // Vera namespace
} // Structures namespace
