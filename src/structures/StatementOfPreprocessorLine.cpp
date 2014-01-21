//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfPreprocessorLine.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME  "pp_line"
#define INTLIT_TOKEN_NAME  "intlit"
#define STRINGLIT_TOKEN_NAME  "stringlit"
#define NEWLINE_TOKEN_NAME  "newline"

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

StatementOfPreprocessorLine::StatementOfPreprocessorLine(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  initialize(it, end);
}

void
StatementOfPreprocessorLine::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = getCurrentStatement();

  push(*it);

  ++it;

  IS_EQUAL_RETURN(it, end);
  addEachInvalidToken(current, it, end);

  if (it->name_.compare(INTLIT_TOKEN_NAME) == 0)
  {
    push(*it);

    ++it;
  }

  IS_EQUAL_RETURN(it, end);
  addEachInvalidToken(current, it, end);

  IS_EQUAL_RETURN(it, end);

  if (it->name_.compare(STRINGLIT_TOKEN_NAME) == 0)
  {
    push(*it);
  }

  IS_EQUAL_RETURN(it, end);
  ++it;

}

bool
StatementOfPreprocessorLine::isValid(
    Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  return it->name_.compare(TOKEN_NAME) == 0;
}

bool
StatementOfPreprocessorLine::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end) == true)
  {
    StatementOfPreprocessorLine builder(statement, it, end);
    successful = true;
  }

  return successful;
}

} // Vera namespace
} // Structures namespace


