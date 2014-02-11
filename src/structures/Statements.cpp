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
#include "StatementOfInclude.h"
#include "StatementOfTemplate.h"
#include "StatementOfFunction.h"
#include "StatementOfOperator.h"
#include "StatementOfPreprocessorDirectives.h"
#include "StatementOfAssign.h"

#include "IsTokenWithName.h"
#include "../plugins/Messages.h"

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
      using namespace Vera::Structures;

      factories_[boost::wave::T_IF] = &StatementOfIf::create;
      factories_[boost::wave::T_FOR] = &StatementOfForLoop::create;
      factories_[boost::wave::T_TRY] = &StatementOfTryCatches::create;
      factories_[boost::wave::T_DO] = &StatementOfDoWhileLoop::create;
      factories_[boost::wave::T_CATCH]= &StatementOfCatch::create;
      factories_[boost::wave::T_ELSE] = &StatementOfElse::create;
      factories_[boost::wave::T_CASE] = &StatementOfCase::create;
      factories_[boost::wave::T_DEFAULT] = &StatementOfDefault::create;
      factories_[boost::wave::T_SWITCH] = &StatementOfSwitch::create;
      factories_[boost::wave::T_WHILE] = &StatementOfWhileLoop::create;
      factories_[boost::wave::T_NAMESPACE] = &StatementOfNamespace::create;
      factories_[boost::wave::T_STRUCT] = &StatementOfStruct::create;
      factories_[boost::wave::T_PUBLIC] = &StatementOfAccessModifiers::create;
      factories_[boost::wave::T_PROTECTED] = &StatementOfAccessModifiers::create;
      factories_[boost::wave::T_PRIVATE] = &StatementOfAccessModifiers::create;
      factories_[boost::wave::T_QUESTION_MARK] = &StatementOfOperatorTernario::create;
      factories_[boost::wave::T_COLON] = &StatementOfOperatorTernario::create;
      factories_[boost::wave::T_LESS] = &StatementOfTemplateParameters::create;
      factories_[boost::wave::T_EXTERN] = &StatementOfExtern::create;
      factories_[boost::wave::T_ENUM] = &StatementOfEnum::create;
      factories_[boost::wave::T_LEFTPAREN] = &StatementOfParensArguments::create;
      factories_[boost::wave::T_LEFTBRACKET] = &StatementOfBracketsArguments::create;
      factories_[boost::wave::T_LEFTBRACE] = &StatementOfBracesArguments::create;
      factories_[boost::wave::T_UNION] = &StatementOfUnion::create;
      factories_[boost::wave::T_CLASS] = &StatementOfClass::create;
      factories_[boost::wave::T_PP_LINE] = &StatementOfPreprocessorLine::create;
      factories_[boost::wave::T_PP_DEFINE] = &StatementOfDefine::create;
      factories_[boost::wave::T_TYPEDEF] = &StatementOfTypedef::create;
      factories_[boost::wave::T_PP_HHEADER] = &StatementOfInclude::create;
      factories_[boost::wave::T_PP_QHEADER] = &StatementOfInclude::create;
      factories_[boost::wave::T_TEMPLATE] = &StatementOfTemplate::create;
      factories_ [boost::wave::T_OPERATOR] = &StatementOfOperator::create;
      factories_[boost::wave::T_PP_UNDEF] = &StatementOfPreprocessorDirectives::create;
      factories_[boost::wave::T_PP_WARNING] = &StatementOfPreprocessorDirectives::create;
      factories_[boost::wave::T_PP_IF] = &StatementOfPreprocessorDirectives::create;
      factories_[boost::wave::T_PP_ELSE] = &StatementOfPreprocessorDirectives::create;
      factories_[boost::wave::T_PP_IFDEF] =  &StatementOfPreprocessorDirectives::create;
      factories_[boost::wave::T_PP_IFNDEF] = &StatementOfPreprocessorDirectives::create;
      factories_[boost::wave::T_PP_PRAGMA] = &StatementOfPreprocessorDirectives::create;
      factories_ [boost::wave::T_PP_ERROR] = &StatementOfPreprocessorDirectives::create;
      factories_ [boost::wave::T_PP_ENDIF] =  &StatementOfPreprocessorDirectives::create;
      factories_ [boost::wave::T_PP_ELIF] = &StatementOfPreprocessorDirectives::create;
      factories_ [boost::wave::T_ASSIGN] = &StatementOfAssign::create;
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
      using namespace Vera::Structures;

      verifiers_[boost::wave::T_IF] = &StatementOfIf::isValid;
      verifiers_[boost::wave::T_FOR] = &StatementOfForLoop::isValid;
      verifiers_[boost::wave::T_TRY] = &StatementOfTryCatches::isValid;
      verifiers_[boost::wave::T_DO] = &StatementOfDoWhileLoop::isValid;
      verifiers_[boost::wave::T_CATCH] = &StatementOfCatch::isValid;
      verifiers_[boost::wave::T_ELSE] = &StatementOfElse::isValid;
      verifiers_[boost::wave::T_CASE] = &StatementOfCase::isValid;
      verifiers_[boost::wave::T_DEFAULT] = &StatementOfDefault::isValid;
      verifiers_[boost::wave::T_SWITCH] = &StatementOfSwitch::isValid;
      verifiers_[boost::wave::T_WHILE] = &StatementOfWhileLoop::isValid;
      verifiers_[boost::wave::T_NAMESPACE] = &StatementOfNamespace::isValid;
      verifiers_[boost::wave::T_STRUCT] = &StatementOfStruct::isValid;
      verifiers_[boost::wave::T_PUBLIC] = &StatementOfAccessModifiers::isValid;
      verifiers_[boost::wave::T_PROTECTED] = &StatementOfAccessModifiers::isValid;
      verifiers_[boost::wave::T_PRIVATE] = &StatementOfAccessModifiers::isValid;
      verifiers_[boost::wave::T_QUESTION_MARK] = &StatementOfOperatorTernario::isValid;
      verifiers_[boost::wave::T_COLON] = &StatementOfOperatorTernario::isValid;
      verifiers_[boost::wave::T_LESS] = &StatementOfTemplateParameters::isValid;
      verifiers_[boost::wave::T_EXTERN] = &StatementOfExtern::isValid;
      verifiers_[boost::wave::T_ENUM] = &StatementOfEnum::isValid;
      verifiers_[boost::wave::T_LEFTPAREN] = &StatementOfParensArguments::isValid;
      verifiers_[boost::wave::T_LEFTBRACKET] = &StatementOfBracketsArguments::isValid;
      verifiers_[boost::wave::T_LEFTBRACE] = &StatementOfBracesArguments::isValid;
      verifiers_[boost::wave::T_UNION] = &StatementOfUnion::isValid;
      verifiers_[boost::wave::T_PP_LINE] = &StatementOfPreprocessorLine::isValid;
      verifiers_[boost::wave::T_PP_DEFINE] = &StatementOfDefine::isValid;
      verifiers_[boost::wave::T_CLASS] = &StatementOfClass::isValid;
      verifiers_[boost::wave::T_TYPEDEF] = &StatementOfTypedef::isValid;
      verifiers_[boost::wave::T_PP_HHEADER] = &StatementOfInclude::isValid;
      verifiers_[boost::wave::T_PP_QHEADER] = &StatementOfInclude::isValid;
      verifiers_[boost::wave::T_TEMPLATE] = &StatementOfTemplate::isValid;
      verifiers_[boost::wave::T_OPERATOR] = &StatementOfOperator::isValid;
      verifiers_[boost::wave::T_PP_UNDEF] = &StatementOfPreprocessorDirectives::isValid;
      verifiers_[boost::wave::T_PP_WARNING] = &StatementOfPreprocessorDirectives::isValid;
      verifiers_[boost::wave::T_PP_IF] =&StatementOfPreprocessorDirectives::isValid;
      verifiers_[boost::wave::T_PP_ELSE] =&StatementOfPreprocessorDirectives::isValid;
      verifiers_[boost::wave::T_PP_IFDEF] =  &StatementOfPreprocessorDirectives::isValid;
      verifiers_[boost::wave::T_PP_IFNDEF] =  &StatementOfPreprocessorDirectives::isValid;
      verifiers_[boost::wave::T_PP_PRAGMA] = &StatementOfPreprocessorDirectives::isValid;
      verifiers_[boost::wave::T_PP_ERROR] = &StatementOfPreprocessorDirectives::isValid;
      verifiers_[boost::wave::T_PP_ENDIF] = &StatementOfPreprocessorDirectives::isValid;
      verifiers_[boost::wave::T_PP_ELIF] = &StatementOfPreprocessorDirectives::isValid;
      verifiers_[boost::wave::T_PP_ELSE] = &StatementOfPreprocessorDirectives::isValid;
      verifiers_[boost::wave::T_ASSIGN] = &StatementOfAssign::isValid;
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
  Statement branch(Token("branch", 0, 0, "unknown"));
  branch.parentId_ = id_;
  branch.parent_ = this;
  branch.doc_ = doc_;
  branch.type_ = TYPE_ITEM_STATEMENT;

  statementSequence_.push_back(branch);

  StatementsBuilder::addNodeToCollection(statementSequence_.back());

  return statementSequence_.back();
}


