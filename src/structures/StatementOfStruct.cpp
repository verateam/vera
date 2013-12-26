//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfStruct.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define IS_NOT_TOKEN "the first element of the collection is not a token of 'struct' type."
#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME             "struct"
#define IDENTIFIER_TOKEN_NAME  "identifier"
#define LEFTPAREN_TOKEN_NAME   "leftparen"
#define RIGHTPAREN_TOKEN_NAME  "rightparen"
#define LEFTBRACE_TOKEN_NAME   "leftbrace"
#define RIGHTBRACE_TOKEN_NAME  "rightbrace"
#define SEMICOLON_TOKEN_NAME   "semicolon"
#define COLON_TOKEN_NAME       "colon"
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
#define IS_EQUAL_BREAK(left, right) \
  {\
    if (left == right) \
    { \
      break;\
    }\
  }

namespace Vera
{
namespace Structures
{

StatementOfStruct::StatementOfStruct(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, name_(NULL)
, scope_(NULL)
, hieritance_(NULL)
{

  const Token& token = *it;

  if (token.name_.compare(TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_TOKEN);
  }
}



void
StatementOfStruct::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);
  ++it;

  Statement& current = getCurrentStatement();

  addEachInvalidToken(current, it, end);
}

const Statement&
StatementOfStruct::getStatementScope()
{
  if (getCurrentStatement().statementSequence_.size() == 0)
  {
    throw StatementsError(WITHOUT_STATEMENT_SCOPE);
  }

  return getCurrentStatement().statementSequence_[0];
}

bool
StatementOfStruct::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  if (it->name_.compare(TOKEN_NAME) != 0)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it+1,
    end,
    IsValidTokenForStatement());

  IS_EQUAL_RETURN_FALSE(itMatched, end)

  if (IsTokenWithName(IDENTIFIER_TOKEN_NAME)(*itMatched) == false)
  {
    return false;
  }

  ++itMatched;
  IS_EQUAL_RETURN_FALSE(itMatched, end)

  itMatched = std::find_if(itMatched,
      end,
      IsValidTokenForStatement());

  if (IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*itMatched) == true)
  {
    return true;
  }

  if (IsTokenWithName(COLON_TOKEN_NAME)(*itMatched) == false)
  {
    return false;
  }

  Statement auxiliar;
  StatementsBuilder partial(auxiliar);

  if (partial.parseHeritage(itMatched, end) == false)
  {
    return false;
  }

  IS_EQUAL_RETURN_FALSE(itMatched, end)

  itMatched = std::find_if(itMatched,
      end,
      IsValidTokenForStatement());

  if (IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*itMatched) == true)
  {
    return true;
  }

  return false;
}

bool
StatementOfStruct::isValidWithoutName(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  if (it->name_.compare(TOKEN_NAME) != 0)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it+1,
    end,
    IsValidTokenForStatement());

  IS_EQUAL_RETURN_FALSE(itMatched, end)

  if (IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*itMatched) == true)
  {
    return true;
  }

  if (IsTokenWithName(COLON_TOKEN_NAME)(*itMatched) == false)
  {
    return false;
  }

  Statement auxiliar;
  StatementsBuilder partial(auxiliar);

  if (partial.parseHeritage(itMatched, end) == false)
  {
    return false;
  }

  IS_EQUAL_RETURN_FALSE(itMatched, end)

  itMatched = std::find_if(itMatched,
      end,
      IsValidTokenForStatement());

  if (IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*itMatched) == true)
  {
    return true;
  }

  return false;
}


bool StatementOfStruct::parseName(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  IS_EQUAL_RETURN_FALSE(it, end);
  if (IsTokenWithName(IDENTIFIER_TOKEN_NAME)(*it) == true )
  {
    push(*it);

    name_ = &(it->value_);
    ++it;
    IS_EQUAL_RETURN_FALSE(it, end);
    addEachInvalidToken(getCurrentStatement(), it, end);
    successful = true;
  }

  return successful;
}

bool StatementOfStruct::parseHeritage(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  IS_EQUAL_RETURN_FALSE(it, end);
  if (IsTokenWithName(COLON_TOKEN_NAME)(*it) == true )
  {
    StatementsBuilder partial(add());

    successful = partial.parseHeritage(it, end);
  }

  return successful;
}

bool StatementOfStruct::parseScope(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  IS_EQUAL_RETURN_FALSE(it, end);
  if (IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*it) == true )
  {
    StatementsBuilder::parseScope(it, end);

    successful = true;
  }

  return successful;
}


} // Vera namespace
} // Structures namespace


