//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfIf.h"
#include "IsTokenWithName.h"
#include "StatementOfParensArguments.h"


#include <vector>
#include <map>
#include <algorithm>

#define IS_NOT_TOKEN "The first element of the collection is not a token of 'if' type."
#define WITHOUT_CONDITIONAL_ARGUMENTS "The statement not contain conditional arguments."
#define WITHOUT_STATEMENT_IF_SCOPE "The 'if' statement not contain a scope associated."
#define WITHOUT_STATEMENT_ELSE_SCOPE "The 'else' statement not contain a scope associated."
#define TOKEN_NAME  "if"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME  "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME  "rightbrace"
#define ELSE_TOKEN_NAME  "else"
#define SEMICOLON_TOKEN_NAME  "semicolon"
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

namespace Vera
{
namespace Structures
{

StatementOfIf::StatementOfIf(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder::StatementsBuilder(statement)
{
  const Token& token = *it;

  if (token.name_.compare(TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_TOKEN);
  }

  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_IF;
  initialize(it, end);
}

void
StatementOfIf::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);
  ++it;
  addEachInvalidToken(it, end);

  IS_EQUAL_RETURN(it, end);

  if (StatementOfParensArguments::isValid(it, end) == false)
  {
    return;
  }

  Statement& current = getCurrentStatement();
  StatementOfParensArguments(current, it, end);

  IS_EQUAL_RETURN(it, end);
  addEachInvalidToken(it, end);

 // ++it;
  IS_EQUAL_RETURN(it, end);

  parseScope(it, end);
  IS_EQUAL_RETURN(it, end);

  //TODO
  if (it->name_.compare(SEMICOLON_TOKEN_NAME) == 0)
  {
    ++it;
  }

  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it,
    end,
    IsValidTokenForStatement());

  IS_EQUAL_RETURN(itMatched, end);

  if (isElse(itMatched))
  {
    addEachInvalidToken(it, end);

    StatementOfElse(add(), it, end);
  }
}

bool
StatementOfIf::isElse(Tokens::TokenSequence::const_iterator& it)
{
  const Token& token = *it;

  return token.name_.compare(ELSE_TOKEN_NAME) == 0;
}

const Statement&
StatementOfIf::getArgumentStatementFromConditionalSentence()
{
  if (getCurrentStatement().statementSequence_.size() == 0)
  {
    throw StatementsError(WITHOUT_CONDITIONAL_ARGUMENTS);
  }

  return getCurrentStatement().statementSequence_[0];
}

const Statement&
StatementOfIf::getStatementIfScope()
{
  if (getCurrentStatement().statementSequence_.size() < 2)
  {
    throw StatementsError(WITHOUT_STATEMENT_IF_SCOPE);
  }

  return getCurrentStatement().statementSequence_[1];
}

const Statement&
StatementOfIf::getStatementElse()
{
  if (getCurrentStatement().statementSequence_.size() < 3)
  {
    throw StatementsError(WITHOUT_STATEMENT_ELSE_SCOPE);
  }

  return getCurrentStatement().statementSequence_[2];
}

bool StatementOfIf::isValid(Tokens::TokenSequence::const_iterator it,
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

  return itMatched->name_.compare(LEFTPAREN_TOKEN_NAME) == 0;
}

bool
StatementOfIf::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  StatementOfIf builder(statement.add(), it, end);

  return true;
}

} // Vera namespace
} // Structures namespace
