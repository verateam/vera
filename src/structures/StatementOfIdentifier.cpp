//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfIdentifier.h"
#include "IsTokenWithName.h"
#include "Document.h"

#include <vector>
#include <map>
#include <algorithm>

#define TOKEN_NAME             "identifier"
#define COLONCOLON_TOKEN_NAME  "colon_colon"
#define COMPL_TOKEN_NAME  "compl"

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

StatementOfIdentifier::StatementOfIdentifier(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  statement.type_ = Statement::TYPE_ITEM_TOKEN;
  initialize(it, end);
}

void
StatementOfIdentifier::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  std::string identifier = "";
  bool hasComplToken = false;

  std::size_t line = it->line_;
  std::size_t column = it->column_;

  for (;it < end; ++it)
  {
    if (it->name_.compare(COMPL_TOKEN_NAME) == 0)
    {
      hasComplToken = true;
      continue;
    }

    if (it->name_.compare(TOKEN_NAME) == 0 ||
        it->name_.compare(COLONCOLON_TOKEN_NAME) == 0)
    {
      if (hasComplToken)
      {
        identifier.append("~");
      }

      identifier.append(it->value_);
      push(*it);
      continue;
    }

    break;
  }

  Statement& current = getCurrentStatement();

  Token& token = current.token_;
  token.name_ = "identifier";
  token.value_ = identifier;

  token.line_ = line;
  token.column_ = column;
}

bool
StatementOfIdentifier::isValid(Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{

  if (it->name_.compare(TOKEN_NAME) != 0)
  {
    return false;
  }

  ++it;

  if (it->name_.compare(COLONCOLON_TOKEN_NAME) != 0)
  {
    return false;
  }

  return true;
}

bool
StatementOfIdentifier::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = statement.add();

  StatementOfIdentifier  builder(current, it, end);

  return true;
}

} // Vera namespace
} // Structures namespace
