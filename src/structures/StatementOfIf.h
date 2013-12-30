//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STATEMENTOFIF_H_INCLUDED
#define STATEMENTOFIF_H_INCLUDED

#include "Tokens.h"
#include <string>
#include <vector>
#include "Statements.h"
#include "StatementOfElse.h"

namespace Vera
{
namespace Structures
{

/**
 * @brief Classes that implements a decorator dedicated to the construction
 * of Statements of type "for loop".
 */
class StatementOfIf
: public StatementsBuilder
{
  public:

    StatementOfIf(Statement& statement,
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Gets the arguments of the current sentence.
     *
     * @return The const reference to the Statement structure
     * which contains the associated tokens.
     */
    const Statement& getArgumentStatementFromConditionalSentence();

    /**
     * @brief Gets the scope of the 'if' sentence.
     *
     * @return The const reference to the Statement structure
     * which contains the associated tokens.
     */
    const Statement& getStatementIfScope();

    /**
     * @brief Gets the scope of the 'else' sentence.
     *
     * @return The const reference to the Statement structure
     * which contains the associated tokens.
     */
    const Statement& getStatementElse();

    static bool isValid(Tokens::TokenSequence::const_iterator it,
        Tokens::TokenSequence::const_iterator end);


    static bool create(Statement& statement,
        Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

  private:

    void initialize(Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

    bool isElse(Tokens::TokenSequence::const_iterator& it);
};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTOFIF_H_INCLUDED
