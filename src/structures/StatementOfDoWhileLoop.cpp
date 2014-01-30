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
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_DOWHILELOOP;
  initialize(it, end);
}

void
StatementOfDoWhileLoop::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = getCurrentStatement();
  push(*it);
  ++it;

  addEachInvalidToken(it, end);
  IS_EQUAL_RETURN(it, end);
  parseScope(it, end);
  IS_EQUAL_RETURN(it, end);

  //TODO
  if (it->name_.compare(SEMICOLON_TOKEN_NAME) == 0)
  {
    ++it;
  }

  addEachInvalidToken(it, end);

  IS_EQUAL_RETURN(it, end);

  if (it->name_.compare(WHILE_TOKEN_NAME) != 0)
  {
    return;
  }

  IS_EQUAL_RETURN(it, end);
  parseScope(it, end);
  Statement& branch = current.statementSequence_.back();

  branch.type_ = Statement::TYPE_ITEM_STATEMENT_OF_WHILE;
  IS_EQUAL_RETURN(it, end);
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

bool
StatementOfDoWhileLoop::isValid(Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  return (it->name_.compare(DO_TOKEN_NAME) == 0);
}

bool
StatementOfDoWhileLoop::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end) == true)
  {
    StatementOfDoWhileLoop builder(statement.add(), it, end);
    successful = true;
  }

  return successful;
}

} // Vera namespace
} // Structures namespace
