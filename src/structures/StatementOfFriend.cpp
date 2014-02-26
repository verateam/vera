//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfFriend.h"
#include "IsTokenWithName.h"
#include "Document.h"

#include <vector>
#include <map>
#include <algorithm>

#define TOKEN_NAME             "friend"

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

#define IS_FAIL_RETURN_FALSE(conditional) \
  {\
    if (conditional == false) \
    { \
      return false;\
    }\
  }

namespace Vera
{
namespace Structures
{

StatementOfFriend::StatementOfFriend(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_FRIEND;
  initialize(it, end);
}

void
StatementOfFriend::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);
  ++it;
  IS_EQUAL_RETURN(it, end);

  addEachInvalidToken(it, end);

  parse(it, end);
}

bool
StatementOfFriend::isValid(Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  return (it->name_.compare(TOKEN_NAME) == 0);
}

bool
StatementOfFriend::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = statement.add();

  StatementOfFriend  builder(current, it, end);

  return true;
}

} // Vera namespace
} // Structures namespace
