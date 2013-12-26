//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfCases.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define IS_NOT_TOKEN "the first element of the collection is not a token of 'case' type."
#define WITHOUT_CONDITIONAL_ARGUMENTS "The statement not contain conditional arguments."
#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define CASE_TOKEN_NAME  "case"
#define COLON_TOKEN_NAME  "colon"
#define DEFAULT_TOKEN_NAME  "default"
#define IDENTIFIER_TOKEN_NAME  "identifier"
#define INTLIT_TOKEN_NAME      "intlit"
#define CHARLIT_TOKEN_NAME     "charlit"
#define FLOATLIT_TOKEN_NAME    "floatlit"
#define HEXAINT_TOKEN_NAME     "hexaint"
#define LONGINTLIT_TOKEN_NAME  "longintlit"
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
    it->name_.compare(DEFAULT_TOKEN_NAME) == 0 ||
    it->name_.compare(RIGHTBRACE_TOKEN_NAME) == 0 )
  {
    return false;
  }

  return true;
}

StatementOfCases::StatementOfCases(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, isDefault_(false)
{

  const Token& token = *it;

  if (token.name_.compare(CASE_TOKEN_NAME) != 0 && token.name_.compare(DEFAULT_TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_TOKEN);
  }

  isDefault_ = token.name_.compare(DEFAULT_TOKEN_NAME) == 0;

  initialize(it, end);
}

void
StatementOfCases::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = getCurrentStatement();

  current.push(*it);
  ++it;
  IS_EQUAL_RETURN(it, end);

  if (isDefault_ == false) //TODO ugly conditional create factory method
  {
    addEachInvalidToken(current, it, end);
    IS_EQUAL_RETURN(it, end);

    if (it->name_.compare(INTLIT_TOKEN_NAME) != 0 &&
        it->name_.compare(CHARLIT_TOKEN_NAME) != 0 &&
        it->name_.compare(FLOATLIT_TOKEN_NAME) != 0 &&
        it->name_.compare(HEXAINT_TOKEN_NAME) != 0 &&
        it->name_.compare(LONGINTLIT_TOKEN_NAME) != 0 &&
        it->name_.compare(IDENTIFIER_TOKEN_NAME) != 0)
    {
      return;
    }

    current.push(*it);
    ++it;
    IS_EQUAL_RETURN(it, end);
  }

  addEachInvalidToken(current, it, end);

  if (it->name_.compare(COLON_TOKEN_NAME) != 0)
  {
    return;
  }

  current.push(*it);
  ++it;
  IS_EQUAL_RETURN(it, end);
  addEachInvalidToken(current, it ,end);
  Tokens::TokenSequence::const_iterator itMatched = end;

  while(it != end)
  {
    addEachInvalidToken(current, it ,end);
    IS_EQUAL_BREAK(it, end);

    if (it->name_.compare(RIGHTBRACE_TOKEN_NAME) == 0 || canAdvance(it, end) == false)
    {
      break;
    }

    builder(current, it, end);
//    IS_EQUAL_BREAK(it, end);
//
//    itMatched = std::find_if(it+1, end, IsValidTokenForStatement());
//
//    if (itMatched != end && canAdvance(itMatched, end) == false)
//    {
//      break;
//    }

    IS_EQUAL_BREAK(it, end);
    it++;
  }
}

const Statement&
StatementOfCases::getStatementScope()
{
  if (getCurrentStatement().statementSequence_.size() < 2)
  {
    throw StatementsError(WITHOUT_STATEMENT_SCOPE);
  }

  return getCurrentStatement().statementSequence_[1];
}

} // Vera namespace
} // Structures namespace
