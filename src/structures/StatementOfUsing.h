//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STATEMENTOFUSING_H_INCLUDED
#define STATEMENTOFUSING_H_INCLUDED

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
 * of Statements of modifier access type.
 */
class StatementOfUsing
: public StatementsBuilder
{
  friend class BuilderImpl;
  public:

    StatementOfUsing(Statement& statement,
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);

    static bool isValid(Tokens::TokenSequence::const_iterator it,
        Tokens::TokenSequence::const_iterator end);

    static bool create(Statement& statement,
        Tokens::TokenSequence::const_iterator& it,
        Tokens::TokenSequence::const_iterator& end);

  private:

    void initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end);

  private:

    const std::vector<std::string> names_;
    Statement::StatementPointer types_;
};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTOFUSING_H_INCLUDED
