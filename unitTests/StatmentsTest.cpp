#include "FixtureForDefineStaments.h"
#include "FixtureWithFullDocument.h"

using namespace Vera::Structures;

TEST_F(FixtureWithFullDocument, getTokensOfStament_givenTokenCollectionAndTheStartingElement_returnStatementAssociated)
{
  //Arrange
  int offset = 206;
  int size = 5;
  Tokens::TokenSequence::const_iterator it = collection_.begin() + offset;
  Tokens::TokenSequence::const_iterator end = collection_.begin() + (offset+size);

  Token intitialToken("int", 34, 4, "int");

  //Act
  Statement response = Statements::getTokensOfStament(intitialToken, collection_);

  //Asserts
  EXPECT_EQ(size, response.tokenSequence_.size());
  ASSERT_TRUE(std::equal(it, end, response.tokenSequence_.begin()));
}

TEST_F(FixtureWithFullDocument, getTokensOfStament_givenTheStartingElementInexistent_returnEmptyCollection)
{
  Token intitialToken("float", 0, 0, "int");

  //Act
  Statement response = Statements::getTokensOfStament(intitialToken, collection_);

  //Asserts
  EXPECT_EQ(0, response.tokenSequence_.size());
}

TEST_F(FixtureForDefineStaments, getTokensOfStament_givenTheStartingElementDefineType_returnCollection)
{
  //Arrange
  int offset = 1;
  int size = 12;
  Tokens::TokenSequence::const_iterator it = collection_.begin() + offset;
  Tokens::TokenSequence::const_iterator end = collection_.begin() + (offset+size);
  Token intitialToken("#define", 2, 0, "pp_define");

  //Act
  Statement response = Statements::getTokensOfStament(intitialToken, collection_);

  //Asserts
  EXPECT_EQ(size, response.tokenSequence_.size());
  ASSERT_TRUE(std::equal(it, end, response.tokenSequence_.begin()));
}
