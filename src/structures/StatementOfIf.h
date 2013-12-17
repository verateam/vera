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

namespace Vera
{
namespace Structures
{

/**
 * @brief TODO
 */
class StatementOfIf
: public StatementsBuilder
{
  public:

    StatementOfIf(Statement& statement,
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    const Tokens::TokenSequence& getIfTokens();
    const Statement& getArgumentStatementFromConditionalSentence();
    const Statement& getStatementIfScope();
    const Statement& getStatementElseScope();

  private:

    void initialize(Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

    void parseArguments(Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    void parseScope(Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    bool isElse(Tokens::TokenSequence::const_iterator& it);

    void addEachInvalidToken(Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end,
      Tokens::TokenSequence& current);
};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTOFIF_H_INCLUDED
