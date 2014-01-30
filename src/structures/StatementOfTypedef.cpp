//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfTypedef.h"
#include "IsTokenWithName.h"
#include "StatementOfStruct.h"
#include "StatementOfEnum.h"
#include "StatementOfUnion.h"
#include "StatementOfClass.h"
#include "StatementOfTemplateParameters.h"
#include "StatementOfParensArguments.h"

#include <vector>
#include <map>
#include <algorithm>

#define TOKEN_NAME             "typedef"
#define STRUCT_TOKEN_NAME      "struct"
#define UNION_TOKEN_NAME       "union"
#define LESS_TOKEN_NAME       "less"
#define GREATER_TOKEN_NAME    "greater"
#define IDENTIFIER_TOKEN_NAME  "identifier"
#define SEMICOLON_TOKEN_NAME  "semicolon"
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

StatementOfTypedef::StatementOfTypedef(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, types_(NULL)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_TYPEDEF;
  initialize(it, end);
}

void
StatementOfTypedef::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  push(*it);
  ++it;
  IS_EQUAL_RETURN(it, end);

  addEachInvalidToken(it, end);
}

bool
StatementOfTypedef::isSignature(Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  Tokens::TokenSequence::const_iterator semicolonMatched = std::find_if(it,
    end,
    IsTokenWithName(SEMICOLON_TOKEN_NAME));

  if (semicolonMatched == end)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator leftParenMatched = std::find_if(it,
    semicolonMatched,
    IsTokenWithName(LEFTPAREN_TOKEN_NAME));

  if (leftParenMatched >= semicolonMatched)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator rightParenMatched = std::find_if(leftParenMatched+1,
    semicolonMatched,
    EndsWithCorrectPattern(LEFTPAREN_TOKEN_NAME, RIGHTPAREN_TOKEN_NAME));

  if (rightParenMatched == semicolonMatched)
  {
    return false;
  }

  leftParenMatched = std::find_if(rightParenMatched+1,
      semicolonMatched,
      IsValidTokenForStatement());

   if (IsTokenWithName(LEFTPAREN_TOKEN_NAME)(*leftParenMatched) == false)
   {
     return false;
   }

  rightParenMatched = std::find_if(leftParenMatched+1,
      semicolonMatched,
      EndsWithCorrectPattern(LEFTPAREN_TOKEN_NAME, RIGHTPAREN_TOKEN_NAME));

  if (rightParenMatched == semicolonMatched)
  {
    return false;
  }

  it = std::find_if(rightParenMatched+1,
      end,
      IsValidTokenForStatement());

  return it == semicolonMatched;
}

void
StatementOfTypedef::parseSignature(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Tokens::TokenSequence::const_iterator semicolonMatched = std::find_if(it,
    end,
    IsTokenWithName(SEMICOLON_TOKEN_NAME));

  addEachInvalidToken(it, semicolonMatched);

  Tokens::TokenSequence::const_iterator leftParenMatched = std::find_if(it,
    semicolonMatched,
    IsTokenWithName(LEFTPAREN_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator rightParenMatched = std::find_if(leftParenMatched+1,
    semicolonMatched,
    EndsWithCorrectPattern(LEFTPAREN_TOKEN_NAME, RIGHTPAREN_TOKEN_NAME));

  {
    Statement& current = add();
    StatementsBuilder  branch(current);
    branch.parse(it , leftParenMatched);  //return Type
  }

  {
    Statement& current = getCurrentStatement();
    StatementOfParensArguments branch(current, it, semicolonMatched);
  }

  addEachInvalidToken(it, semicolonMatched);

  {
    parseScope(it, semicolonMatched);
  }

  push(*it);
}

bool
StatementOfTypedef::isValid(Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  return (it->name_.compare(TOKEN_NAME) == 0);
}

bool
StatementOfTypedef::parseListFromScopeToSemicolon(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (StatementsBuilder::parseVariablesFromScopeToSemicolon(it, end))
  {
    types_ = &getCurrentStatement().statementSequence_.back();
    successful = true;
  }

  return successful;
}

enum OperationState
{
 OPERATION_STATE_UNKNOWN,
 OPERATION_STATE_INVALID,
 OPERATION_STATE_FAILED,
 OPERATION_STATE_SUCCESSFUL
};

class BuilderImpl
{
  public:

    BuilderImpl(StatementOfTypedef& root)
    : root_(root)
    {
    }

    template<typename Builder>
    OperationState parse(Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end)
    {
      bool hasName = false;

      if (Builder::isValidWithName(it, end) == true)
      {
        hasName = true;
      }
      else if (Builder::isValidWithoutName(it, end) == false)
      {
        return OPERATION_STATE_INVALID;
      }
      else
      {
        return OPERATION_STATE_INVALID;
      }

      Builder branch(root_.add(), it, end);

      if (hasName)
      {
        branch.parseName(it, end);
      }

      if (branch.parseScope(it, end) == false)
      {
        return OPERATION_STATE_FAILED;
      }

      root_.addEachInvalidToken(it, end);

      if (root_.parseListFromScopeToSemicolon(it, end) == false)
      {
        return OPERATION_STATE_FAILED;
      }

      return OPERATION_STATE_SUCCESSFUL;
    }

  private:

    StatementOfTypedef& root_;
};

bool
StatementOfTypedef::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  if (isValid(it, end) == false)
  {
    return false;
  }

  Statement& current = StatementsBuilder(statement).add();
  StatementOfTypedef  root(current, it, end);

  OperationState status = OPERATION_STATE_UNKNOWN;

  status = BuilderImpl(root).parse<StatementOfUnion>(it, end);

  if (status != OPERATION_STATE_INVALID)
  {
    return status == OPERATION_STATE_SUCCESSFUL;
  }

  status = BuilderImpl(root).parse<StatementOfStruct>(it, end);

  if (status != OPERATION_STATE_INVALID)
  {
    return status == OPERATION_STATE_SUCCESSFUL;
  }

  status = BuilderImpl(root).parse<StatementOfEnum>(it, end);

  if (status != OPERATION_STATE_INVALID)
  {
    return status == OPERATION_STATE_SUCCESSFUL;
  }

  if (root.isSignature(it, end) == true)
  {
    root.parseSignature(it, end);
  }

  return true;
}

} // Vera namespace
} // Structures namespace
