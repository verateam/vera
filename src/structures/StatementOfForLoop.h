//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STATEMENTOFFORLOOP_H_INCLUDED
#define STATEMENTOFFORLOOP_H_INCLUDED

#include "Tokens.h"
#include <string>
#include <vector>
#include "Statements.h"

namespace Vera
{
namespace Structures
{

/**
 * @brief Classes that implements a decorator dedicated to the construction
 * of Statements of type "for loop".
 */
class StatementOfForLoop
: public StatementsBuilder
{
  public:

    StatementOfForLoop(Statement& statement,
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Gets the tokens of the current sentence.
     *
     * @return The const reference to the token collection.
     */
    const Tokens::TokenSequence& getTokens();

    /**
     * @brief Gets the arguments of the current sentence.
     *
     * @return The const reference to the Statement structure
     * which contains the associated tokens.
     */
    const Statement& getArgumentStatementFromConditionalSentence();

    /**
     * @brief Gets the scope of the current sentence.
     *
     * @return The const reference to the Statement structure
     * which contains the associated tokens.
     */
    const Statement& getStatementScope();

  private:

    bool extractOneStatementArgument(StatementsBuilder& builder,
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end,
      std::string tokenName);

    void initialize(Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Parses all the parameters on the given sentence.
     * Where the given sentence is determined by a start and end point.
     *
     * @param it Defines the starting point of the statement.
     * @param end Defines the ending point of the statement.
     */
    bool parseArguments(Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTOFFORLOOP_H_INCLUDED
