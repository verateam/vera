//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfNamespace.h"
#include "IsTokenWithName.h"
#include "Document.h"

#include <vector>
#include <map>
#include <algorithm>
#include <sstream>


#define IS_NOT_TOKEN "the first element of the collection is not a token of 'namespace' type."
#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME  "namespace"
#define IDENTIFIER_TOKEN_NAME  "identifier"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME  "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME  "rightbrace"
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

StatementOfNamespace::StatementOfNamespace(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, id_(statement.id_)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_NAMESPACE_UNNAME;
}

bool
StatementOfNamespace::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);
  ++it;

  Statement& current = getCurrentStatement();

  addEachInvalidToken(it, end);

  IS_EQUAL_RETURN_FALSE(it, end);

  if (IsTokenWithName(IDENTIFIER_TOKEN_NAME)(*it) == true )
  {
    push(*it);

    name_ = it->value_;

    current.type_ = Statement::TYPE_ITEM_STATEMENT_OF_NAMESPACE;

    ++it;
    IS_EQUAL_RETURN_FALSE(it, end);
  }
  else
  {
    std::stringstream stream;
    stream << id_ << '_'<<std::flush;
    name_ = stream.str();
  }

  addEachInvalidToken(it, end);
  IS_EQUAL_RETURN_FALSE(it, end);

  return parseScope(it, end);
}

bool
StatementOfNamespace::parseScope(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  IS_EQUAL_RETURN_FALSE(it, end);
  if (IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*it) == false )
  {
    return false;
  }

  Statement& current = add();
  current.type_ = Statement::TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS;
  StatementsBuilder branch(current);

  branch.push(*it);

  it++;

  Tokens::TokenSequence::const_iterator rightBraceMached = std::find_if(it,
    end,
    EndsWithCorrectPattern(LEFTBRACE_TOKEN_NAME, RIGHTBRACE_TOKEN_NAME));

  branch.addEachInvalidToken(it, end);

  while (it < rightBraceMached)
  {
    branch.builder(it, rightBraceMached);

    Statement& lastItem = current.getBack();

    if (isSignature(lastItem))
    {
      lastItem.type_ = Statement::TYPE_ITEM_STATEMENT_OF_SIGNATURE_DECLARATION;
    }

    ++it;
    branch.addEachInvalidToken(it, rightBraceMached);
  }

  if (it >= end)
  {
    return false;
  }

  if (IsTokenWithName(RIGHTBRACE_TOKEN_NAME)(*it) == true)
  {
    successful = true;

    branch.push(*it);
    ++it;
  }

  return successful;
}

bool
StatementOfNamespace::isValid(Tokens::TokenSequence::const_iterator it,
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

  if (IsTokenWithName(LEFTBRACE_TOKEN_NAME)(*itMatched) == true)
  {
    return true;
  }

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
StatementOfNamespace::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  StatementOfNamespace builder(statement.add(), it, end);

  successful = builder.initialize(it, end);

  statement.doc_->addNamespace(builder.name_, builder.id_);

  return successful;
}

} // Vera namespace
} // Structures namespace
