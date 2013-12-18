#include "FixtureOfLoopStatements.h"
#include <structures/StatementOfForLoop.h>

using namespace Vera::Structures;

namespace Testing
{
//
TEST_F(FixtureOfLoopStatements, getTokens_givenCollectionWithValidSentenceForLoop_returnStatementWithAssociatedTokens)
{
  //Arrange
  Tokens::TokenSequence inputCollection = getSubCollection(0, 3, collectionTokensOfLoopFor_);
  Tokens::TokenSequence tokensExpected = getSubCollection(0, 2, inputCollection);

  Statement response;
  Tokens::TokenSequence::const_iterator begin = inputCollection.begin();
  Tokens::TokenSequence::const_iterator end = inputCollection.end();

  //Act
  StatementOfForLoop forLoop(response, begin, end);

  //Asserts
  EXPECT_EQ(tokensExpected.size(), response.tokenSequence_.size());
  ASSERT_TRUE(tokensExpected == response.tokenSequence_);
}
//
TEST_F(FixtureOfLoopStatements, getArgumentStatementFromConditionalSentence_givenCollectionWithValidSentenceForLoop_returnStatementAssociatedToConditionalSentence)
{
  //Arrange
  Tokens::TokenSequence inputCollection = getSubCollection(0, 31, collectionTokensOfLoopFor_);

  Statement response;
  Tokens::TokenSequence::const_iterator begin = inputCollection.begin();
  Tokens::TokenSequence::const_iterator end = inputCollection.end();
  initializeSigleForLoop();

  //Act
  StatementOfForLoop forLoop(response, begin, end);

  const Statement& arguments = forLoop.getArgumentStatementFromConditionalSentence();

  //Asserts
  ASSERT_EQ(argumentsOfForLoop_, arguments);
}

TEST_F(FixtureOfLoopStatements, getStatementScope_givenCollectionWithValidSentenceForLoop_returnAssociatedScope)
{
  //Arrange
  Tokens::TokenSequence inputCollection = getSubCollection(0, 46, collectionTokensOfLoopFor_);

  Statement response;
  Tokens::TokenSequence::const_iterator begin = inputCollection.begin();
  Tokens::TokenSequence::const_iterator end = inputCollection.end();
  initializeSigleForLoop();

  //Act
  StatementOfForLoop forLoop(response, begin, end);
  const Statement& scope = forLoop.getStatementScope();

  //Asserts
  ASSERT_EQ(scopeOfForLoop_, scope);
}

TEST_F(FixtureOfLoopStatements, getStatementScope_givenCollectionWithComplexForLoop_returnStatement)
{
  //Arrange
  Tokens::TokenSequence inputCollection = getSubCollection(41, 62, collectionTokensOfLoopFor_);

  Statement response;
  Tokens::TokenSequence::const_iterator begin = inputCollection.begin();
  Tokens::TokenSequence::const_iterator end = inputCollection.end();
  initializeComplexForLoop();

  //Act
  StatementOfForLoop forLoop(response, begin, end);
  const Statement& scope = forLoop.getStatementScope();
  const Statement& arguments = forLoop.getArgumentStatementFromConditionalSentence();

  //Asserts
  EXPECT_EQ(argumentsOfForLoop_, arguments);
  ASSERT_EQ(scopeOfForLoop_, scope);
}

} // Testing namespace
