//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfDefault.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define IS_NOT_TOKEN "the first element of the collection is not a\
valid token of modifier access type."
#define WITHOUT_CONDITIONAL_ARGUMENTS "The statement not contain conditional arguments."
#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define COLON_TOKEN_NAME  "colon"
#define CASE_TOKEN_NAME      "case"
#define DEFAULT_TOKEN_NAME     "default"
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

static bool
canAdvance( Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  IS_EQUAL_RETURN_FALSE(it, end)

  if (it->name_.compare(CASE_TOKEN_NAME) == 0 ||
    it->name_.compare(DEFAULT_TOKEN_NAME) == 0  ||
    it->name_.compare(RIGHTBRACE_TOKEN_NAME) == 0)
  {
    return false;
  }

  return true;
}

StatementOfDefault::StatementOfDefault(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, it_(it)
, end_(end)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_DEFAULT;
  initialize();
}

void
StatementOfDefault::initialize()
{
  Statement& current = getCurrentStatement();

  current.push(*it_);
  ++it_;
  IS_EQUAL_RETURN(it_, end_);

  addEachInvalidToken(it_, end_);

  if (it_->name_.compare(COLON_TOKEN_NAME) != 0)
  {
    return;
  }

  current.push(*it_);

  if (canAdvance(it_+1, end_) == false)
  {
    return;
  }

  ++it_;

  IS_EQUAL_RETURN(it_, end_);

  Tokens::TokenSequence::const_iterator itMatched = end_;

  while (it_ < end_)
  {
    if (IsValidTokenForStatement()(*it_) == false)
    {
      push(*it_);
    }
    else if (it_->name_.compare(LEFTBRACE_TOKEN_NAME) == 0)
    {
      parseScope(it_, end_);
    }
    else
    {
      builder(it_, end_);
    }

    IS_EQUAL_BREAK(it_, end_);

    if (canAdvance(it_+1, end_) == false)
    {
      break;
    }

    it_++;
  }
}

bool
StatementOfDefault::isValid(
    Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  if (it->name_.compare(DEFAULT_TOKEN_NAME) != 0)
  {
    return false;
  }
  ++it;

  IS_EQUAL_RETURN_FALSE(it, end)

  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it,
      end,
      IsValidTokenForStatement());

  IS_EQUAL_RETURN_FALSE(itMatched, end)

  return itMatched->name_.compare(COLON_TOKEN_NAME) == 0;
}

bool
StatementOfDefault::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  StatementOfDefault  builder(statement.add(), it, end);

  if (it != end)
  {
    --it;
  }

  return true;
}

} // Vera namespace
} // Structures namespace
