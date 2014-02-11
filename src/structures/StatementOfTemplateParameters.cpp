//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfTemplateParameters.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define TOKEN_NAME  "namespace"
#define LESS_TOKEN_NAME       "less"
#define GREATER_TOKEN_NAME    "greater"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME "rightbrace"
#define SEMICOLON_TOKEN_NAME  "semicolon"
#define COMMA_TOKEN_NAME  "comma"
#define OROR_TOKEN_NAME       "oror"
#define ANDAND_TOKEN_NAME     "andand"
#define EQUAL_TOKEN_NAME      "equal"
#define NOTEQUAL_TOKEN_NAME   "notequal"
#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."

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

StatementOfTemplateParameters::StatementOfTemplateParameters(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_TEMPLATEPARAMETERS;
  initialize(it, end);
}

void
StatementOfTemplateParameters::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{

  Tokens::TokenSequence::const_iterator first;
  Tokens::TokenSequence::const_iterator second;

  Tokens::TokenSequence::const_iterator endMatched = std::find_if(it+1,
    end, EndsWithCorrectPattern(LESS_TOKEN_NAME, GREATER_TOKEN_NAME));

  Statement& current = getCurrentStatement();
  push(*it);
  ++it;
  addEachInvalidToken(it, endMatched);

  while (it < endMatched)
  {
    builder(it, endMatched);

    IS_EQUAL_BREAK(it, endMatched);
    ++it;
    addEachInvalidToken(it, endMatched);
   // partial.push(*it);
  }

  IS_EQUAL_RETURN(it, end);

  push(*it);

  if (it < end)
  {
    ++it;
  }
}

//TODO fail
bool
StatementOfTemplateParameters::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  static std::vector<std::string> invalidTokens;

  invalidTokens.push_back(OROR_TOKEN_NAME);
  invalidTokens.push_back(ANDAND_TOKEN_NAME);
  invalidTokens.push_back(EQUAL_TOKEN_NAME);
  invalidTokens.push_back(NOTEQUAL_TOKEN_NAME);

  if (it->name_.compare(LESS_TOKEN_NAME) != 0)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator endMatched = std::find_if(it,
      end, IsTokenWithName(SEMICOLON_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator greaterMatched = std::find_if(it+1,
      endMatched, EndsWithCorrectPattern(LESS_TOKEN_NAME, GREATER_TOKEN_NAME));

   if (greaterMatched == endMatched)
   {
     return false;
   }

   if (greaterMatched > endMatched)
   {
     return false;
   }

   for (;it < greaterMatched; ++it)
   {
     if (std::find(invalidTokens.begin(), invalidTokens.end(), it->name_) != invalidTokens.end())
     {
       return false;
     }
   }

  return true;
}

bool
StatementOfTemplateParameters::create(Statement& statement,
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
  current.parent_ = lastItem;
  current.parentId_ = lastItem->id_;

  StatementOfTemplateParameters builder(current, it, end);

  return true;
}

bool
StatementOfTemplateParameters::requiredContinue()
{
  return true;
}

} // Vera namespace
} // Structures namespace
