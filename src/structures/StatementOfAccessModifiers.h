//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STATEMENTOFACCESSMODIFIERS_H_INCLUDED
#define STATEMENTOFACCESSMODIFIERS_H_INCLUDED

#include "Tokens.h"
#include <string>
#include <vector>
#include "Statements.h"

namespace Vera
{
namespace Structures
{

/**
 * @brief Class that implements a decorator dedicated to the construction
 * of Statements of modifier access type.
 */
class StatementOfAccessModifiers
: public StatementsBuilder
{
  public:

    /**
     * @brief Initializes an instance of StatementOfAccessModifiers.
     *
     * @param statement The reference to the statement.
     * @param it The initial point of the statement.
     * @param end The end point of the statement.
     */
    StatementOfAccessModifiers(Statement& statement,
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Determines if a new statement can be created from the given tokens.
     *
     * @param it The initial point of the statement.
     * @param end The end point of the statement.
     * 
     * @return True if a new statement can be created, otherwise, false.
     */
    static bool isValid(Tokens::TokenSequence::const_iterator it,
        Tokens::TokenSequence::const_iterator end);
        
    /**
     * @brief Creates a new statement from the given tokens.
     *
     * @param statement The reference to the statement.
     * @param it The initial point of the statement.
     * @param end The end point of the statement.
     * 
     * @return True if the new statement was created, otherwise, false.
     */
    static bool create(Statement& statement,
        Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

    bool requiredContinue();

  private:
        
    /**
     * @brief Executes the analysis of the first tokens of the sentence.
     */
    void initialize();

  private:

    Tokens::TokenSequence::const_iterator& it_;
    Tokens::TokenSequence::const_iterator& end_;
};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTOFACCESSMODIFIERS_H_INCLUDED
