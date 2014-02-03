//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfCase.h"
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

StatementOfCase::StatementOfCase(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  const Token& token = *it;

  if (token.name_.compare(CASE_TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_TOKEN);
  }

  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_CASE;
  initialize(it, end);
}

void
StatementOfCase::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = getCurrentStatement();

  push(*it);
  ++it;
  IS_EQUAL_RETURN(it, end);

  addEachInvalidToken(it, end);
  IS_EQUAL_RETURN(it, end);

  Tokens::TokenSequence::const_iterator colonMatched =
    std::find_if(it, end, IsTokenWithName(COLON_TOKEN_NAME));

  parse(it, colonMatched);

  push(*it);
  ++it;

  if (canAdvance(it+1, end) == false)
  {
    return;
  }
  ++it;

  IS_EQUAL_RETURN(it, end);

  if (it->line_ < 374 && it->line_ >300)
  {
    IS_EQUAL_RETURN(it, end);
    IS_EQUAL_RETURN(it, end);
    IS_EQUAL_RETURN(it, end);
  }

  Tokens::TokenSequence::const_iterator caseMatched = std::find_if(it,
      end,
      IsTokenWithName(CASE_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator defaultMatched = std::find_if(it,
      end,
      IsTokenWithName(DEFAULT_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator endMatched = end;

  if (caseMatched < defaultMatched)
  {
    endMatched = caseMatched;
  }
  else
  {
    endMatched = defaultMatched;
  }

  while(it < endMatched)
  {
    if (IsValidTokenForStatement()(*it) == false)
    {
      push(*it);
    }
    else if (it->name_.compare(LEFTBRACE_TOKEN_NAME) == 0)
    {
      parseScope(it, endMatched);
    }
    else
    {
      builder(it, endMatched);
    }

    IS_EQUAL_BREAK(it, endMatched);

    if (canAdvance(it+1, endMatched) == false)
    {
      break;
    }

    if (it < endMatched)
    {
      it++;
    }
  }
}

bool
StatementOfCase::isValid(
    Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  if (it->name_.compare(CASE_TOKEN_NAME) != 0)
  {
    return false;
  }

  ++it;

  Tokens::TokenSequence::const_iterator colonMatched =
    std::find_if(it, end, IsTokenWithName(COLON_TOKEN_NAME));
  Tokens::TokenSequence::const_iterator semicolonMatched =
    std::find_if(it, end, IsTokenWithName(SEMICOLON_TOKEN_NAME));

  return colonMatched < semicolonMatched && colonMatched < end;
}

bool
StatementOfCase::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = statement.add();
  StatementOfCase builder(current, it, end);

  //if (it != end)
   // --it;

  return true;
}

} // Vera namespace
} // Structures namespace

