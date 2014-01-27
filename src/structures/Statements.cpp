//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Statements.h"
#include "StatementOfIf.h"
#include "StatementOfElse.h"
#include "StatementOfForLoop.h"
#include "StatementOfWhileLoop.h"
#include "StatementOfTryCatches.h"
#include "StatementOfDoWhileLoop.h"
#include "StatementOfSwitch.h"
#include "StatementOfCase.h"
#include "StatementOfCatch.h"
#include "StatementOfNamespace.h"
#include "StatementOfStruct.h"
#include "StatementOfAccessModifiers.h"
#include "StatementOfDefault.h"
#include "StatementOfOperatorTernario.h"
#include "StatementOfExtern.h"
#include "StatementOfTemplateParameters.h"
#include "StatementOfEnum.h"
#include "StatementOfParensArguments.h"
#include "StatementOfBracketsArguments.h"
#include "StatementOfBracesArguments.h"
#include "StatementOfUnion.h"
#include "StatementOfPreprocessorLine.h"
#include "StatementOfDefine.h"
#include "StatementOfClass.h"
#include "StatementOfTypedef.h"

#include "IsTokenWithName.h"

#include <functional>
#include <boost/algorithm/string/case_conv.hpp>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <cctype>

#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME  "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME  "rightbrace"
#define SEMICOLON_TOKEN_NAME  "semicolon"

#define IS_EQUAL_BREAK(left, right) \
  {\
    if (left == right) \
    { \
      break;\
    }\
  }
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

namespace
{
/**
 * @brief Selects the best strategy for the given token.
 */
class StrategySelector
{
  public:
    /**
     * @brief Factory method type.
     */
    typedef bool (*FactoryMethod)(Vera::Structures::Statement& statement,
        Vera::Structures::Tokens::TokenSequence::const_iterator& it,
        Vera::Structures::Tokens::TokenSequence::const_iterator& end);

  public:
    /**
     * @brief Initializes all the supported strategies.
     */
    StrategySelector()
    {
      factories_[boost::wave::T_IF] = &Vera::Structures::StatementOfIf::create;
      factories_[boost::wave::T_FOR] = &Vera::Structures::StatementOfForLoop::create;
      factories_[boost::wave::T_TRY] = &Vera::Structures::StatementOfTryCatches::create;
      factories_[boost::wave::T_DO] = &Vera::Structures::StatementOfDoWhileLoop::create;
      factories_[boost::wave::T_CATCH]= &Vera::Structures::StatementOfCatch::create;
      factories_[boost::wave::T_ELSE] = &Vera::Structures::StatementOfElse::create;
      factories_[boost::wave::T_CASE] = &Vera::Structures::StatementOfCase::create;
      factories_[boost::wave::T_DEFAULT] = &Vera::Structures::StatementOfDefault::create;
      factories_[boost::wave::T_SWITCH] = &Vera::Structures::StatementOfSwitch::create;
      factories_[boost::wave::T_WHILE] = &Vera::Structures::StatementOfWhileLoop::create;
      factories_[boost::wave::T_NAMESPACE] = &Vera::Structures::StatementOfNamespace::create;
      factories_[boost::wave::T_STRUCT] = &Vera::Structures::StatementOfStruct::create;
      factories_[boost::wave::T_PUBLIC] = &Vera::Structures::StatementOfAccessModifiers::create;
      factories_[boost::wave::T_PROTECTED] = &Vera::Structures::StatementOfAccessModifiers::create;
      factories_[boost::wave::T_PRIVATE] = &Vera::Structures::StatementOfAccessModifiers::create;
      factories_[boost::wave::T_QUESTION_MARK] =
        &Vera::Structures::StatementOfOperatorTernario::create;
      factories_[boost::wave::T_COLON] = &Vera::Structures::StatementOfOperatorTernario::create;
      factories_[boost::wave::T_LESS] = &Vera::Structures::StatementOfTemplateParameters::create;
      factories_[boost::wave::T_EXTERN] = &Vera::Structures::StatementOfExtern::create;
      factories_[boost::wave::T_ENUM] = &Vera::Structures::StatementOfEnum::create;
      factories_[boost::wave::T_LEFTPAREN] = &Vera::Structures::StatementOfParensArguments::create;
      factories_[boost::wave::T_LEFTBRACKET] =
        &Vera::Structures::StatementOfBracketsArguments::create;
      factories_[boost::wave::T_LEFTBRACE] = &Vera::Structures::StatementOfBracesArguments::create;
      factories_[boost::wave::T_UNION] = &Vera::Structures::StatementOfUnion::create;
      factories_[boost::wave::T_CLASS] = &Vera::Structures::StatementOfClass::create;
      factories_[boost::wave::T_PP_LINE] = &Vera::Structures::StatementOfPreprocessorLine::create;
      factories_[boost::wave::T_PP_DEFINE] = &Vera::Structures::StatementOfDefine::create;
      factories_[boost::wave::T_TYPEDEF] = &Vera::Structures::StatementOfTypedef::create;
    }

