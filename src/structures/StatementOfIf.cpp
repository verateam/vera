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

  setCurrentStatement(add());

  initialize(it, end);
}

void
StatementOfIf::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it + 1,
    end,
    IsValidTokenForStatement());

  IS_EQUAL_RETURN(itMatched, end);

  if (IsTokenWithName(LEFTPAREN_TOKEN_NAME)(*itMatched) == false)
  {
    return;
  }

  std::copy(it,
    itMatched,
    std::back_inserter<Tokens::TokenSequence>(getCurrentStatement().tokenSequence_));

  it += std::distance(it, itMatched)-1;

  IS_EQUAL_RETURN(it, end);

  parseArguments(it, end);

  ++it;
  IS_EQUAL_RETURN(it, end);

  parseScope(it, end);

  IS_EQUAL_RETURN(it, end);

  itMatched = std::find_if(it, end, IsValidTokenForStatement());

  IS_EQUAL_RETURN(itMatched, end);

  if (isElse(itMatched))
  {
    addEachInvalidToken(it,end, getCurrentStatement().tokenSequence_);

    getCurrentStatement().tokenSequence_.push_back(*it);
    ++it;

    parseScope(it, end);
  }
}

bool
StatementOfIf::isElse(Tokens::TokenSequence::const_iterator& it)
{
  const Token& token = *it;

  return token.name_.compare(ELSE_TOKEN_NAME) == 0;
}

void
StatementOfIf::parseArguments(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = add();

  ++it;
  current.tokenSequence_.push_back(*it);
  ++it;

  builder(current, it, end);
}

void
StatementOfIf::parseScope(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = add();

  addEachInvalidToken(it, end, current.tokenSequence_);

  if (IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*it) == true)
  {
    current.tokenSequence_.push_back(*it);

    for (++it; it != end; ++it)
    {

      addEachInvalidToken(it, end, current.tokenSequence_);

      if (it == end)
      {
        break;
      }

      if (IsTokenWithName(RIGHTBRACE_TOKEN_NAME)(*it) == true)
      {
        current.tokenSequence_.push_back(*it);
        ++it;
        break;
      }

      builder(current, it, end);

      if (it == end)
      {
        break;
      }
    }
  }
  else
  {
    builder(current, it, end);
  }
}

void
StatementOfIf::addEachInvalidToken(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end,
  Tokens::TokenSequence& current)
{
  IsValidTokenForStatement isValid(false);

  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it,
    end,
    IsValidTokenForStatement(false));

  std::copy(it, itMatched, std::back_inserter<Vera::Structures::Tokens::TokenSequence>(current));
  it += std::distance(it, itMatched);
}

const Tokens::TokenSequence&
StatementOfIf::getIfTokens()
{
  return getCurrentStatement().tokenSequence_;
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
StatementOfIf::getStatementElseScope()
{
  if (getCurrentStatement().statementSequence_.size() < 3)
  {
    throw StatementsError(WITHOUT_STATEMENT_ELSE_SCOPE);
  }

  return getCurrentStatement().statementSequence_[2];
}

} // Vera namespace
} // Structures namespace