std::map<std::size_t, Statement*> statementsCollection_;

Statement::Statement(const Statement& object)
: parentId_(object.parentId_)
, doc_(object.doc_)
, token_(object.token_)
, type_(object.type_)
, id_(object.id_)
, parent_(object.parent_)
{
  StatementSequence::const_iterator it = object.statementSequence_.begin();
  StatementSequence::const_iterator end = object.statementSequence_.end();

  std::copy(it, end, std::back_inserter<StatementSequence>(statementSequence_));
}

Statement::~Statement()
{
}

bool
Statement::operator==(const Statement& statement) const
{
  bool isEqual = token_ == statement.token_ && parentId_ == statement.parentId_ && parent_ == statement.parent_;

  if (isEqual && statementSequence_.size() == statement.statementSequence_.size())
  {
    isEqual = std::equal(statementSequence_.begin(),
        statementSequence_.end(),
      statement.statementSequence_.begin());
  }

  return isEqual && id_ == statement.id_;
}

void
Statement::push(const Token& token)
{
  Statement item(token);
  item.parentId_ = id_;
  item.parent_ = this;
  item.doc_ = doc_;

  statementSequence_.push_back(item);
  StatementsBuilder::addNodeToCollection(statementSequence_.back());
}

const Statement&
Statement::getParent()
{
  return *parent_;
}

