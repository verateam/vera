//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfEnum.h"
#include "IsTokenWithName.h"
#include "Document.h"
#include <vector>
#include <map>
#include <algorithm>

#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME  "enum"
#define IDENTIFIER_TOKEN_NAME  "identifier"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME  "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME  "rightbrace"
#define SEMICOLON_TOKEN_NAME  "semicolon"
#define COMMA_TOKEN_NAME  "comma"
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

StatementOfEnum::StatementOfEnum(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, variables_(NULL)
, id_(statement.id_)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_ENUM_UNNAME;

  std::stringstream out;

  name_ = getDefaultName();

  initialize(it, end);
}

bool
StatementOfEnum::parseName(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  name_ = it->value_;

  push(*it);
  ++it;

  Statement& current = getCurrentStatement();

  current.type_ = Statement::TYPE_ITEM_STATEMENT_OF_ENUM;

  return true;
}

void
StatementOfEnum::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);
  ++it;

  Statement& current = getCurrentStatement();

  addEachInvalidToken(it, end);

  IS_EQUAL_RETURN(it, end);
}

bool
StatementOfEnum::parseScope(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  addEachInvalidToken(it, end);
  IS_EQUAL_RETURN_FALSE(it, end);

  Tokens::TokenSequence::const_iterator endScope = std::find_if(it+1,
      end,
      EndsWithCorrectPattern(LEFTBRACE_TOKEN_NAME, RIGHTBRACE_TOKEN_NAME));

  StatementsBuilder::parseScope(it, end);
  IS_EQUAL_RETURN_FALSE(it, end);
  addEachInvalidToken(it, end);

  return true;
}

bool
StatementOfEnum::parseVariablesFromScopeToSemicolon(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  Statement& current = getCurrentStatement();

  addEachInvalidToken(it, end);

  if (StatementsBuilder::parseVariablesFromScopeToSemicolon(it, end))
  {
    variables_ = &current.statementSequence_.back();
    variables_->type_ = Statement::TYPE_ITEM_STATEMENT_OF_DECLARATION_LIST;
    successful = true;
  }

  return successful;
}

bool
StatementOfEnum::isValidWithName(Tokens::TokenSequence::const_iterator it,
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

  itMatched = std::find_if(itMatched+1,
      end,
      IsValidTokenForStatement());

  IS_EQUAL_RETURN_FALSE(itMatched, end)

  return IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*itMatched);
}

bool
StatementOfEnum::isValidWithoutName(Tokens::TokenSequence::const_iterator it,
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

const std::string&
StatementOfEnum::getName()
{
  return name_;
}

std::size_t
StatementOfEnum::getId()
{
  return id_;
}

bool
StatementOfEnum::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  return isValidWithName(it, end);
}

bool
StatementOfEnum::create(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  StatementOfEnum branch(statement.add(), it, end);

  branch.parseName(it, end);
  branch.parseScope(it, end);
  branch.parseVariablesFromScopeToSemicolon(it, end);

  statement.doc_->addEnum(branch.getName(), branch.getId());

  return true;
}

} // Vera namespace
} // Structures namespace
