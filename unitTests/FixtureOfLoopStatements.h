//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef FIXTUREOFLOOPSTAMENTS_H_INCLUDED
#define FIXTUREOFLOOPSTAMENTS_H_INCLUDED

#include <gtest/gtest.h>
#include <structures/Statements.h>

/**
 * @brief Fixture for the loop type statements.
 */
class FixtureOfLoopStatements : public ::testing::Test
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

    Vera::Structures::Tokens::TokenSequence collectionTokensOfLoopFor_;
    Vera::Structures::Tokens::TokenSequence collectionTokensOfLoopWhile_;
    Vera::Structures::Tokens::TokenSequence collectionTokensOfLoopDo_;
};

#endif // FIXTUREOFLOOPSTAMENTS_H_INCLUDED
