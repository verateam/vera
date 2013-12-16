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

#define IS_NOT_TOKEN "the first element of the collection is not a token of type 'if'."
#define TOKEN_NAME  "if"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define ELSE_TOKEN_NAME  "else"
namespace Vera
{
namespace Structures
{

StatementOfIf::StatementOfIf(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder::StatementsBuilder(statement)
, statement_(statement)
{
  const Token& token = *it;

  if (token.name_.compare(TOKEN_NAME) != 0)
  {
    throw StatementsError(IS_NOT_TOKEN);
  }

  setCurrentStatement(add());

  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it,
    end,
    IsTokenWithName(LEFTPAREN_TOKEN_NAME));

  if (itMatched != end)
  {
    --itMatched;
  }

  std::copy(it,
    itMatched,
    std::back_inserter<Tokens::TokenSequence>(getCurrentStatement().tokenSequence_));

  it += std::distance(it, itMatched);

  if (it != end)
  {
    parseArguments(it, end);
    parseScope(it, end);
    itMatched = std::find_if(it, end, IsValidTokenForStatement());

    if (itMatched != end)
    {
      if (isElse(itMatched))
      {
        --itMatched;
        std::copy(it, itMatched, std::back_inserter<Vera::Structures::Tokens::TokenSequence>(getCurrentStatement().tokenSequence_));
        it += std::distance(it, itMatched)+1;

        parseScope(it, end);
      }
    }
  }

}

bool
StatementOfIf::isElse(Tokens::TokenSequence::const_iterator& it)
{
  const Token& token = *it;

  return token.name_.compare(ELSE_TOKEN_NAME) != 0;
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

  ++it;
  current.tokenSequence_.push_back(*it);
  ++it;

  builder(current, it, end);
}

} // Vera namespace
} // Structures namespace
