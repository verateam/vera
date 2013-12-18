//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STATEMENTOFWHILELOOP_H_INCLUDED
#define STATEMENTOFWHILELOOP_H_INCLUDED

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
 * of Statements of type "while loop".
 */
class StatementOfWhileLoop
: public StatementsBuilder
{
  public:

    StatementOfWhileLoop(Statement& statement,
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

    void initialize(Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTOFWHILELOOP_H_INCLUDED
