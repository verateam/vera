//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STATEMENTS_H_INCLUDED
#define STATEMENTS_H_INCLUDED

#include "Tokens.h"
#include <boost/smart_ptr.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

namespace Vera
{
namespace Structures
{

class Document;

/**
 * @brief Wrapper implemented to define the errors related to
 * the parse statements.
 */
class StatementsError : public std::runtime_error
{
public:
    /**
     * @brief  Initializes a new instance of the StatementsError class.
     * @param msg The message associated to the error.
     */
    StatementsError(const std::string & msg) : std::runtime_error(msg) {}
};

/**
 * @brief Implemented to define a statement.
 */
struct Statement
{
    enum TypeItem
    {
      TYPE_ITEM_TOKEN,
      TYPE_ITEM_STATEMENT,
      TYPE_ITEM_ROOT,
      TYPE_ITEM_STATEMENT_OF_IF,
      TYPE_ITEM_STATEMENT_OF_FORLOOP,
      TYPE_ITEM_STATEMENT_OF_WHILELOOP,
      TYPE_ITEM_STATEMENT_OF_TRYCATCHES,
      TYPE_ITEM_STATEMENT_OF_CATCH,
      TYPE_ITEM_STATEMENT_OF_DOWHILELOOP,
      TYPE_ITEM_STATEMENT_OF_WHILE,
      TYPE_ITEM_STATEMENT_OF_ELSE,
      TYPE_ITEM_STATEMENT_OF_SWITCH,
      TYPE_ITEM_STATEMENT_OF_NAMESPACE,
      TYPE_ITEM_STATEMENT_OF_NAMESPACE_UNNAME,
      TYPE_ITEM_STATEMENT_OF_STRUCT,
      TYPE_ITEM_STATEMENT_OF_STRUCT_UNNAME,
      TYPE_ITEM_STATEMENT_OF_ACCESSMODIFIERS,
      TYPE_ITEM_STATEMENT_OF_DEFAULT,
      TYPE_ITEM_STATEMENT_OF_CASE,
      TYPE_ITEM_STATEMENT_OF_HERITAGE,
      TYPE_ITEM_STATEMENT_OF_DECLARATION_LIST,
      TYPE_ITEM_STATEMENT_OF_INITIALIZER_LIST,
      TYPE_ITEM_STATEMENT_OF_OPERATORTERNARIO,
      TYPE_ITEM_STATEMENT_OF_EXTERN,
      TYPE_ITEM_STATEMENT_OF_TEMPLATEPARAMETERS,
      TYPE_ITEM_STATEMENT_OF_ENUM,
      TYPE_ITEM_STATEMENT_OF_ENUM_UNNAME,
      TYPE_ITEM_STATEMENT_OF_USING_NAMESPACE,
      TYPE_ITEM_STATEMENT_OF_USING,
      TYPE_ITEM_STATEMENT_OF_FRIEND,
      TYPE_ITEM_STATEMENT_OF_PARENSARGUMENTS,
      TYPE_ITEM_STATEMENT_OF_BRACKETSARGUMENTS,
      TYPE_ITEM_STATEMENT_OF_BRACESARGUMENTS,
      TYPE_ITEM_STATEMENT_OF_PREPROCESSORLINE,
      TYPE_ITEM_STATEMENT_OF_UNION,
      TYPE_ITEM_STATEMENT_OF_UNION_UNNAME,
      TYPE_ITEM_STATEMENT_OF_DEFINE,
      TYPE_ITEM_STATEMENT_OF_DEFINE_SIGNATURE,
      TYPE_ITEM_STATEMENT_OF_CLASS,
      TYPE_ITEM_STATEMENT_OF_SIGNATURE_DECLARATION,
      TYPE_ITEM_STATEMENT_OF_TYPEDEF,
      TYPE_ITEM_STATEMENT_OF_TYPEDEF_SIGNATURE,
      TYPE_ITEM_STATEMENT_OF_TYPEDEF_STRUCT,
      TYPE_ITEM_STATEMENT_OF_TYPEDEF_UNION,
      TYPE_ITEM_STATEMENT_OF_TYPEDEF_ENUM,
      TYPE_ITEM_STATEMENT_OF_INCLUDE,
      TYPE_ITEM_STATEMENT_OF_FUNCTION,
      TYPE_ITEM_STATEMENT_OF_OPERATOR,
      TYPE_ITEM_STATEMENT_OF_TEMPLATE,
      TYPE_ITEM_STATEMENT_OF_ASSIGN
    };

    /**
     * @brief Statement collection type.
     */
    typedef Statement* StatementPointer;
    typedef std::vector<Statement> StatementSequence;

    typedef std::vector<TypeItem> SequenceOfChilds;

    Statement();

    Statement(const Token& token);

    Statement(const Statement& object);

    ~Statement();

    Statement& operator=(const Statement& object);

    void push(const Token& token);

    /**
     * @brief Adds a new statement to the statement collection associated to the parent statement.
     *
     * @param current
     * @return The reference to the new statement.
     */
    Statement& add();

    const Token& getToken();

    const Statement& getParent();
    
    Statement& getFront();
    Statement& getBack();

    /**
     * @brief This is the comparison operator required
     * for the indexing_suite of the boost python.
     *
     * @param statement Reference to the statement to be compared.
     * @return True if is equal. Otherwise false.
     */
    bool operator==(Statement const& statement) const;

