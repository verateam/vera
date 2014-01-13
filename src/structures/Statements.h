//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STATEMENTS_H_INCLUDED
#define STATEMENTS_H_INCLUDED

#include "Tokens.h"
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

namespace Vera
{
namespace Structures
{

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
      TYPE_ITEM_STATEMENT
    };

    /**
     * @brief Statement collection type.
     */
    typedef std::vector<Statement> StatementSequence;

    typedef std::vector<TypeItem> SequenceOfChilds;

    Statement(){}


    void push(Token token);

    /**
     * @brief This is the comparison operator required
     * for the indexing_suite of the boost python.
     *
     * @param statement Reference to the statement to be compared.
     * @return True if is equal. Otherwise false.
     */
    bool operator==(Statement const& statement) const;

  StatementSequence statementSequence_;
  Tokens::TokenSequence tokenSequence_;
  SequenceOfChilds childs_;
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
  //friend class StatementOfCases;
  friend class StatementOfNamespace;
  friend class StatementOfStruct;
  friend class StatementOfAccessModifiers;
  friend class StatementOfDefault;
  friend class StatementOfCase;
  friend class Document;
  friend class StatementOfOperatorTernario;

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
    void builder(Statement& response,
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Gets the tokens of the current sentence.
     *
     * @return The const reference to the token collection.
     */
    const Tokens::TokenSequence& getTokens();

  protected:

    /**
     * @brief Adds a new statement to the statement collection associated to the parent statement.
     * @return The reference to the new statement.
     */
    Statement& add();


    void push(Token token);

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
     * @brief Parses the given list in order to determine if it
     * contains the token with the given id.
     *
     * @param it Defines the starting point of the statement.
     * @param end Defines the ending point of the statement.
     * @param id_ Defines the id of the last token.
     * @return True if it contains the given last token. Otherwise false.
     */
    bool paramentersWithLastToken(
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end,
      boost::wave::token_id id_);

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

    bool parseVariablesFromScopeToSemicolon(Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end, std::vector<boost::wave::token_id>& finishTypeList);

    bool parseVariableDeclaration(Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end,
        std::vector<boost::wave::token_id>& finishTypeList);

  private:

    Statement& statement_;
};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTS_H_INCLUDED