    /**
     * @brief Gets the adequate factory method for the given token.
     *
     * @param id Defines the identifier of the related token.
     * @return The adequate factory method  for the given id.
     */
    FactoryMethod operator() (boost::wave::token_id id)
    {
      if (factories_.find(id) == factories_.end())
      {
        throw;
      }

      return *factories_[id];
    }

    /**
     * @brief  Determines if there's a factory method for the given token.
     *
     * @param id Defines the identifier of the related token.
     * @return True if there's a factory method. Otherwise false.
     */
    bool isHandled(boost::wave::token_id id)
    {
      return factories_.find(id) != factories_.end();
    }

  private:

    /**
      * @brief The factory methods map type.
      */
     typedef std::map<boost::wave::token_id, FactoryMethod> Factories;

  private:
     Factories factories_;
} builders_;

/**
 * @brief Determines the most adequate method to verify the syntax of
 * the given statements.
 */
class SelectorOfVerifiers
{
  public:
    /**
     * @brief IsValidMethod type.
     */
    typedef bool (*IsValidMethod)(Vera::Structures::Tokens::TokenSequence::const_iterator it,
        Vera::Structures::Tokens::TokenSequence::const_iterator end);

  public:
    /**
     * @brief Initializes the verifiers for the supported strategies.
     */
    SelectorOfVerifiers ()
    {
      verifiers_[boost::wave::T_IF] = &Vera::Structures::StatementOfIf::isValid;
      verifiers_[boost::wave::T_FOR] = &Vera::Structures::StatementOfForLoop::isValid;
      verifiers_[boost::wave::T_TRY] = &Vera::Structures::StatementOfTryCatches::isValid;
      verifiers_[boost::wave::T_DO] = &Vera::Structures::StatementOfDoWhileLoop::isValid;
      verifiers_[boost::wave::T_CATCH] = &Vera::Structures::StatementOfCatch::isValid;
      verifiers_[boost::wave::T_ELSE] = &Vera::Structures::StatementOfElse::isValid;
      verifiers_[boost::wave::T_CASE] = &Vera::Structures::StatementOfCase::isValid;
      verifiers_[boost::wave::T_DEFAULT] = &Vera::Structures::StatementOfDefault::isValid;
      verifiers_[boost::wave::T_SWITCH] = &Vera::Structures::StatementOfSwitch::isValid;
      verifiers_[boost::wave::T_WHILE] = &Vera::Structures::StatementOfWhileLoop::isValid;
      verifiers_[boost::wave::T_NAMESPACE] = &Vera::Structures::StatementOfNamespace::isValid;
      verifiers_[boost::wave::T_STRUCT] = &Vera::Structures::StatementOfStruct::isValid;
      verifiers_[boost::wave::T_PUBLIC] = &Vera::Structures::StatementOfAccessModifiers::isValid;
      verifiers_[boost::wave::T_PROTECTED] = &Vera::Structures::StatementOfAccessModifiers::isValid;
      verifiers_[boost::wave::T_PRIVATE] = &Vera::Structures::StatementOfAccessModifiers::isValid;
      verifiers_[boost::wave::T_QUESTION_MARK] =
         &Vera::Structures::StatementOfOperatorTernario::isValid;
      verifiers_[boost::wave::T_COLON] = &Vera::Structures::StatementOfOperatorTernario::isValid;
      verifiers_[boost::wave::T_LESS] = &Vera::Structures::StatementOfTemplateParameters::isValid;
      verifiers_[boost::wave::T_EXTERN] = &Vera::Structures::StatementOfExtern::isValid;
      verifiers_[boost::wave::T_ENUM] = &Vera::Structures::StatementOfEnum::isValid;
      verifiers_[boost::wave::T_LEFTPAREN] = &Vera::Structures::StatementOfParensArguments::isValid;
      verifiers_[boost::wave::T_LEFTBRACKET] =
        &Vera::Structures::StatementOfBracketsArguments::isValid;
      verifiers_[boost::wave::T_LEFTBRACE] = &Vera::Structures::StatementOfBracesArguments::isValid;
      verifiers_[boost::wave::T_UNION] = &Vera::Structures::StatementOfUnion::isValid;
      verifiers_[boost::wave::T_PP_LINE] = &Vera::Structures::StatementOfPreprocessorLine::isValid;
      verifiers_[boost::wave::T_PP_DEFINE] = &Vera::Structures::StatementOfDefine::isValid;
      verifiers_[boost::wave::T_CLASS] = &Vera::Structures::StatementOfClass::isValid;
      verifiers_[boost::wave::T_TYPEDEF] = &Vera::Structures::StatementOfTypedef::isValid;
    }

