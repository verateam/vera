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
, variables_(NULL)
,it_(it)
,end_(end)
{

  const Token& token = *it;

  if (token.name_.compare(TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_TOKEN);
  }
}

void
StatementOfStruct::initialize()
{
  push(*it_);
  ++it_;

  Statement& current = getCurrentStatement();

  addEachInvalidToken(current, it_, end_);
}

const Statement&
StatementOfStruct::getStatementScope()
{
  if (scope_ == NULL)
  {
    throw StatementsError(WITHOUT_STATEMENT_SCOPE);
  }

  return *scope_;
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

bool StatementOfStruct::parseName()
{
  bool successful = false;

  IS_EQUAL_RETURN_FALSE(it_, end_);
  if (IsTokenWithName(IDENTIFIER_TOKEN_NAME)(*it_) == true )
  {
    push(*it_);

    name_ = &(it_->value_);
    ++it_;
    IS_EQUAL_RETURN_FALSE(it_, end_);
    addEachInvalidToken(getCurrentStatement(), it_, end_);
    successful = true;
  }

  return successful;
}

bool StatementOfStruct::parseHeritage()
{
  bool successful = false;

  IS_EQUAL_RETURN_FALSE(it_, end_);
  if (IsTokenWithName(COLON_TOKEN_NAME)(*it_) == true )
  {
    StatementsBuilder partial(add());

    successful = partial.parseHeritage(it_, end_);
  }

  if (successful)
  {
    hieritance_ = &getCurrentStatement().statementSequence_.back();
  }

  return successful;
}

bool StatementOfStruct::parseScope()
{
  bool successful = false;

  IS_EQUAL_RETURN_FALSE(it_, end_);
  if (IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*it_) == true )
  {
    StatementsBuilder::parseScope(it_, end_);
    scope_ = &getCurrentStatement().statementSequence_.back();

    successful = true;
  }

  return successful;
}

bool StatementOfStruct::parseVariablesFromScopeToSemicolon()
{
  bool successful = false;
  ++it_;
  IS_EQUAL_RETURN_FALSE(it_, end_);

  std::vector<boost::wave::token_id> finishTypeList;
  finishTypeList.push_back(boost::wave::T_SEMICOLON);
  finishTypeList.push_back(boost::wave::T_COMMA);

  successful = StatementsBuilder::parseVariablesFromScopeToSemicolon(it_, end_, finishTypeList);

  if (successful == true)
  {
    variables_ = &getCurrentStatement().statementSequence_.back();
  }

  IS_EQUAL_RETURN_FALSE(it_, end_)
  if (IsTokenWithName(SEMICOLON_TOKEN_NAME)(*it_) == true && successful)
  {
      push(*it_);
  }
  else
    successful = false;

  return successful;
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

bool StatementOfStruct::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end))
  {
    //TODO
    StatementOfStruct builder(StatementsBuilder(statement).add(), it, end);

    builder.initialize();
    builder.parseName();
    builder.parseHeritage();
    builder.parseScope();
    builder.parseVariablesFromScopeToSemicolon();
    successful = true;
  }

  return successful;
}


} // Vera namespace
} // Structures namespace


