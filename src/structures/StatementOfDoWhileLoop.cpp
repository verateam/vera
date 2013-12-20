//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfDoWhileLoop.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define IS_NOT_DO_TOKEN "the first element of the collection is not a token of 'do' type."
#define IS_NOT_WHILE_TOKEN "the element of the collection is not a token of 'while' type."
#define WITHOUT_CONDITIONAL_ARGUMENTS "The statement not contain conditional arguments."
#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define DO_TOKEN_NAME  "do"
#define WHILE_TOKEN_NAME  "while"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME "rightbrace"
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

StatementOfDoWhileLoop::StatementOfDoWhileLoop(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{

  const Token& token = *it;

  if (token.name_.compare(DO_TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_DO_TOKEN);
  }

  initialize(it, end);
}

void
StatementOfDoWhileLoop::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  getCurrentStatement().tokenSequence_.push_back(*it);
  ++it;
  IS_EQUAL_RETURN(it, end);
  parseScope(it, end);
  IS_EQUAL_RETURN(it, end);

  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it+1, end, IsValidTokenForStatement());

  IS_EQUAL_RETURN(itMatched, end);


  if (itMatched->name_.compare(WHILE_TOKEN_NAME) != 0)
  {
    return;
  }

  StatementsBuilder partial(add());
  Statement& current = partial.getCurrentStatement();

  ++it;
  addEachInvalidToken(it, end, getCurrentStatement().tokenSequence_);

  current.tokenSequence_.push_back(*it);


  IS_EQUAL_RETURN(it, end);
  ++it;
  IS_EQUAL_RETURN(it, end);
  partial.addEachInvalidToken(it, end, current.tokenSequence_);
  IS_EQUAL_RETURN(it, end);
  if(partial.parseArguments(it, end) == false)
  {
    return;
  }

  IS_EQUAL_RETURN(it, end);
  ++it;
  IS_EQUAL_RETURN(it, end);
  partial.addEachInvalidToken(it, end, current.tokenSequence_);
  IS_EQUAL_RETURN(it, end);

  if (it->name_.compare(SEMICOLON_TOKEN_NAME) == 0)
  {
    current.tokenSequence_.push_back(*it);
  }
}

const Statement&
StatementOfDoWhileLoop::getArgumentStatementFromConditionalSentence()
{
  if (getCurrentStatement().statementSequence_.size() < 2)
  {
    throw StatementsError(WITHOUT_CONDITIONAL_ARGUMENTS);
  }

  return getCurrentStatement().statementSequence_[1];
}

const Statement&
StatementOfDoWhileLoop::getStatementScope()
{
  if (getCurrentStatement().statementSequence_.size() == 0)
  {
    throw StatementsError(WITHOUT_STATEMENT_SCOPE);
  }

  return getCurrentStatement().statementSequence_[0];
}

} // Vera namespace
} // Structures namespace
