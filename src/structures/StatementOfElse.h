//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STATEMENTOFELSE_H_INCLUDED
#define STATEMENTOFELSE_H_INCLUDED

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
 * of Statements of type "else".
 */
class StatementOfElse
: public StatementsBuilder
{
  public:
    StatementOfElse(const Statement& statement);

    StatementOfElse(Statement& statement,
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Gets the tokens of the current sentence.
     *
     * @return The const reference to the token collection.
     */
    const Tokens::TokenSequence& getTokens();

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

#endif // STATEMENTOFELSE_H_INCLUDED
