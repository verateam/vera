//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfPreprocessorDirectives.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define PP_ELSE_TOKEN_NAME "pp_else"
#define PP_ENDIF_TOKEN_NAME "pp_endif"
#define PP_ERROR_TOKEN_NAME "pp_error"
#define PP_IFDEF_TOKEN_NAME  "pp_ifdef"
#define PP_IF_TOKEN_NAME  "pp_if"
#define PP_IFNDEF_TOKEN_NAME "pp_ifndef"
#define PP_WARNING_TOKEN_NAME "pp_warning"
#define PP_UNDEF_TOKEN_NAME "pp_undef"
#define PP_PRAGMA_TOKEN_NAME "pp_pragma"
#define PP_ELIF_TOKEN_NAME  "pp_elif"
#define INTLIT_TOKEN_NAME  "intlit"
#define STRINGLIT_TOKEN_NAME  "stringlit"
#define NEWLINE_TOKEN_NAME  "newline"
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

StatementOfPreprocessorDirectives::StatementOfPreprocessorDirectives(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_PREPROCESSORLINE;
  initialize(it, end);
}

void
StatementOfPreprocessorDirectives::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Tokens::TokenSequence::const_iterator endMatched = std::find_if(it, end, IsTokenWithName(NEWLINE_TOKEN_NAME));
  push(*it);
  ++it;
  parse(it, endMatched);
}

bool
StatementOfPreprocessorDirectives::isValid(
    Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  return it->name_.compare(PP_ELSE_TOKEN_NAME) == 0 ||
      it->name_.compare(PP_ELIF_TOKEN_NAME) == 0 ||
      it->name_.compare(PP_ELSE_TOKEN_NAME) == 0 ||
      it->name_.compare(PP_ERROR_TOKEN_NAME) == 0 ||
      it->name_.compare(PP_WARNING_TOKEN_NAME) == 0 ||
      it->name_.compare(PP_PRAGMA_TOKEN_NAME) == 0 ||
      it->name_.compare(PP_IFDEF_TOKEN_NAME) == 0 ||
      it->name_.compare(PP_IFNDEF_TOKEN_NAME) == 0 ||
      it->name_.compare(PP_UNDEF_TOKEN_NAME) == 0 ||
      it->name_.compare(PP_IF_TOKEN_NAME) == 0 ||
      it->name_.compare(PP_ENDIF_TOKEN_NAME) == 0 ;
}

bool
StatementOfPreprocessorDirectives::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  StatementOfPreprocessorDirectives builder(statement.add(), it, end);

  return true;
}

bool
StatementOfPreprocessorDirectives::requiredContinue()
{
  return true;
}

} // Vera namespace
} // Structures namespace
