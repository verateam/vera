//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfTemplate.h"
#include "StatementOfTemplateParameters.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>


#define TOKEN_NAME  "template"


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

namespace Vera
{
namespace Structures
{

StatementOfTemplate::StatementOfTemplate(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_TEMPLATE;
  initialize(it, end);
}

void
StatementOfTemplate::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = getCurrentStatement();
  push(*it);

  ++it;

  addEachInvalidToken(it, end);

  StatementOfTemplateParameters::create(current, it, end);

  addEachInvalidToken(it, end);

  builder(current, it, end);
}

bool
StatementOfTemplate::isValid(
    Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  return it->name_.compare(TOKEN_NAME) == 0;
}

bool
StatementOfTemplate::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end) == true)
  {
    StatementOfTemplate builder(statement.add(), it, end);
    successful = true;
  }

  return successful;
}

} // Vera namespace
} // Structures namespace