const Token&
Statement::getToken()
{
  return token_;
}

static bool isFunction_ = false;

Statement
StatementsBuilder::create(Token token, Tokens::TokenSequence& tokenCollection)
{
  Statement response(token);
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
StatementsBuilder::builder(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  if (it == end)
  {
    throw StatementsError("Token Collection is empty.");
  }

  boost::wave::token_id id = getTokenId(*it);

  if (isValid_.isHandled(id) == true && isValid_(id)(it, end) == true)
  {
    parse(it, end);
  }
  else if (StatementOfFunction::isValid(it, end))
  {
    parse(it, end);
  }
  else
  {
    Statement& current = statement_.add();

    StatementsBuilder branch(current);

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
StatementsBuilder::addEachInvalidToken(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it,
    end,
    IsValidTokenForStatement());

  for (;it < itMatched; ++it)
  {
    statement_.push(*it);
  }
}

bool
StatementsBuilder::parseArguments(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  IS_EQUAL_RETURN_FALSE(it, end);

  addEachInvalidToken(it, end);

  IS_EQUAL_RETURN_FALSE(it, end);

  if (IsTokenWithId(boost::wave::T_LEFTPAREN)(*it) == false)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator rightLimit = std::find_if(it+1,
      end,
      EndsWithCorrectPattern(boost::wave::T_LEFTPAREN));

  Tokens::TokenSequence::const_iterator endArguments = (rightLimit < end)? rightLimit + 1 : end;

  StatementOfParensArguments::create(statement_, it, endArguments);

  return IsTokenWithId(boost::wave::T_RIGHTPAREN)(*rightLimit);
}

void
StatementsBuilder::parseScope(Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
{
  IS_EQUAL_RETURN(it, end)

  if (IsTokenWithId(boost::wave::T_LEFTBRACE)(*it) ||
      IsTokenWithId(boost::wave::T_LEFTPAREN)(*it) ||
      IsTokenWithId(boost::wave::T_LEFTBRACKET)(*it))
  {
    parse(it, end);
  }
  else
  {
    builder(it, end);
  }
}

const
Statement::StatementSequence&
StatementsBuilder::getScope()
{
  return statement_.statementSequence_;
}

void
StatementsBuilder::push(const Token& token)
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
    builder(it, endLeftbrace);

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

    scope.builder(it, endItem);

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

  addEachInvalidToken(it, end);

  Tokens::TokenSequence::const_iterator semicolonMatched =
    std::find_if(it, end, IsTokenWithName(SEMICOLON_TOKEN_NAME));

  if (it == semicolonMatched)
  {
    push(*it);
    return true;
  }

  Statement& current = add();
  current.type_ = Statement::TYPE_ITEM_STATEMENT_OF_DECLARATION_LIST;

  StatementsBuilder partial(current);

  while (it < semicolonMatched)
  {
    partial.builder(it, semicolonMatched);

    IS_EQUAL_BREAK(it, semicolonMatched);
    ++it;

    partial.addEachInvalidToken(it, semicolonMatched);
  }

  if (IsTokenWithName(SEMICOLON_TOKEN_NAME)(*it) == true)
  {
      push(*it);
  }
  else
  {
    successful = false;
  }

  return successful;
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
      id == boost::wave::T_OPERATOR ||
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

    if (StatementOfFunction::isValid(it, end) == true && isFunction_ == false)
    {
      isFunction_ = true;
      StatementOfFunction::create(statement_, it, end);
      IS_EQUAL_BREAK(it, end);
      isFunction_ = false;
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

    if (id == boost::wave::T_COLON &&
        isFunction_ == true &&
        statement_.parent_ != NULL)
    {
      if (statement_.parent_->type_ == Statement::TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS &&
          statement_.parent_->parent_->type_ != Statement::TYPE_ITEM_STATEMENT_OF_UNION)
      {
        push(*it);
        break;
      }
    }

    if (id == boost::wave::T_SEMICOLON)
    {
      push(*it);
      break;
    }

#ifdef ENABLE_COMMA_BREAK

    //  TODO The statements related to multiple variables declaration could not be assembled correctly.
    if (id == boost::wave::T_COMMA)
    {
      push(*it);
      break;
    }

#endif

    push(*it);
    ++it;
  }
}

bool
StatementsBuilder::requiredContinue()
{
  return false;
}

void StatementsBuilder::addNodeToCollection(Statement& node)
{
  statementsCollection_[node.id_] = &node;
}

#define CASE_TYPE(item) \
{ \
  case Vera::Structures::Statement::TYPE_##item: \
     return "TYPE_"#item; \
} 


static std::string
typesToString(Statement::TypeItem type)
{
  using Vera::Structures::Statement;

  switch(type)
  {
    CASE_TYPE(ITEM_TOKEN)
    CASE_TYPE(ITEM_STATEMENT)
    CASE_TYPE(ITEM_ROOT)
    CASE_TYPE(ITEM_STATEMENT_OF_IF)
    CASE_TYPE(ITEM_STATEMENT_OF_FORLOOP)
    CASE_TYPE(ITEM_STATEMENT_OF_WHILELOOP)
    CASE_TYPE(ITEM_STATEMENT_OF_TRYCATCHES)
    CASE_TYPE(ITEM_STATEMENT_OF_CATCH)
    CASE_TYPE(ITEM_STATEMENT_OF_DOWHILELOOP)
    CASE_TYPE(ITEM_STATEMENT_OF_WHILE)
    CASE_TYPE(ITEM_STATEMENT_OF_ELSE)
    CASE_TYPE(ITEM_STATEMENT_OF_SWITCH)
    CASE_TYPE(ITEM_STATEMENT_OF_NAMESPACE)
    CASE_TYPE(ITEM_STATEMENT_OF_NAMESPACE_UNNAME)
    CASE_TYPE(ITEM_STATEMENT_OF_STRUCT)
    CASE_TYPE(ITEM_STATEMENT_OF_STRUCT_UNNAME)
    CASE_TYPE(ITEM_STATEMENT_OF_ACCESSMODIFIERS)
    CASE_TYPE(ITEM_STATEMENT_OF_DEFAULT)
    CASE_TYPE(ITEM_STATEMENT_OF_CASE)
    CASE_TYPE(ITEM_STATEMENT_OF_HERITAGE)
    CASE_TYPE(ITEM_STATEMENT_OF_DECLARATION_LIST)
    CASE_TYPE(ITEM_STATEMENT_OF_INITIALIZER_LIST)
    CASE_TYPE(ITEM_STATEMENT_OF_OPERATORTERNARIO)
    CASE_TYPE(ITEM_STATEMENT_OF_EXTERN)
    CASE_TYPE(ITEM_STATEMENT_OF_TEMPLATEPARAMETERS)
    CASE_TYPE(ITEM_STATEMENT_OF_ENUM)
    CASE_TYPE(ITEM_STATEMENT_OF_ENUM_UNNAME)
    CASE_TYPE(ITEM_STATEMENT_OF_PARENSARGUMENTS)
    CASE_TYPE(ITEM_STATEMENT_OF_BRACKETSARGUMENTS)
    CASE_TYPE(ITEM_STATEMENT_OF_BRACESARGUMENTS)
    CASE_TYPE(ITEM_STATEMENT_OF_PREPROCESSORLINE)
    CASE_TYPE(ITEM_STATEMENT_OF_UNION)
    CASE_TYPE(ITEM_STATEMENT_OF_UNION_UNNAME)
    CASE_TYPE(ITEM_STATEMENT_OF_DEFINE)
    CASE_TYPE(ITEM_STATEMENT_OF_CLASS)
    CASE_TYPE(ITEM_STATEMENT_OF_TYPEDEF)
    CASE_TYPE(ITEM_STATEMENT_OF_TYPEDEF_SIGNATURE)
    CASE_TYPE(ITEM_STATEMENT_OF_TYPEDEF_STRUCT)
    CASE_TYPE(ITEM_STATEMENT_OF_TYPEDEF_UNION)
    CASE_TYPE(ITEM_STATEMENT_OF_TYPEDEF_ENUM)
    CASE_TYPE(ITEM_STATEMENT_OF_INCLUDE)
    CASE_TYPE(ITEM_STATEMENT_OF_FUNCTION)
    CASE_TYPE(ITEM_STATEMENT_OF_OPERATOR)
    CASE_TYPE(ITEM_STATEMENT_OF_TEMPLATE)
    CASE_TYPE(ITEM_STATEMENT_OF_ASSIGN)

    default:
      break;
  }

  return "";
}


Statement*
StatementsBuilder::getNodeToCollection(std::size_t id)
{
  std::map<std::size_t, Statement*>::iterator it = statementsCollection_.find(id);

  if (it == statementsCollection_.end())
  {
    return NULL;
  }

  std::stringstream out;
  out << "Parent: "<<typesToString(statementsCollection_[id]->type_)<<std::endl;

  Plugins::Message::get_mutable_instance().show(out.str());

  return statementsCollection_[id];
}

}
}
