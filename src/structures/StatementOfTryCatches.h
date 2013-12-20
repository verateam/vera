//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STATEMENTOFTRYCATCHES_H_INCLUDED
#define STATEMENTOFTRYCATCHES_H_INCLUDED

#include "Tokens.h"
#include <string>
#include <vector>
#include "Statements.h"
#include "StatementOfCatch.h"

namespace Vera
{
namespace Structures
{

/**
 * @brief Classes that implements a decorator dedicated to the construction
 * of Statements of type "try catch".
 */
class StatementOfTryCatches
: public StatementsBuilder
{
  public:
    typedef std::vector<Statement*> listOfCatchSentences;
    typedef listOfCatchSentences::const_iterator     iteratorOfCatchStatements;

    StatementOfTryCatches(Statement& statement,
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);




    iteratorOfCatchStatements begin();

    iteratorOfCatchStatements end();

    /**
     * @brief Gets the scope of the current sentence.
     *
     * @return The const reference to the Statement structure
     * which contains the associated tokens.
     */
    const Statement& getStatementScope();

    /**
     * @brief Gets the scope of the current sentence.
     *
     * @return The const reference to the Statement structure
     * which contains the associated tokens.
     */
    const Statement& getStatementScopeOfTry();

  private:

    void initialize(Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

  private:

    listOfCatchSentences collection_;

};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTOFTRYCATCHES_H_INCLUDED
