#include "FixtureOfDefineStaments.h"
#include "FixtureWithFullDocument.h"
#include "FixtureOfIfStatements.h"

using namespace Vera::Structures;

namespace Testing
{

Vera::Structures::Tokens::TokenSequence
getSubCollection(unsigned offset, unsigned size, const Vera::Structures::Tokens::TokenSequence& collection)
{
  Tokens::TokenSequence response;

  if (collection.size()<offset+size)
    throw std::runtime_error("Error out of range.");

  Tokens::TokenSequence::const_iterator it = collection.begin() + offset;
  Tokens::TokenSequence::const_iterator end = collection.begin() + (offset+size);
  std::copy(it, end ,std::back_inserter<Vera::Structures::Tokens::TokenSequence>(response));
  return response;
}

TEST_F(FixtureWithFullDocument, getStament_givenTokenCollectionAndTheStartingElement_returnAssociatedStatement)
{
  //Arrange
  int offset = 204;
  int size = 8;
  Tokens::TokenSequence expectedCollection = getSubCollection(offset, size, collection_);

  Token intitialToken("int", 34, 4, "int");

  //Act
  Statement response = StatementsBuilder::create(intitialToken, collection_);

  //Asserts
  EXPECT_EQ(size, response.tokenSequence_.size());
  ASSERT_TRUE(response.tokenSequence_ == expectedCollection);
}

TEST_F(FixtureWithFullDocument, getStament_givenTheStartingElementInexistent_returnEmptyCollection)
{
  Token intitialToken("float", 0, 0, "int");

  //Act
  Statement response = StatementsBuilder::create(intitialToken, collection_);

  //Asserts
  EXPECT_EQ(0, response.tokenSequence_.size());
}


TEST_F(FixtureOfDefineStaments, getStament_givenTheStartingElementDefineType_returnAssociatedStatement)
{
  //Arrange
  int offset = 2;
  int size = 4;

  Tokens::TokenSequence expectedCollection = getSubCollection(offset, size, collection_);

  Token intitialToken("#define", 2, 0, "pp_define");

  //Act
  Statement response = StatementsBuilder::create(intitialToken, collection_);
  Statement& first = response.statementSequence_.front();

  //Asserts
  EXPECT_EQ(1, response.tokenSequence_.size());
  EXPECT_EQ(intitialToken, response.tokenSequence_.front());
  ASSERT_TRUE(first.tokenSequence_ == expectedCollection);
}

} // Testing namespace
