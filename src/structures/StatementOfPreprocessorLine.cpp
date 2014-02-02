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
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_PREPROCESSORLINE;
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
  addEachInvalidToken(it, end);

  if (it->name_.compare(INTLIT_TOKEN_NAME) == 0)
  {
    push(*it);

    ++it;
  }

  IS_EQUAL_RETURN(it, end);
  addEachInvalidToken(it, end);

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
  StatementOfPreprocessorLine builder(statement.add(), it, end);

  return true;
}

bool
StatementOfPreprocessorLine::requiredContinue()
{
  return true;
}

} // Vera namespace
} // Structures namespace
