//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfAssign.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define TOKEN_NAME  "assign"

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

StatementOfAssign::StatementOfAssign(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_ASSIGN;
  initialize(it, end);
}

void
StatementOfAssign::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);
  ++it;

  addEachInvalidToken(it, end);

  parse(it, end);
}

//TODO fail
bool
StatementOfAssign::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  return it->name_.compare(TOKEN_NAME) == 0;
}

bool
StatementOfAssign::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement::StatementSequence::reverse_iterator rit = statement.statementSequence_.rbegin();
  Statement::StatementSequence::reverse_iterator rend = statement.statementSequence_.rend();
  Statement* lastItem = NULL;

  for (;rit != rend; ++rit)
  {
    Statement& item = *rit;
    if (IsValidTokenForStatement()(item.token_) == true)
    {
      lastItem = &item;
      break;
    }
  }

  Statement& current = statement.add();

  current.parentId_ = lastItem->id_;
  StatementOfAssign builder(current, it, end);

  return true;
}

bool
StatementOfAssign::requiredContinue()
{
  return true;
}

} // Vera namespace
} // Structures namespace
