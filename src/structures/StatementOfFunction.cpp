//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "StatementOfParensArguments.h"
#include "StatementOfTemplateParameters.h"
#include "StatementOfFunction.h"
#include "StatementOfOperator.h"
#include "IsTokenWithName.h"
#include "Document.h"

#include <vector>
#include <map>
#include <algorithm>

#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME  "identifier"
#define IDENTIFIER_TOKEN_NAME  "identifier"
#define LESS_TOKEN_NAME  "less"
#define GREATER_TOKEN_NAME  "greater"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME  "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME  "rightbrace"
#define OPERATOR_TOKEN_NAME  "operator"
#define SEMICOLON_TOKEN_NAME  "semicolon"
#define COLON_TOKEN_NAME  "colon"
#define PP_DEFINE_TOKEN_NAME  "pp_define"
#define PP_ELIF_TOKEN_NAME   "pp_elif"
#define PP_ELSE_TOKEN_NAME      "pp_else"
#define PP_ERROR_TOKEN_NAME   "pp_error"
#define PP_HHEADER_TOKEN_NAME   "pp_hheader"
#define PP_HHEADER_NEXT_TOKEN_NAME  "pp_hheader_next"
#define PP_IF_TOKEN_NAME     "pp_if"
#define PP_ERROR_TOKEN_NAME  "pp_error"
#define PP_IFNDEF_TOKEN_NAME    "pp_ifndef"
#define PP_INCLUDE_TOKEN_NAME   "pp_hheader"
#define PP_INCLUDE_NEXT_TOKEN_NAME  "pp_hheader_next"
#define PP_NUMBER_TOKEN_NAME  "pp_number"
#define PP_LINE_TOKEN_NAME  "pp_line"
#define PP_PRAGMA_TOKEN_NAME  "pp_pragma"
#define PP_UNDEF_TOKEN_NAME  "pp_undef"
#define PP_WARNING_TOKEN_NAME  "pp_warning"

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

StatementOfFunction::StatementOfFunction(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, variables_(NULL)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_FUNCTION;
  initialize(it, end);
}

void
StatementOfFunction::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  addEachInvalidToken(it, end);


  Tokens::TokenSequence::const_iterator leftbrace = std::find_if(it,
      end,
      IsTokenWithName(LEFTBRACE_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator rightbrace  = std::find_if(leftbrace+1,
      end,
      EndsWithCorrectPattern(LEFTBRACE_TOKEN_NAME, RIGHTBRACE_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator operatorMatched  = std::find_if(it,
      leftbrace,
      IsTokenWithName(OPERATOR_TOKEN_NAME));

  if (operatorMatched < leftbrace &&
      StatementOfOperator::isValid(operatorMatched, leftbrace)== true)
  {
    parse(it, operatorMatched);
    Statement& current = getCurrentStatement();

    StatementOfOperator::create(current, it, leftbrace);
  }

  Tokens::TokenSequence::const_iterator leftparen  = std::find_if(it,
      leftbrace,
      IsTokenWithName(LEFTPAREN_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator rightparen  = std::find_if(leftparen+1,
      leftbrace,
      EndsWithCorrectPattern(LEFTPAREN_TOKEN_NAME, RIGHTPAREN_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator colon  = std::find_if(rightparen,
      leftbrace,
      IsTokenWithName(COLON_TOKEN_NAME));

  parse(it, leftparen);

  parseArguments(it, end);

  addEachInvalidToken(it, end);

  if (IsTokenWithName(COLON_TOKEN_NAME)(*it) == true )
  {

    StatementsBuilder partial(add());

    if (partial.parseHeritage(it, end))
    {
      Statement& aux = partial.getCurrentStatement();
      aux.type_ = Statement::TYPE_ITEM_STATEMENT_OF_INITIALIZER_LIST;
    }
  }

  addEachInvalidToken(it, end);

  if (it < leftbrace)
  {
    for (;it < leftbrace; ++it)
    {
      push(*it);
    }
  }

  parseScope(it, end);
  (void)it;

}

bool
StatementOfFunction::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  if (IsValidTokenForStatement()(*it) == false)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator leftbrace = std::find_if(it,
      end,
      IsTokenWithName(LEFTBRACE_TOKEN_NAME));

  if (leftbrace == end)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator semicolon = std::find_if(it,
      leftbrace,
      IsTokenWithName(SEMICOLON_TOKEN_NAME));

  if (semicolon < leftbrace)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator operatorMatched = std::find_if(it,
      leftbrace,
      IsTokenWithName(OPERATOR_TOKEN_NAME));

  if (operatorMatched < leftbrace &&
      StatementOfOperator::isValid(operatorMatched, leftbrace)== true)
  {
    it = operatorMatched;
    Statement aux;
    StatementOfOperator::create(aux, it, leftbrace);
  }

  Tokens::TokenSequence::const_iterator leftparen  = std::find_if(it,
      leftbrace,
      IsTokenWithName(LEFTPAREN_TOKEN_NAME));


  if (leftbrace <= leftparen)
    return false;

  Tokens::TokenSequence::const_iterator rightparen  = std::find_if(leftparen+1,
      leftbrace,
      EndsWithCorrectPattern(LEFTPAREN_TOKEN_NAME, RIGHTPAREN_TOKEN_NAME));

  IS_EQUAL_RETURN_FALSE(rightparen, leftbrace)

  Tokens::TokenSequence::const_iterator rightbrace  = std::find_if(leftbrace+1,
      end,
      EndsWithCorrectPattern(LEFTBRACE_TOKEN_NAME, RIGHTBRACE_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator colon  = std::find_if(it,
      leftbrace,
      IsTokenWithName(COLON_TOKEN_NAME));

  for (;it < leftparen; ++it)
  {
    if (IsTokenWithName(PP_DEFINE_TOKEN_NAME)(*it) ||
        IsTokenWithName(PP_ELSE_TOKEN_NAME)(*it) ||
        IsTokenWithName(PP_ERROR_TOKEN_NAME)(*it) ||
        IsTokenWithName(PP_HHEADER_NEXT_TOKEN_NAME)(*it) ||
        IsTokenWithName(PP_HHEADER_TOKEN_NAME)(*it) ||
        IsTokenWithName(PP_IF_TOKEN_NAME)(*it) ||
        IsTokenWithName(PP_IFNDEF_TOKEN_NAME)(*it) ||
        IsTokenWithName(PP_ELIF_TOKEN_NAME)(*it))
    {
      return false;
    }
  }

  if (colon<rightparen)
  {
    return false;
  }

  IS_EQUAL_RETURN_FALSE(rightbrace, end)

  if (IsTokenWithName(RIGHTBRACE_TOKEN_NAME)(*rightbrace) == false)
  {
    return false;
  }

  return true;
}

bool
StatementOfFunction::create(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  statement.doc_->enableFunction();

  StatementOfFunction branch(statement.add(), it, end);

  statement.doc_->disableFunction();

  return true;
}

} // Vera namespace
} // Structures namespace
