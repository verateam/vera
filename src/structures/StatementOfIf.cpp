//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfIf.h"
#include "IsTokenWithName.h"


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
#define IS_EQUAL_RETURN(left, right) \
  {\
    if (left == right) \
    { \
      return;\
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


  initialize(it, end);
}

void
StatementOfIf::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Tokens::TokenSequence::const_iterator itMatched = end;
  Statement& current = getCurrentStatement();

  current.push(*it);
  ++it;

  if (parseArguments(it, end) == false)
  {
    return;
  }

  ++it;
  IS_EQUAL_RETURN(it, end);

  parseScope(it, end);
  IS_EQUAL_RETURN(it, end);


  IS_EQUAL_RETURN(it, end);

  itMatched = std::find_if(it+1, end, IsValidTokenForStatement());

  IS_EQUAL_RETURN(itMatched, end);

  if (isElse(itMatched))
  {
    ++it;
    addEachInvalidToken(current, it, end);

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
  return true;
}

bool
StatementOfIf::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end) == true)
  {
    StatementOfIf builder(statement/*StatementsBuilder(statement).add()*/, it, end);
    successful = true;
  }

  return successful;
}

} // Vera namespace
} // Structures namespace
