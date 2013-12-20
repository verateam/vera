//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef FIXTUREOFIFSTATEMENTS_H_INCLUDED
#define FIXTUREOFIFSTATEMENTS_H_INCLUDED

#include <gtest/gtest.h>
#include <structures/Statements.h>
#include "Utilities.h"

/**
 * @brief Fixture for the statements of if sentences type.
 */
class FixtureOfIfStatements
: public ::testing::Test
, public Testing::Utilities
{
  protected:

    /**
     * @brief It will be called before each test is run.
     */
    virtual void SetUp();
    /**
     * @brief It will be called after each test is run.
     */
    virtual void TearDown();

  protected:

    Vera::Structures::Tokens::TokenSequence collection_;
    Vera::Structures::Tokens::TokenSequence ifTokens_;
    Vera::Structures::Tokens::TokenSequence elseTokens_;
    Vera::Structures::Statement ifScope_;
    Vera::Structures::Statement elseScope_;
};

#endif // FIXTUREOFIFSTATEMENTS_H_INCLUDED
