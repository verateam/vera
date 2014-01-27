//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfAccessModifiers.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define IS_NOT_TOKEN "the first element of the collection is not a valid token of modifier access type."
#define WITHOUT_CONDITIONAL_ARGUMENTS "The statement not contain conditional arguments."
#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define COLON_TOKEN_NAME  "colon"
#define PUBLIC_TOKEN_NAME      "public"
#define PRIVATE_TOKEN_NAME     "private"
#define PROTECTED_TOKEN_NAME   "protected"
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

  if (it->name_.compare(PUBLIC_TOKEN_NAME) == 0 ||
    it->name_.compare(PRIVATE_TOKEN_NAME) == 0 ||
    it->name_.compare(PROTECTED_TOKEN_NAME) == 0 ||
    it->name_.compare(RIGHTBRACE_TOKEN_NAME) == 0 )
  {
    return false;
  }

  return true;
}

StatementOfAccessModifiers::StatementOfAccessModifiers(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, it_(it)
, end_(end)
{
  const Token& token = *it;

  if (token.name_.compare(PUBLIC_TOKEN_NAME) != 0 &&
      token.name_.compare(PRIVATE_TOKEN_NAME) != 0 &&
      token.name_.compare(PROTECTED_TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_TOKEN);
  }

  initialize();
}

void
StatementOfAccessModifiers::initialize()
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
    else
    {
      builder(current, it_, end_);
    }

    IS_EQUAL_BREAK(it_, end_);

    if (canAdvance(it_+1, end_) == false)
    {
      break;
    }

    it_++;
  }
}

const Statement&
StatementOfAccessModifiers::getStatementScope()
{
  if (getCurrentStatement().statementSequence_.size() < 2)
  {
    throw StatementsError(WITHOUT_STATEMENT_SCOPE);
  }

  return getCurrentStatement().statementSequence_[1];
}

bool
StatementOfAccessModifiers::isValid(
    Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  if (it->name_.compare(PUBLIC_TOKEN_NAME) != 0 &&
      it->name_.compare(PRIVATE_TOKEN_NAME) != 0 &&
      it->name_.compare(PROTECTED_TOKEN_NAME) != 0)
  {
    return false;
  }
  ++it;

  IS_EQUAL_RETURN_FALSE(it, end)

  Tokens::TokenSequence::const_iterator itMatched =
    std::find_if(it, end, IsValidTokenForStatement());

  IS_EQUAL_RETURN_FALSE(itMatched, end)

  return itMatched->name_.compare(COLON_TOKEN_NAME) == 0;
}

bool
StatementOfAccessModifiers::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end))
  {
    //TODO
    StatementOfAccessModifiers builder(StatementsBuilder(statement).add(), it, end);

    if (it < end)
    {
      ++it;
    }

    successful = true;
  }

  return successful;
}

} // Vera namespace
} // Structures namespace
