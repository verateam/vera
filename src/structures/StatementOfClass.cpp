//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfClass.h"
#include "IsTokenWithName.h"
#include "StatementOfTemplateParameters.h"

#include <vector>
#include <map>
#include <algorithm>

#define IS_NOT_TOKEN "the first element of the collection is not a token of 'class' type."
#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME             "class"
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

StatementOfClass::StatementOfClass(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, name_(NULL)
, scope_(NULL)
, hieritance_(NULL)
, variables_(NULL)
, it_(it)
, end_(end)
{
  const Token& token = *it;

  if (token.name_.compare(TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_TOKEN);
  }
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_CLASS;
}

void
StatementOfClass::initialize()
{
  push(*it_);
  ++it_;
  addEachInvalidToken(it_, end_);
}

const Statement&
StatementOfClass::getStatementScope()
{
  if (scope_ == NULL)
  {
    throw StatementsError(WITHOUT_STATEMENT_SCOPE);
  }

  return *scope_;
}

bool
StatementOfClass::parseName()
{
  bool successful = false;

  IS_EQUAL_RETURN_FALSE(it_, end_);

  if (IsTokenWithName(IDENTIFIER_TOKEN_NAME)(*it_) == true )
  {
    push(*it_);

    name_ = &(it_->value_);
    ++it_;
    IS_EQUAL_RETURN_FALSE(it_, end_);
    addEachInvalidToken(it_, end_);
    successful = true;
  }

  return successful;
}

bool
StatementOfClass::parseHeritage()
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
    hieritance_->type_ = Statement::TYPE_ITEM_STATEMENT_OF_HERITAGE;
  }

  return successful;
}

bool
StatementOfClass::parseScope()
{
  bool successful = false;

  IS_EQUAL_RETURN_FALSE(it_, end_);
  if (IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*it_) == false )
  {
    return false;
  }

  Statement& current = add();
  current.type_ = Statement::TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS;
  StatementsBuilder branch(current);

  branch.push(*it_);

  it_++;

  Tokens::TokenSequence::const_iterator rightBraceMached = std::find_if(it_,
    end_,
    EndsWithCorrectPattern(LEFTBRACE_TOKEN_NAME, RIGHTBRACE_TOKEN_NAME));

  branch.addEachInvalidToken(it_, end_);

  while (it_ < rightBraceMached)
  {
    //TODO constructors??
    branch.parse(it_, rightBraceMached);

    branch.addEachInvalidToken(it_, rightBraceMached);

    branch.push(*it_);
    ++it_;
  }

//  if (it_ < end_)
//  {
//    branch.push(*it_);
//    ++it_;
//  }

  scope_ = &current;

  return successful;
}

bool
StatementOfClass::parseVariablesFromScopeToSemicolon()
{
  bool successful = false;

  Statement& current = getCurrentStatement();

  addEachInvalidToken(it_, end_);

  if (StatementsBuilder::parseVariablesFromScopeToSemicolon(it_, end_))
  {
    variables_ = &current.statementSequence_.back();
    //variables_->type_ = Statement::TYPE_ITEM_STATEMENT_OF_DECLARATION_LIST;
    successful = true;
  }

  return successful;
}

bool
StatementOfClass::isValid(Tokens::TokenSequence::const_iterator it,
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

  Token aux("", 0, 0, "");
  Statement auxiliar(aux);
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
StatementOfClass::create(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  StatementOfClass builder(statement.add(), it, end);

  builder.initialize();
  builder.parseName();
  builder.addEachInvalidToken(it, end);

  if (StatementOfTemplateParameters::isValid(it, end))
  {
    StatementOfTemplateParameters(builder.getCurrentStatement(), it, end);
  }

  builder.parseHeritage();
  builder.parseScope();
  builder.parseVariablesFromScopeToSemicolon();

  return true;
}

} // Vera namespace
} // Structures namespace

