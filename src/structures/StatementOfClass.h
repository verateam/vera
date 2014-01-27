//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STATEMENTOFCLASS_H_INCLUDED
#define STATEMENTOFCLASS_H_INCLUDED

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
 * of Statements of type "Class".
 */
class StatementOfClass
: public StatementsBuilder
{
  public:

    StatementOfClass(Statement& statement,
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    /**
     * @brief Gets the scope of the current sentence.
     *
     * @return The const reference to the Statement structure
     * which contains the associated tokens.
     */
    const Statement& getStatementScope();

    static bool isValid(Tokens::TokenSequence::const_iterator it,
      Tokens::TokenSequence::const_iterator end);

    void initialize();

    bool parseName();

    bool parseHeritage();

    bool parseScope();

    bool parseVariablesFromScopeToSemicolon();

    static bool create(Statement& statement,
        Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

  private:

    const std::string* name_;
    Statement* scope_;
    Statement* hieritance_;
    Statement* variables_;
    Tokens::TokenSequence::const_iterator& it_;
    Tokens::TokenSequence::const_iterator& end_;
};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTOFCLASS_H_INCLUDED
