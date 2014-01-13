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

  const Token& token = *it;

  if (token.name_.compare(TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_TOKEN);
  }

  initialize(it, end);
}

void
StatementOfTryCatches::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);

  ++it;

  IS_EQUAL_RETURN(it, end);

  parseScope(it, end);
  Tokens::TokenSequence::const_iterator   itMatched  = end;

  do
  {
    itMatched = std::find_if(it+1, end, IsValidTokenForStatement());

    IS_EQUAL_BREAK(itMatched, end);

    if (itMatched->name_.compare(CATCH_TOKEN_NAME) == 0)
    {
      Statement& current = add();
      ++it;
      addEachInvalidToken(current, it , end);
      StatementOfCatch partialBuilder(current, it, end);

      collection_.push_back(&partialBuilder.getCurrentStatement());
    }
    else
    {
      break;
    }

  } while(itMatched != end && it != end);

}


StatementOfTryCatches::iteratorOfCatchStatements
StatementOfTryCatches::begin()
{
  return collection_.begin();
}

StatementOfTryCatches::iteratorOfCatchStatements
StatementOfTryCatches::end()
{
  return collection_.end();
}


const Statement&
StatementOfTryCatches::getStatementScope()
{
  if (getCurrentStatement().statementSequence_.size() < 2)
  {
    throw StatementsError(WITHOUT_STATEMENT_SCOPE);
  }

  return getCurrentStatement().statementSequence_[1];
}

bool StatementOfTryCatches::isValid(Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
 return true;
}

bool
StatementOfTryCatches::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end) == true)
  {
    StatementOfTryCatches builder(StatementsBuilder(statement).add(), it, end);
    successful = true;
  }

  return successful;
}

} // Vera namespace
} // Structures namespace
