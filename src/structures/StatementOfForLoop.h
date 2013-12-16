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
 * @brief
 */
class StatementOfForLoop
: public StatementsBuilder
{
  public:

    StatementOfForLoop(Statement& statement,
      Tokens::TokenSequence::const_iterator& it,
      Tokens::TokenSequence::const_iterator& end);


  private:

    Statement& statement_;
};

} // namespace Structures

} // namespace Vera

#endif // STATEMENTOFFORLOOP_H_INCLUDED