    /**
     * @brief Gets the adequate verifier method for the given token.
     *
     * @param id Defines the identifier of the related token.
     * @return The adequate verifier method for the given id.
     */
    IsValidMethod operator() (boost::wave::token_id id)
    {
      if (isHandled(id) == false)
      {
        throw;
      }

      return *verifiers_[id];
    }

    /**
     * @brief Determines if there's a valid strategy for the given token.
     *
     * @param id Defines the identifier of the related token.
     * @return True if there's a valid strategy. Otherwise false.
     */
    bool isHandled(boost::wave::token_id id)
    {
      return verifiers_.find(id) != verifiers_.end();
    }

  private:

     /**
       * @brief The isValid methods map type.
       */
      typedef std::map<boost::wave::token_id, IsValidMethod> IsValidMapType;

  private:
      IsValidMapType verifiers_;

} isValid_;

} /*unname namespace */

namespace Vera
{
namespace Structures
{

using namespace underlyImpl;

namespace //unname
{

/**
 * @brief Gets the enumerator id associated to the token.
 *
 * @param token The reference to the token instance.
 * @return The id assigned to the token.
 */
static boost::wave::token_id
getTokenId(const Token& token);

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

} //unname namespace

Statement&
Statement::add()
{
  statementSequence_.push_back(Statement());

  Statement& child = statementSequence_.back();

  child.parent_ = this;
  child.doc_ = doc_;
  childs_.push_back(Statement::TYPE_ITEM_STATEMENT);

  return child;
}

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

  return isEqual && id_ == statement.id_;
}

void
Statement::push(Token token)
{
  tokenSequence_.push_back(token);
  childs_.push_back(Statement::TYPE_ITEM_TOKEN);
}

Statement*
Statement::getParent()
{
  return parent_;
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
    concreteBuilder.parse(it, end);
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
  return statement_.add();
}

void
StatementsBuilder::builder(Statement& response,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  if (it == end)
  {
    throw StatementsError("Token Collection is empty.");
  }

  boost::wave::token_id id = getTokenId(*it);
  StatementsBuilder partial(response);


  if (isValid_.isHandled(id) == true && isValid_(id)(it, end) == true)
  {
    partial.parse(it, end);
  }
  else
  {
    StatementsBuilder branch(partial.add());

    branch.parse(it, end);
  }
}

StatementsBuilder::StatementsBuilder(Statement& statement)
: statement_(statement)
, id_(statement.id_)
{
}

std::size_t
StatementsBuilder::getId() const
{
  return id_;
}

void
StatementsBuilder::addEachInvalidToken(Statement& current,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  Tokens::TokenSequence& tokenList = current.tokenSequence_;
  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it,
    end,
    IsValidTokenForStatement());

  std::copy(it, itMatched, std::back_inserter<Vera::Structures::Tokens::TokenSequence>(tokenList));

  for (int i = 0; i < std::distance(it, itMatched); ++i)
  {
    current.childs_.push_back(Statement::TYPE_ITEM_TOKEN);
  }

  it += std::distance(it, itMatched);
}

void
StatementsBuilder::addEachInvalidToken(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  Tokens::TokenSequence& tokenList = statement_.tokenSequence_;
  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it,
    end,
    IsValidTokenForStatement());

  std::copy(it, itMatched, std::back_inserter<Vera::Structures::Tokens::TokenSequence>(tokenList));

  for (int i = 0; i < std::distance(it, itMatched); ++i)
  {
    statement_.childs_.push_back(Statement::TYPE_ITEM_TOKEN);
  }

  it += std::distance(it, itMatched);
}


bool
StatementsBuilder::parseArguments(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  IS_EQUAL_RETURN_FALSE(it, end);

  addEachInvalidToken(statement_, it, end);

  IS_EQUAL_RETURN_FALSE(it, end);

  if (IsTokenWithId(boost::wave::T_LEFTPAREN)(*it) == false)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator rightLimit = std::find_if(it+1,
      end,
      EndsWithCorrectPattern(boost::wave::T_LEFTPAREN));

  Tokens::TokenSequence::const_iterator endArguments = (endArguments < end)? rightLimit + 1 : end;

  parse(it, endArguments);

  return IsTokenWithId(boost::wave::T_RIGHTPAREN)(*rightLimit);
}

