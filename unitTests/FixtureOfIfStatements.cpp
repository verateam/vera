#include "FixtureOfIfStatements.h"

using namespace Vera::Structures;

void
FixtureOfIfStatements::SetUp()
{
  collection_.push_back(Token("if", 1, 0, "if"));
  collection_.push_back(Token(" ", 1, 2, "space"));
  collection_.push_back(Token("(", 1, 3, "leftparen"));
  collection_.push_back(Token("true", 1, 4, "stringlit"));
  collection_.push_back(Token(")", 1, 8, "rightparen"));
  collection_.push_back(Token(" ", 1, 9, "space"));
  collection_.push_back(Token("{", 1, 10, "leftbrace"));
  collection_.push_back(Token(" ", 1, 11, "space"));
  collection_.push_back(Token("/*any comment */", 1, 12, "ccomment"));
  collection_.push_back(Token(" ", 1, 28, "space"));


  collection_.push_back(Token("return", 1, 29, "return"));
  collection_.push_back(Token(" ", 1, 35, "space"));
  collection_.push_back(Token("toupper", 1, 36, "identifier"));
  collection_.push_back(Token("(", 1, 43, "leftparen"));
  collection_.push_back(Token("\"true\"", 1, 44, "stringlit"));
  collection_.push_back(Token(")", 1, 50, "rightparen"));
  collection_.push_back(Token(";", 1, 51, "semicolon"));


  collection_.push_back(Token(" ", 1, 52, "space"));
  collection_.push_back(Token("/*other comment */", 1, 53, "ccomment"));
  collection_.push_back(Token(" ", 1, 71, "space"));
  collection_.push_back(Token("}", 1, 72, "rightbrace"));
  collection_.push_back(Token(" ", 1, 73, "space"));
  collection_.push_back(Token("else", 1, 74, "else"));
  collection_.push_back(Token(" ", 1, 78, "space"));
  collection_.push_back(Token("return", 1, 79, "return"));
  collection_.push_back(Token(" ", 1, 85, "space"));
  collection_.push_back(Token("toupper", 1, 86, "identifier"));
  collection_.push_back(Token("(", 1, 93, "leftparen"));
  collection_.push_back(Token("\"false\"", 1, 94, "stringlit"));
  collection_.push_back(Token(")", 1, 102, "rightparen"));
  collection_.push_back(Token(" ", 1, 103, "space"));
  collection_.push_back(Token(";", 1, 104, "semicolon"));
  collection_.push_back(Token(" ", 1, 105, "space"));
  collection_.push_back(Token("//Ok\n", 1, 106, "cppcomment"));

  ifScope_.tokenSequence_.push_back(Token(" ", 1, 9, "space"));
  ifScope_.tokenSequence_.push_back(Token("{", 1, 10, "leftbrace"));
  ifScope_.tokenSequence_.push_back(Token(" ", 1, 11, "space"));
  ifScope_.tokenSequence_.push_back(Token("/*any comment */", 1, 12, "ccomment"));
  ifScope_.tokenSequence_.push_back(Token(" ", 1, 28, "space"));

  {
    ifScope_.statementSequence_.push_back(Statement());
    Statement& returnSentence = ifScope_.statementSequence_.back();
    returnSentence.tokenSequence_.push_back(Token("return", 1, 29, "return"));
    returnSentence.tokenSequence_.push_back(Token(" ", 1, 35, "space"));
    returnSentence.tokenSequence_.push_back(Token("toupper", 1, 36, "identifier"));

    {
      returnSentence.statementSequence_.push_back(Statement());
      Statement& current = returnSentence.statementSequence_.back();

      current.tokenSequence_.push_back(Token("(", 1, 43, "leftparen"));
      current.tokenSequence_.push_back(Token("\"true\"", 1, 44, "stringlit"));
      current.tokenSequence_.push_back(Token(")", 1, 50, "rightparen"));
    }

    returnSentence.tokenSequence_.push_back(Token(";", 1, 51, "semicolon"));
  }


  ifScope_.tokenSequence_.push_back(Token(" ", 1, 52, "space"));
  ifScope_.tokenSequence_.push_back(Token("/*other comment */", 1, 53, "ccomment"));
  ifScope_.tokenSequence_.push_back(Token(" ", 1, 71, "space"));
  ifScope_.tokenSequence_.push_back(Token("}", 1, 72, "rightbrace"));

  ifTokens_.push_back(Token("if", 1, 0, "if"));
  ifTokens_.push_back(Token(" ", 1, 2, "space"));
  ifTokens_.push_back(Token(" ", 1, 73, "space"));
  ifTokens_.push_back(Token("else", 1, 74, "else"));

  elseScope_.tokenSequence_.push_back(Token(" ", 1, 78, "space"));

  {
    elseScope_.statementSequence_.push_back(Statement());
    Statement& returnSentence = elseScope_.statementSequence_.back();
    returnSentence.tokenSequence_.push_back(Token("return", 1, 79, "return"));
    returnSentence.tokenSequence_.push_back(Token(" ", 1, 85, "space"));
    returnSentence.tokenSequence_.push_back(Token("toupper", 1, 86, "identifier"));
    returnSentence.tokenSequence_.push_back(Token(" ", 1, 103, "space"));

    {
      returnSentence.statementSequence_.push_back(Statement());
      Statement& current = returnSentence.statementSequence_.back();

      current.tokenSequence_.push_back(Token("(", 1, 93, "leftparen"));
      current.tokenSequence_.push_back(Token("\"false\"", 1, 94, "stringlit"));
      current.tokenSequence_.push_back(Token(")", 1, 102, "rightparen"));
    }

    returnSentence.tokenSequence_.push_back(Token(";", 1, 104, "semicolon"));
  }

}


void
FixtureOfIfStatements::TearDown()
{
}
