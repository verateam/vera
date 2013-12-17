//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Statements.h"
//#include "SourceLines.h"
#include "StatementOfIf.h"
#include <functional>
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpplexer_exceptions.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <cctype>


namespace Vera
{
namespace Structures
{

using namespace underlyImpl;

namespace //unname
{

/**
 * @brief Gets the enumerator id associated to the token.
 * @param token The reference to the token instance.
 * @return The id assigned to the token.
 */
static boost::wave::token_id
getTokenId(const Token& token);

/**
 *  @brief Binary function object class whose call returns whether a item is equal
 *  to the given token.
 */
class IsTokenEqual
: public std::unary_function<const Token, bool>
{
  public:

    /**
     * @brief Initializes a new instance of the IsTokenEqual class.
     * @param token The token desired of the collection.
     */
    IsTokenEqual(Token token)
    : token_(token)
    {
    }

    /**
     * @brief Member function that returns true if the element is equal to the given token.
     *
     * @param token The token to be compare
     *
     * @return True if the item is equal to the given token, otherwise, false.
     */
    result_type operator()(argument_type item) const
    {
      return token_ == item;
    }

  private:

    Token token_;

};


/**
 *  @brief Binary function object class whose call returns whether a item with id given.
 */
class IsTokenWithId
: public std::unary_function<const Token, bool>
{
  public:

    /**
     * @brief Initializes a new instance of the IsTokenEqual class.
     * @param id The lexer id desired.
     */
    IsTokenWithId(boost::wave::token_id id)
    : id_(id)
    {
    }

    /**
     * @brief Member function that returns true if the element is equal to the given token.
     *
     * @param token The token to be compare
     *
     * @return True if the item is equal to the given token, otherwise, false.
     */
    result_type operator()(argument_type item) const
    {
      return getTokenId(item) == id_;
    }

  private:

    boost::wave::token_id id_;

};


#define returnIdWheterTokenMatch(TOKEN, NAME) \
{ \
  std::string tokenName = #TOKEN; \
  boost::algorithm::to_lower(tokenName);\
   \
  if (tokenName.compare(NAME) == 0) \
  { \
    return boost::wave::T_##TOKEN; \
  }\
}

static boost::wave::token_id
getTokenId(const Token& token)
{
  std::string name = token.name_;

  returnIdWheterTokenMatch(ALIGNAS, name)
  returnIdWheterTokenMatch(ALIGNOF, name)
  returnIdWheterTokenMatch(AND, name)
  returnIdWheterTokenMatch(AND_ALT, name)
  returnIdWheterTokenMatch(ANDAND, name)
  returnIdWheterTokenMatch(ANDAND_ALT, name)
  returnIdWheterTokenMatch(ANDASSIGN, name)
  returnIdWheterTokenMatch(ANDASSIGN_ALT, name)
  returnIdWheterTokenMatch(ANY, name)
  returnIdWheterTokenMatch(ANY_TRIGRAPH, name)
  returnIdWheterTokenMatch(ARROW, name)
  returnIdWheterTokenMatch(ARROWSTAR, name)
  returnIdWheterTokenMatch(ASM, name)
  returnIdWheterTokenMatch(ASSIGN, name)
  returnIdWheterTokenMatch(AUTO, name)
  returnIdWheterTokenMatch(BOOL, name)
  returnIdWheterTokenMatch(BREAK, name)
  returnIdWheterTokenMatch(CASE, name)
  returnIdWheterTokenMatch(CATCH, name)
  returnIdWheterTokenMatch(CCOMMENT, name)
  returnIdWheterTokenMatch(CHAR, name)
  returnIdWheterTokenMatch(CHAR16_T, name)
  returnIdWheterTokenMatch(CHAR32_T, name)
  returnIdWheterTokenMatch(CHARLIT, name)
  returnIdWheterTokenMatch(CLASS, name)
  returnIdWheterTokenMatch(COLON, name)
  returnIdWheterTokenMatch(COLON_COLON, name)
  returnIdWheterTokenMatch(COMMA, name)
  returnIdWheterTokenMatch(COMPL, name)
  returnIdWheterTokenMatch(COMPL_ALT, name)
  returnIdWheterTokenMatch(COMPL_TRIGRAPH, name)
  returnIdWheterTokenMatch(CONST, name)
  returnIdWheterTokenMatch(CONSTCAST, name)
  returnIdWheterTokenMatch(CONSTEXPR, name)
  returnIdWheterTokenMatch(CONTINUE, name)
  returnIdWheterTokenMatch(CONTLINE, name)
  returnIdWheterTokenMatch(CPPCOMMENT, name)
  returnIdWheterTokenMatch(DECIMALINT, name)
  returnIdWheterTokenMatch(DECLTYPE, name)
  returnIdWheterTokenMatch(DEFAULT, name)
  returnIdWheterTokenMatch(DELETE, name)
  returnIdWheterTokenMatch(DIVIDE, name)
  returnIdWheterTokenMatch(DIVIDEASSIGN, name)
  returnIdWheterTokenMatch(DO, name)
  returnIdWheterTokenMatch(DOT, name)
  returnIdWheterTokenMatch(DOTSTAR, name)
  returnIdWheterTokenMatch(DOUBLE, name)
  returnIdWheterTokenMatch(DYNAMICCAST, name)
  returnIdWheterTokenMatch(ELLIPSIS, name)
  returnIdWheterTokenMatch(ELSE, name)
  returnIdWheterTokenMatch(ENUM, name)
  returnIdWheterTokenMatch(EOF, name)
  returnIdWheterTokenMatch(EOI, name)
  returnIdWheterTokenMatch(EQUAL, name)
  returnIdWheterTokenMatch(EXPLICIT, name)
  returnIdWheterTokenMatch(EXPORT, name)
  returnIdWheterTokenMatch(EXTERN, name)
  returnIdWheterTokenMatch(FALSE, name)
  returnIdWheterTokenMatch(FIRST_TOKEN, name)
  returnIdWheterTokenMatch(FIXEDPOINTLIT, name)
  returnIdWheterTokenMatch(FLOAT, name)
  returnIdWheterTokenMatch(FLOATLIT, name)
  returnIdWheterTokenMatch(FOR, name)
  returnIdWheterTokenMatch(FRIEND, name)
  returnIdWheterTokenMatch(GENERATEDNEWLINE, name)
  returnIdWheterTokenMatch(GOTO, name)
  returnIdWheterTokenMatch(GREATER, name)
  returnIdWheterTokenMatch(GREATEREQUAL, name)
  returnIdWheterTokenMatch(HEXAINT, name)
  returnIdWheterTokenMatch(IDENTIFIER, name)
  returnIdWheterTokenMatch(IF, name)
  returnIdWheterTokenMatch(IMPORT, name)
  returnIdWheterTokenMatch(INLINE, name)
  returnIdWheterTokenMatch(INT, name)
  returnIdWheterTokenMatch(INTLIT, name)
  returnIdWheterTokenMatch(LEFTBRACE, name)
  returnIdWheterTokenMatch(LEFTBRACE_ALT, name)
  returnIdWheterTokenMatch(LEFTBRACE_TRIGRAPH, name)
  returnIdWheterTokenMatch(LEFTBRACKET, name)
  returnIdWheterTokenMatch(LEFTBRACKET_ALT, name)
  returnIdWheterTokenMatch(LEFTBRACKET_TRIGRAPH, name)
  returnIdWheterTokenMatch(LEFTPAREN, name)
  returnIdWheterTokenMatch(LESS, name)
  returnIdWheterTokenMatch(LESSEQUAL, name)
  returnIdWheterTokenMatch(LONG, name)
  returnIdWheterTokenMatch(LONGINTLIT, name)
  returnIdWheterTokenMatch(MINUS, name)
  returnIdWheterTokenMatch(MINUSASSIGN, name)
  returnIdWheterTokenMatch(MINUSMINUS, name)
  returnIdWheterTokenMatch(MSEXT_ASM, name)
  returnIdWheterTokenMatch(MSEXT_BASED, name)
  returnIdWheterTokenMatch(MSEXT_CDECL, name)
  returnIdWheterTokenMatch(MSEXT_DECLSPEC, name)
  returnIdWheterTokenMatch(MSEXT_EXCEPT, name)
  returnIdWheterTokenMatch(MSEXT_FASTCALL, name)
  returnIdWheterTokenMatch(MSEXT_FINALLY, name)
  returnIdWheterTokenMatch(MSEXT_INLINE, name)
  returnIdWheterTokenMatch(MSEXT_INT16, name)
  returnIdWheterTokenMatch(MSEXT_INT32, name)
  returnIdWheterTokenMatch(MSEXT_INT64, name)
  returnIdWheterTokenMatch(MSEXT_INT8, name)
  returnIdWheterTokenMatch(MSEXT_LEAVE, name)
  returnIdWheterTokenMatch(MSEXT_PP_ENDREGION, name)
  returnIdWheterTokenMatch(MSEXT_PP_REGION, name)
  returnIdWheterTokenMatch(MSEXT_STDCALL, name)
  returnIdWheterTokenMatch(MSEXT_TRY, name)
  returnIdWheterTokenMatch(MUTABLE, name)
  returnIdWheterTokenMatch(NAMESPACE, name)
  returnIdWheterTokenMatch(NEW, name)
  returnIdWheterTokenMatch(NEWLINE, name)
  returnIdWheterTokenMatch(NOEXCEPT, name)
  returnIdWheterTokenMatch(NOT, name)
  returnIdWheterTokenMatch(NOT_ALT, name)
  returnIdWheterTokenMatch(NOTEQUAL, name)
  returnIdWheterTokenMatch(NOTEQUAL_ALT, name)
  returnIdWheterTokenMatch(NULLPTR, name)
  returnIdWheterTokenMatch(OCTALINT, name)
  returnIdWheterTokenMatch(OPERATOR, name)
  returnIdWheterTokenMatch(OR, name)
  returnIdWheterTokenMatch(OR_ALT, name)
  returnIdWheterTokenMatch(ORASSIGN, name)
  returnIdWheterTokenMatch(ORASSIGN_ALT, name)
  returnIdWheterTokenMatch(ORASSIGN_TRIGRAPH, name)
  returnIdWheterTokenMatch(OROR, name)
  returnIdWheterTokenMatch(OROR_ALT, name)
  returnIdWheterTokenMatch(OROR_TRIGRAPH, name)
  returnIdWheterTokenMatch(OR_TRIGRAPH, name)
  returnIdWheterTokenMatch(PERCENT, name)
  returnIdWheterTokenMatch(PERCENTASSIGN, name)
  returnIdWheterTokenMatch(PLUS, name)
  returnIdWheterTokenMatch(PLUSASSIGN, name)
  returnIdWheterTokenMatch(PLUSPLUS, name)
  returnIdWheterTokenMatch(POUND, name)
  returnIdWheterTokenMatch(POUND_ALT, name)
  returnIdWheterTokenMatch(POUND_POUND, name)
  returnIdWheterTokenMatch(POUND_POUND_ALT, name)
  returnIdWheterTokenMatch(POUND_POUND_TRIGRAPH, name)
  returnIdWheterTokenMatch(POUND_TRIGRAPH, name)
  returnIdWheterTokenMatch(PP_DEFINE, name)
  returnIdWheterTokenMatch(PP_ELIF, name)
  returnIdWheterTokenMatch(PP_ELSE, name)
  returnIdWheterTokenMatch(PP_ENDIF, name)
  returnIdWheterTokenMatch(PP_ERROR, name)
  returnIdWheterTokenMatch(PP_HHEADER, name)
  returnIdWheterTokenMatch(PP_HHEADER_NEXT, name)
  returnIdWheterTokenMatch(PP_IF, name)
  returnIdWheterTokenMatch(PP_IFDEF, name)
  returnIdWheterTokenMatch(PP_IFNDEF, name)
  returnIdWheterTokenMatch(PP_INCLUDE, name)
  returnIdWheterTokenMatch(PP_INCLUDE_NEXT, name)
  returnIdWheterTokenMatch(PP_LINE, name)
  returnIdWheterTokenMatch(PP_NUMBER, name)
  returnIdWheterTokenMatch(PP_PRAGMA, name)
  returnIdWheterTokenMatch(PP_QHEADER, name)
  returnIdWheterTokenMatch(PP_QHEADER_NEXT, name)
  returnIdWheterTokenMatch(PP_UNDEF, name)
  returnIdWheterTokenMatch(PP_WARNING, name)
  returnIdWheterTokenMatch(PRIVATE, name)
  returnIdWheterTokenMatch(PROTECTED, name)
  returnIdWheterTokenMatch(PUBLIC, name)
  returnIdWheterTokenMatch(QUESTION_MARK, name)
  returnIdWheterTokenMatch(RAWSTRINGLIT, name)
  returnIdWheterTokenMatch(REGISTER, name)
  returnIdWheterTokenMatch(REINTERPRETCAST, name)
  returnIdWheterTokenMatch(RETURN, name)
  returnIdWheterTokenMatch(RIGHTBRACE, name)
  returnIdWheterTokenMatch(RIGHTBRACE_ALT, name)
  returnIdWheterTokenMatch(RIGHTBRACE_TRIGRAPH, name)
  returnIdWheterTokenMatch(RIGHTBRACKET, name)
  returnIdWheterTokenMatch(RIGHTBRACKET_ALT, name)
  returnIdWheterTokenMatch(RIGHTBRACKET_TRIGRAPH, name)
  returnIdWheterTokenMatch(RIGHTPAREN, name)
  returnIdWheterTokenMatch(SEMICOLON, name)
  returnIdWheterTokenMatch(SHIFTLEFT, name)
  returnIdWheterTokenMatch(SHIFTLEFTASSIGN, name)
  returnIdWheterTokenMatch(SHIFTRIGHT, name)
  returnIdWheterTokenMatch(SHIFTRIGHTASSIGN, name)
  returnIdWheterTokenMatch(SHORT, name)
  returnIdWheterTokenMatch(SIGNED, name)
  returnIdWheterTokenMatch(SIZEOF, name)
  returnIdWheterTokenMatch(SPACE, name)
  returnIdWheterTokenMatch(SPACE2, name)
  returnIdWheterTokenMatch(STAR, name)
  returnIdWheterTokenMatch(STARASSIGN, name)
  returnIdWheterTokenMatch(STATIC, name)
  returnIdWheterTokenMatch(STATICASSERT, name)
  returnIdWheterTokenMatch(STATICCAST, name)
  returnIdWheterTokenMatch(STRINGLIT, name)
  returnIdWheterTokenMatch(STRUCT, name)
  returnIdWheterTokenMatch(SWITCH, name)
  returnIdWheterTokenMatch(TEMPLATE, name)
  returnIdWheterTokenMatch(THIS, name)
  returnIdWheterTokenMatch(THREADLOCAL, name)
  returnIdWheterTokenMatch(THROW, name)
  returnIdWheterTokenMatch(TRUE, name)
  returnIdWheterTokenMatch(TRY, name)
  returnIdWheterTokenMatch(TYPEDEF, name)
  returnIdWheterTokenMatch(TYPEID, name)
  returnIdWheterTokenMatch(TYPENAME, name)
  returnIdWheterTokenMatch(UNION, name)
  returnIdWheterTokenMatch(UNKNOWN, name)
  returnIdWheterTokenMatch(UNSIGNED, name)
  returnIdWheterTokenMatch(USING, name)
  returnIdWheterTokenMatch(VIRTUAL, name)
  returnIdWheterTokenMatch(VOID, name)
  returnIdWheterTokenMatch(VOLATILE, name)
  returnIdWheterTokenMatch(WCHART, name)
  returnIdWheterTokenMatch(WHILE, name)
  returnIdWheterTokenMatch(XOR, name)
  returnIdWheterTokenMatch(XOR_ALT, name)
  returnIdWheterTokenMatch(XORASSIGN, name)
  returnIdWheterTokenMatch(XORASSIGN_ALT, name)
  returnIdWheterTokenMatch(XORASSIGN_TRIGRAPH, name)
  returnIdWheterTokenMatch(XOR_TRIGRAPH, name)

  return boost::wave::T_UNKNOWN;
}

/**
 * @brief Parses the statements from the token list
 * and retrieves the related list of tokens.
 *
 * @param response [out] the reference to the statement
 * used to return the response.
 * @param it the reference to the const iterator with the
 * initial element of the statement.
 * @param end the of the list to be analyzed.
 */
void parseStatement(Statement& response,
  TokenSequenceConstIterator& it,
  TokenSequenceConstIterator& end);

/**
 * @brief Parses the statements from the token list
 * and retrieves the related list of tokens.
 * @param response [out] the reference to the statement
 * used to return the response.
 * @param collection token list to be analyzed.
 */
void parseStatement(Statement& response, Tokens::TokenSequence& collection);

/**
 * @brief TODO
 * @param response
 * @param it
 * @param end
 */
void parseStatementDefineType(Statement& response,
  TokenSequenceConstIterator& it,
  TokenSequenceConstIterator& end);

/**
 * @brief TODO
 */
Tokens::TokenSequence
extractTokens(TokenSequenceConstIterator& begin,
    TokenSequenceConstIterator& end, boost::wave::token_id id);

/**
 * @brief Parses recursively the statements from the
 * token list and retrieves the related list of tokens.
 *
 * @param response [out] the reference to the statement
 * used to return the response.
 * @param it the reference to the const iterator with the
 * initial element of the statement.
 * @param end the of the list to be analyzed.
 */
void recursiveParseStatement(Statement& response,
  TokenSequenceConstIterator& it,
  TokenSequenceConstIterator& end)
{
  while (it != end)
  {
    const struct Token& token = *it;
    boost::wave::token_id id = getTokenId(token);

    if (id == boost::wave::T_EOF)
    {
      break;
    }

    if (id == boost::wave::T_IF)
    {
      StatementOfIf ifStatement(response, it, end);
      ++it;
    }

    if (id == boost::wave::T_IF
        || id == boost::wave::T_ELSE
        || id == boost::wave::T_WHILE
        || id == boost::wave::T_DO
        || id == boost::wave::T_EXTERN
        || id == boost::wave::T_FOR
        || id == boost::wave::T_TRY
        || id == boost::wave::T_CATCH)
    {
      response.statementSequence_.push_back(Statement());
      Statement& current = response.statementSequence_.back();
      current.tokenSequence_.push_back(*it);

      ++it;
      parseStatement(current, it, end);
      ++it; //TODO require continue
      continue;
    }

    if (id == boost::wave::T_LEFTPAREN || id == boost::wave::T_LEFTBRACE)
    {
      response.tokenSequence_.push_back(*it);
      ++it;
      recursiveParseStatement(response, it, end);
      //continue;
    }
    else
    {
      response.tokenSequence_.push_back(*it);
    }

    if (id == boost::wave::T_SEMICOLON)
    {
      //TODO break;
    }

    if (id == boost::wave::T_RIGHTPAREN)
    {
      break;
    }

    if (id == boost::wave::T_RIGHTBRACE)
    {
      break;
    }

    if (id == boost::wave::T_PP_DEFINE)
    {
      parseStatementDefineType(response, it, end);
    }

    ++it;
  }
}

void parseStatementDefineType(Statement& response,
  TokenSequenceConstIterator& it,
  TokenSequenceConstIterator& end)
{
  response.tokenSequence_.push_back(*it);
  ++it;

  response.statementSequence_.push_back(Statement());
  Statement& current = response.statementSequence_.back();

  Tokens::TokenSequence subCollection = extractTokens(it, end, boost::wave::T_NEWLINE);

  parseStatement(current, subCollection);
}

Tokens::TokenSequence
extractTokens(TokenSequenceConstIterator& begin,
    TokenSequenceConstIterator& end, boost::wave::token_id id)
{
  Tokens::TokenSequence response;

  TokenSequenceConstIterator itMatched = std::find_if(begin, end, IsTokenWithId(id));

  if (itMatched != end && itMatched != begin)
  {
    std::copy(begin,
      itMatched,
      std::back_inserter<Vera::Structures::Tokens::TokenSequence>(response));
  }

  return response;
}


void parseStatement(Statement& response, Tokens::TokenSequence& collection)
{
  TokenSequenceConstIterator it = collection.begin();
  TokenSequenceConstIterator end = collection.end();


  parseStatement(response, it, end);
}

void parseStatement(Statement& response,
  TokenSequenceConstIterator& it,
  TokenSequenceConstIterator& end)
{
  while (it != end)
  {
    const struct Token& token = *it;

    boost::wave::token_id id = getTokenId(token);

    if (id == boost::wave::T_EOF)
    {
      break;
    }

    if (id == boost::wave::T_IF)
    {
      StatementOfIf ifStatement(response, it, end);
      ++it;
      continue;
    }

    if (id == boost::wave::T_LEFTBRACE || id == boost::wave::T_LEFTPAREN)
    {
      response.statementSequence_.push_back(Statement());
      Statement& current = response.statementSequence_.back();
      current.tokenSequence_.push_back(*it);

      ++it;

      recursiveParseStatement(current, it, end);

      ++it;

      continue;
    }

    if (id == boost::wave::T_LEFTBRACE || id == boost::wave::T_LEFTPAREN)
    {
      break;
    }

    if (id == boost::wave::T_RIGHTPAREN)
    {
      response.tokenSequence_.push_back(*it);
      break;
    }

    if (id == boost::wave::T_SEMICOLON)
    {
      response.tokenSequence_.push_back(*it);
      break;
    }

    if (id == boost::wave::T_PP_DEFINE)
    {
      parseStatementDefineType(response, it, end);
      break;
    }

    response.tokenSequence_.push_back(*it);
    ++it;
  }
}

} //unname namespace

bool
Statement::operator==(const Statement& statement) const
{

  bool isEqual = tokenSequence_.size() == statement.tokenSequence_.size();

  if (isEqual)
  {
    isEqual = std::equal(tokenSequence_.begin(),
      tokenSequence_.end(),
      statement.tokenSequence_.begin());
  }

  if (isEqual && statementSequence_.size() == statement.statementSequence_.size())
  {
    isEqual = std::equal(statementSequence_.begin(),
        statementSequence_.end(),
      statement.statementSequence_.begin());
  }

  return isEqual;
}

Statement
StatementsBuilder::create(Token token, Tokens::TokenSequence& tokenCollection)
{
  Statement response;
  StatementsBuilder concreteBuilder(response);

  Tokens::TokenSequence::const_iterator it = tokenCollection.begin();
  Tokens::TokenSequence::const_iterator end = tokenCollection.end();

  it = std::find_if(it, end, IsTokenEqual(token));

  if (it != end)
  {
    concreteBuilder.builder(response, it, end);
  }

  return response;
}

Statement&
StatementsBuilder::getCurrentStatement()
{
  return statement_;
}

void
StatementsBuilder::setCurrentStatement(Statement& current)
{
  statement_ = current;
}

Statement&
StatementsBuilder::add()
{
  statement_.statementSequence_.push_back(Statement());
  Statement& current = statement_.statementSequence_.back();

  return current;
}

void
StatementsBuilder::builder(Statement& response, Tokens::TokenSequence::const_iterator& it, Tokens::TokenSequence::const_iterator& end)
{
  if (it == end)
  {
    throw StatementsError("Token Collection is empty.");
  }

  parseStatement(response, it, end);
}

StatementsBuilder::StatementsBuilder(Statement& statement)
: statement_(statement)
{
}

}
}