  StatementSequence statementSequence_;
  Token token_;
  std::size_t parentId_;
  std::size_t id_;
  Document* doc_;
  TypeItem type_;
  Statement* parent_;
};

/**
 * @brief Class that creates statement sequences relative to a specified token.
 */
class StatementsBuilder : public boost::noncopyable_::noncopyable
{
  friend class StatementOfIf;
  friend class StatementOfForLoop;
  friend class StatementOfWhileLoop;
  friend class StatementOfTryCatches;
  friend class StatementOfCatch;
  friend class StatementOfDoWhileLoop;
  friend class StatementOfElse;
  friend class StatementOfSwitch;
  friend class StatementOfNamespace;
  friend class StatementOfStruct;
  friend class StatementOfAccessModifiers;
  friend class StatementOfDefault;
  friend class StatementOfCase;
  friend class StatementOfOperatorTernario;
  friend class StatementOfExtern;
  friend class StatementOfTemplateParameters;
  friend class StatementOfEnum;
  friend class StatementOfParensArguments;
  friend class StatementOfBracketsArguments;
  friend class StatementOfBracesArguments;
  friend class StatementOfPreprocessorLine;
  friend class StatementOfUnion;
  friend class StatementOfDefine;
  friend class StatementOfClass;
  friend class StatementOfTypedef;
  friend class StatementOfInclude;
  friend class StatementOfTemplate;
  friend class StatementOfFunction;
  friend class StatementOfOperator;
  friend class StatementOfPreprocessorDirectives;
  friend class StatementOfAssign;
  friend class StatementOfUsing;
  friend class Document;
  friend class StatementOfFriend;
  friend class StatementOfIdentifier;

  public:

    /**
     * @brief Creates a statement for the given token.
     *
     * @param item Is the initial element of the declaration.
     * @param tokenCollection Is the reference to the token
     * collection that needs to be analyzed.
     * @return An instance of the statement with the
     * related tokens.
     */
    static Statement create(Token item, Tokens::TokenSequence& tokenCollection);

    /**
     * @brief Concrete builder of the statement, this method retrieves the statements parsed
     * from the first iterator. And ends at the end of the scope or when it comes to the
     * final iterator.
     *
     * @param response is a composed structure used to retrieve the related statements/tokens for
     * the given sentence.
     * @param it The const reference to the first iterator of the collection to be parsed.
     * @param end The const reference to the last iterator of the collection to be parsed.
     */
     void builder(Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Gets the tokens of the current sentence.
     *
     * @return The const reference to the token collection.
     */
    const Statement::StatementSequence& getScope();


    /*static void addNodeToCollection(Statement& node);*/

    static Statement* getNodeToCollection(std::size_t id);

    std::size_t getId() const;

    bool isSignature(const Statement& root);

  protected:

    /**
     * @brief Adds a new statement to the statement collection associated to the parent statement.
     * @return The reference to the new statement.
     */
    Statement& add();


    void push(const Token& token);


    /**
     * @brief Gets the parent of the current statement.
     * @return The reference to the parent of the current statement.
     */
    Statement& getCurrentStatement();

    /**
     * @brief Sets the parent of the current statement.
     * @param statement The reference to the parent of the current statement.
     */
    void setCurrentStatement(Statement& statement);

    /**
     * @brief  Initializes a new instance of the StatementsBuilder class.
     * @param statement The reference to the parent statement.
     */
    StatementsBuilder(Statement& statement);

    /**
     * @brief Adds all the tokens that a statement can not contain.
     *
     * @param current The reference to the parent of the current statement.
     * @param it The const reference to the first iterator of the collection to be parsed.
     * @param end The const reference to the last iterator of the collection to be parsed.
     */
    void addEachInvalidToken(Statement& current,
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    void addEachInvalidToken(Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Parses all the parameters on the given sentence.
     * Where the given sentence is determined by a start and end point.
     *
     * @param it Defines the starting point of the statement.
     * @param end Defines the ending point of the statement.
     * @return True if it found a valid argument. Otherwise false.
     */
    bool parseArguments(Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Parses the given list in order to determine all
     * the valid tokens contained on its scope.
     *
     * @param it Defines the starting point of the statement.
     * @param end Defines the ending point of the statement.
     */
    void parseScope(Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Parses the given list in order to determine the
     * related heritage.
     *
     * @param it Defines the starting point of the statement.
     * @param end Defines the ending point of the statement.
     * @return True if it has a valid heritage. Otherwise false.
     */
    bool parseHeritage(Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Parses the parameter on the given sentence.
     * Where the given sentence is determined by the start and end point.
     *
     * @param it Defines the starting point of the statement.
     * @param end Defines the ending point of the statement.
     * @return True if it is a valid argument. Otherwise false.
     */
    bool parseArgument(
        Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

    void parseListScope(Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

    bool parseVariablesFromScopeToSemicolon(Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Parses the statements from the token list
     * and retrieves the related list of tokens.
     *
     * used to return the response.
     * @param it the reference to the const iterator with the
     * initial element of the statement.
     * @param end the of the list to be analyzed.
     */
    void parse(TokenSequenceConstIterator& it,
      TokenSequenceConstIterator& end);

    virtual bool requiredContinue();

    std::string getDefaultName();

  protected:

    std::size_t id_;

  private:

    Statement& statement_;
};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTS_H_INCLUDED