void
StatementsBuilder::parseScope(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  IS_EQUAL_RETURN(it, end)

  if (IsTokenWithId(boost::wave::T_LEFTBRACE)(*it) ||
      IsTokenWithId(boost::wave::T_LEFTPAREN)(*it) ||
      IsTokenWithId(boost::wave::T_LEFTBRACE)(*it))
  {
    parse(it, end);
  }
  else
  {
    StatementsBuilder branch(add());
    branch.parse(it, end);
    ++it;
  }
}

const
Tokens::TokenSequence&
StatementsBuilder::getTokens()
{
  return statement_.tokenSequence_;
}

void
StatementsBuilder::push(Token token)
{
  statement_.push(token);
}

bool
StatementsBuilder::parseHeritage(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  if (IsTokenWithId(boost::wave::T_COLON)(*it) == false)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator endLeftbrace = std::find_if(it,
      end,
      IsTokenWithName(LEFTBRACE_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator endSemicolon = std::find_if(it,
      end,
      IsTokenWithName(SEMICOLON_TOKEN_NAME));

  if (endLeftbrace > endSemicolon || endLeftbrace >= end)
  {
    return false;
  }

  Statement& current = getCurrentStatement();

  push(*it);
  ++it;

  addEachInvalidToken(it, endLeftbrace);

  while (it < endLeftbrace)
  {
    builder(current, it, endLeftbrace);

    IS_EQUAL_BREAK(it, endLeftbrace);
    ++it;

    addEachInvalidToken(it, endLeftbrace);
  }

  return true;
}

void
StatementsBuilder::parseListScope(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = add();
  StatementsBuilder scope(current);

  scope.push(*it);
  ++it;

  while (it < end)
  {
    scope.addEachInvalidToken(it, end);

    Tokens::TokenSequence::const_iterator endItem = std::find_if(it,
      end,
      IsTokenWithId(boost::wave::T_COMMA));

    scope.builder(current, it, endItem);

    scope.push(*it);
    ++it;
  }
}

bool
StatementsBuilder::parseVariablesFromScopeToSemicolon(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  bool successful = true;

  IS_EQUAL_RETURN_FALSE(it, end);

  Tokens::TokenSequence::const_iterator endMatched =
    std::find_if(it, end, IsTokenWithName(SEMICOLON_TOKEN_NAME));

  Statement& current = add();
  StatementsBuilder partial(current);
  partial.addEachInvalidToken(it, endMatched);

  while (it < endMatched)
  {
    partial.builder(current, it, endMatched);

    IS_EQUAL_BREAK(it, endMatched);
    ++it;

    partial.addEachInvalidToken(it, endMatched);
  }

  if (IsTokenWithName(SEMICOLON_TOKEN_NAME)(*it) == true && successful)
  {
      push(*it);
  }
  else
  {
    successful = false;
  }

  return true;
}

void
StatementsBuilder::parse(TokenSequenceConstIterator& it,
  TokenSequenceConstIterator& end)
{
  while (it < end)
  {
    const struct Token& token = *it;

    boost::wave::token_id id = getTokenId(token);

    if (id == boost::wave::T_EOF)
    {
      break;
    }

    if ((id == boost::wave::T_QUESTION_MARK ||
      id == boost::wave::T_LEFTPAREN ||
      id == boost::wave::T_LEFTBRACKET ||
      id == boost::wave::T_PP_LINE ||
      id == boost::wave::T_PUBLIC ||
      id == boost::wave::T_PROTECTED ||
      id == boost::wave::T_PRIVATE ||
      id == boost::wave::T_LESS)
        && isValid_(id)(it, end) == true)
    {
      builders_(id)(statement_, it, end);
      IS_EQUAL_BREAK(it, end);
      continue;
    }

    if (builders_.isHandled(id) && isValid_(id)(it, end) == true)
    {
      builders_(id)(statement_, it, end);
      IS_EQUAL_BREAK(it, end);
      //++it;
      break;
    }

    if (id == boost::wave::T_RIGHTPAREN)
    {
      push(*it);
      break;
    }

    if (id == boost::wave::T_RIGHTBRACKET)
    {
      push(*it);
      break;
    }

    if (id == boost::wave::T_COLON)
    {
      push(*it);
      break;
    }

    if (id == boost::wave::T_SEMICOLON)
    {
      push(*it);
      break;
    }

    if (id == boost::wave::T_COMMA)
    {
      push(*it);
      break;
    }

    push(*it);
    ++it;
  }
}

}
}
