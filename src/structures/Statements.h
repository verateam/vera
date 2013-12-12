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
    StatementsError(const std::string & msg) : std::runtime_error(msg) {}
};

/**
 * @brief Implemented to define a statement.
 */
struct Statement
{
    /**
     * @brief Statement collection type.
     */
    typedef std::vector<Statement> StatementSequence;

    Statement(){}

    /**
     * @brief This is the comparison operator required
     * by the indexing_suite of the boost python.
     *
     * @param statement Reference to the statement to be compared.
     * @return True if is equal. Otherwise false.
     */
    bool operator==(Statement const& statement) const;

  StatementSequence statementSequence_;
  Tokens::TokenSequence tokenSequence_;
};

/**
 * @brief
 */
class Statements
{
  public:

    /**
     * @brief Gets the related statement of the given token.
     *
     * @param token Is the start element of the declaration.
     * @param tokenCollection Is the reference to the token
     * collection to be analyzed.
     * @return An instance of the statement with the
     * related tokens.
     */
    static Statement getTokensOfStament(Token item, Tokens::TokenSequence& tokenCollection);

};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTS_H_INCLUDED
