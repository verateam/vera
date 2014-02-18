//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfUnion.h"
#include "IsTokenWithName.h"
#include "Document.h"

#include <vector>
#include <map>
#include <algorithm>


#define IS_NOT_TOKEN "the first element of the collection is not a token of 'struct' type."
#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME             "union"
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

StatementOfUnion::StatementOfUnion(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, id_(statement.id_)
{

  name_ = getDefaultName();

  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_UNION_UNNAME;

  initialize(it, end);
}

void
StatementOfUnion::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);
  ++it;
  
  Statement& current = getCurrentStatement();

  current.type_ = Statement::TYPE_ITEM_STATEMENT_OF_UNION;

  addEachInvalidToken(it, end);
}

bool
StatementOfUnion::parseName(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  IS_EQUAL_RETURN_FALSE(it, end);
  if (IsTokenWithName(IDENTIFIER_TOKEN_NAME)(*it) == true )
  {
    push(*it);

    name_ = it->value_;
    ++it;
    IS_EQUAL_RETURN_FALSE(it, end);
    addEachInvalidToken(it, end);
    successful = true;
  }

  return successful;
}

bool
StatementOfUnion::parseVariablesFromScopeToSemicolon(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (StatementsBuilder::parseVariablesFromScopeToSemicolon(it, end))
  {
    Statement* variables_ = &getCurrentStatement().statementSequence_.back();
    variables_->type_ = Statement::TYPE_ITEM_STATEMENT_OF_DECLARATION_LIST;
    successful = true;
  }

  return successful;
}

bool
StatementOfUnion::parseScope(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  StatementsBuilder::parseScope(it, end);
  return true;
}

bool
StatementOfUnion::isValidWithName(Tokens::TokenSequence::const_iterator it,
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

  return IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*itMatched);
}

const std::string&
StatementOfUnion::getName()
{
  return name_;
}

std::size_t
StatementOfUnion::getId()
{
  return id_;
}

bool
StatementOfUnion::isValidWithoutName(Tokens::TokenSequence::const_iterator it,
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

  return IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*itMatched);
}

bool
StatementOfUnion::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  return isValidWithName(it, end) || isValidWithoutName(it, end);
}

bool
StatementOfUnion::create(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;
  bool isValid = false;
  bool hasName = false;

  if (isValidWithName(it, end))
  {
    hasName = true;
    isValid = true;
  }
  else
  {
    hasName = false;
    isValid = true;
  }

  if (isValid)
  {
    //TODO
    StatementOfUnion builder(statement.add(), it, end);

    statement.doc_->enableUnion();
    builder.initialize(it, end);

    if (hasName)
    {
      builder.parseName(it, end);
    }

    builder.parseScope(it, end);
    builder.parseVariablesFromScopeToSemicolon(it, end);
    successful = true;
    statement.doc_->disableUnion();

    statement.doc_->addUnion(builder.getName(), builder.getId());
  }

  return successful;
}

} // Vera namespace
} // Structures namespace
