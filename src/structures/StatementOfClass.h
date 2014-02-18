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

    static bool isValid(Tokens::TokenSequence::const_iterator it,
      Tokens::TokenSequence::const_iterator end);

    void initialize();

    bool parseName();

    bool parseHeritage();

    bool parseScope();

    const std::string& getName();

    std::size_t getId();

    bool parseVariablesFromScopeToSemicolon();

    static bool create(Statement& statement,
        Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

  private:

    std::string name_;
    std::size_t id_;
    Statement::StatementPointer scope_;
    Statement::StatementPointer hieritance_;
    Statement::StatementPointer variables_;
    Tokens::TokenSequence::const_iterator& it_;
    Tokens::TokenSequence::const_iterator& end_;
};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTOFCLASS_H_INCLUDED
