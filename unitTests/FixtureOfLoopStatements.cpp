#include "FixtureOfLoopStatements.h"

using namespace Vera::Structures;

void
FixtureOfLoopStatements::SetUp()
{
  collectionTokensOfLoopFor_.push_back(Token("for", 1, 0, "for"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 1, 3, "space"));
  collectionTokensOfLoopFor_.push_back(Token("(", 1, 4, "leftparen"));
  collectionTokensOfLoopFor_.push_back(Token("int", 1, 5, "int"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 1, 8, "space"));
  collectionTokensOfLoopFor_.push_back(Token("i", 1, 9, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 1, 10, "space"));
  collectionTokensOfLoopFor_.push_back(Token("=", 1, 11, "assign"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 1, 12, "space"));
  collectionTokensOfLoopFor_.push_back(Token("0", 1, 13, "intlit"));
  collectionTokensOfLoopFor_.push_back(Token(";", 1, 14, "semicolon"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 1, 15, "space"));
  collectionTokensOfLoopFor_.push_back(Token("i", 1, 16, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token("<", 1, 17, "less"));
  collectionTokensOfLoopFor_.push_back(Token("104", 1, 18, "intlit"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 1, 21, "space"));
  collectionTokensOfLoopFor_.push_back(Token("&&", 1, 22, "andand"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 1, 24, "space"));
  collectionTokensOfLoopFor_.push_back(Token("j", 1, 25, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token("%", 1, 26, "percent"));
  collectionTokensOfLoopFor_.push_back(Token("i", 1, 27, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 1, 28, "space"));
  collectionTokensOfLoopFor_.push_back(Token("==", 1, 29, "equal"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 1, 31, "space"));
  collectionTokensOfLoopFor_.push_back(Token("0", 1, 32, "intlit"));
  collectionTokensOfLoopFor_.push_back(Token(";", 1, 33, "semicolon"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 1, 34, "space"));
  collectionTokensOfLoopFor_.push_back(Token("i", 1, 35, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token("++", 1, 36, "plusplus"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 1, 38, "space"));
  collectionTokensOfLoopFor_.push_back(Token(")", 1, 39, "rightparen"));
  collectionTokensOfLoopFor_.push_back(Token("\n", 1, 40, "newline"));
  collectionTokensOfLoopFor_.push_back(Token("  ", 2, 0, "space"));
  collectionTokensOfLoopFor_.push_back(Token("printf", 2, 2, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token("(", 2, 8, "leftparen"));
  collectionTokensOfLoopFor_.push_back(Token("\"ok\"", 2, 9, "stringlit"));
  collectionTokensOfLoopFor_.push_back(Token(")", 2, 13, "rightparen"));
  collectionTokensOfLoopFor_.push_back(Token(";", 2, 14, "semicolon"));
  collectionTokensOfLoopFor_.push_back(Token("   ", 2, 15, "space"));
  collectionTokensOfLoopFor_.push_back(Token("\n", 2, 18, "newline"));
  collectionTokensOfLoopFor_.push_back(Token("\n", 3, 0, "newline"));
  collectionTokensOfLoopFor_.push_back(Token("for", 4, 0, "for"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 3, "space"));
  collectionTokensOfLoopFor_.push_back(Token("(", 4, 4, "leftparen"));
  collectionTokensOfLoopFor_.push_back(Token("int", 4, 5, "int"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 8, "space"));
  collectionTokensOfLoopFor_.push_back(Token("i", 4, 9, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 10, "space"));
  collectionTokensOfLoopFor_.push_back(Token("=", 4, 11, "assign"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 12, "space"));
  collectionTokensOfLoopFor_.push_back(Token("0", 4, 13, "intlit"));
  collectionTokensOfLoopFor_.push_back(Token(",", 4, 14, "comma"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 15, "space"));
  collectionTokensOfLoopFor_.push_back(Token("j", 4, 16, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 17, "space"));
  collectionTokensOfLoopFor_.push_back(Token("=", 4, 18, "assign"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 19, "space"));
  collectionTokensOfLoopFor_.push_back(Token("1", 4, 20, "intlit"));
  collectionTokensOfLoopFor_.push_back(Token(";", 4, 21, "semicolon"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 22, "space"));
  collectionTokensOfLoopFor_.push_back(Token("i", 4, 23, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token("<", 4, 24, "less"));
  collectionTokensOfLoopFor_.push_back(Token("104", 4, 25, "intlit"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 28, "space"));
  collectionTokensOfLoopFor_.push_back(Token("&&", 4, 29, "andand"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 31, "space"));
  collectionTokensOfLoopFor_.push_back(Token("j", 4, 32, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token("%", 4, 33, "percent"));
  collectionTokensOfLoopFor_.push_back(Token("i", 4, 34, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 35, "space"));
  collectionTokensOfLoopFor_.push_back(Token("==", 4, 36, "equal"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 38, "space"));
  collectionTokensOfLoopFor_.push_back(Token("0", 4, 39, "intlit"));
  collectionTokensOfLoopFor_.push_back(Token(";", 4, 40, "semicolon"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 41, "space"));
  collectionTokensOfLoopFor_.push_back(Token("i", 4, 42, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token("++", 4, 43, "plusplus"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 4, 45, "space"));
  collectionTokensOfLoopFor_.push_back(Token(")", 4, 46, "rightparen"));
  collectionTokensOfLoopFor_.push_back(Token("\n", 4, 47, "newline"));
  collectionTokensOfLoopFor_.push_back(Token("{", 5, 0, "leftbrace"));
  collectionTokensOfLoopFor_.push_back(Token("\n", 5, 1, "newline"));
  collectionTokensOfLoopFor_.push_back(Token("   ", 6, 0, "space"));
  collectionTokensOfLoopFor_.push_back(Token("if", 6, 3, "if"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 6, 5, "space"));
  collectionTokensOfLoopFor_.push_back(Token("(", 6, 6, "leftparen"));
  collectionTokensOfLoopFor_.push_back(Token("i", 6, 7, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 6, 8, "space"));
  collectionTokensOfLoopFor_.push_back(Token("%", 6, 9, "percent"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 6, 10, "space"));
  collectionTokensOfLoopFor_.push_back(Token("3", 6, 11, "intlit"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 6, 12, "space"));
  collectionTokensOfLoopFor_.push_back(Token("==", 6, 13, "equal"));
  collectionTokensOfLoopFor_.push_back(Token(" ", 6, 15, "space"));
  collectionTokensOfLoopFor_.push_back(Token("0", 6, 16, "intlit"));
  collectionTokensOfLoopFor_.push_back(Token(")", 6, 17, "rightparen"));
  collectionTokensOfLoopFor_.push_back(Token("\n", 6, 18, "newline"));
  collectionTokensOfLoopFor_.push_back(Token("     ", 7, 0, "space"));
  collectionTokensOfLoopFor_.push_back(Token("j", 7, 5, "identifier"));
  collectionTokensOfLoopFor_.push_back(Token("++", 7, 6, "plusplus"));
  collectionTokensOfLoopFor_.push_back(Token(";", 7, 8, "semicolon"));
  collectionTokensOfLoopFor_.push_back(Token("\n", 7, 9, "newline"));
  collectionTokensOfLoopFor_.push_back(Token("}", 8, 0, "rightbrace"));
  collectionTokensOfLoopFor_.push_back(Token("\n", 8, 1, "newline"));

}

void
FixtureOfLoopStatements::initializeSigleForLoop()
{
  argumentsOfForLoop_.tokenSequence_.push_back(Token("(", 1, 4, "leftparen"));

  {
    argumentsOfForLoop_.statementSequence_.push_back(Statement());
    Statement& current = argumentsOfForLoop_.statementSequence_.back();

    current.tokenSequence_.push_back(Token("int", 1, 5, "int"));
    current.tokenSequence_.push_back(Token(" ", 1, 8, "space"));
    current.tokenSequence_.push_back(Token("i", 1, 9, "identifier"));
    current.tokenSequence_.push_back(Token(" ", 1, 10, "space"));
    current.tokenSequence_.push_back(Token("=", 1, 11, "assign"));
    current.tokenSequence_.push_back(Token(" ", 1, 12, "space"));
    current.tokenSequence_.push_back(Token("0", 1, 13, "intlit"));
    current.tokenSequence_.push_back(Token(";", 1, 14, "semicolon"));
  }

  {
    argumentsOfForLoop_.statementSequence_.push_back(Statement());
    Statement& current = argumentsOfForLoop_.statementSequence_.back();

    current.tokenSequence_.push_back(Token(" ", 1, 15, "space"));
    current.tokenSequence_.push_back(Token("i", 1, 16, "identifier"));
    current.tokenSequence_.push_back(Token("<", 1, 17, "less"));
    current.tokenSequence_.push_back(Token("104", 1, 18, "intlit"));
    current.tokenSequence_.push_back(Token(" ", 1, 21, "space"));
    current.tokenSequence_.push_back(Token("&&", 1, 22, "andand"));
    current.tokenSequence_.push_back(Token(" ", 1, 24, "space"));
    current.tokenSequence_.push_back(Token("j", 1, 25, "identifier"));
    current.tokenSequence_.push_back(Token("%", 1, 26, "percent"));
    current.tokenSequence_.push_back(Token("i", 1, 27, "identifier"));
    current.tokenSequence_.push_back(Token(" ", 1, 28, "space"));
    current.tokenSequence_.push_back(Token("==", 1, 29, "equal"));
    current.tokenSequence_.push_back(Token(" ", 1, 31, "space"));
    current.tokenSequence_.push_back(Token("0", 1, 32, "intlit"));
    current.tokenSequence_.push_back(Token(";", 1, 33, "semicolon"));
  }

  {
    argumentsOfForLoop_.statementSequence_.push_back(Statement());
    Statement& current = argumentsOfForLoop_.statementSequence_.back();

    current.tokenSequence_.push_back(Token(" ", 1, 34, "space"));
    current.tokenSequence_.push_back(Token("i", 1, 35, "identifier"));
    current.tokenSequence_.push_back(Token("++", 1, 36, "plusplus"));
    current.tokenSequence_.push_back(Token(" ", 1, 38, "space"));
  }

  argumentsOfForLoop_.tokenSequence_.push_back(Token(")", 1, 39, "rightparen"));

  scopeOfForLoop_.tokenSequence_.push_back(Token("\n", 1, 40, "newline"));
  scopeOfForLoop_.tokenSequence_.push_back(Token("  ", 2, 0, "space"));

  scopeOfForLoop_.tokenSequence_.push_back(Token("printf", 2, 2, "identifier"));
  {
    scopeOfForLoop_.statementSequence_.push_back(Statement());
    Statement& current = scopeOfForLoop_.statementSequence_.back();
    current.tokenSequence_.push_back(Token("(", 2, 8, "leftparen"));
    current.tokenSequence_.push_back(Token("\"ok\"", 2, 9, "stringlit"));
    current.tokenSequence_.push_back(Token(")", 2, 13, "rightparen"));
  }

  scopeOfForLoop_.tokenSequence_.push_back(Token(";", 2, 14, "semicolon"));

}

void
FixtureOfLoopStatements::initializeComplexForLoop()
{
  argumentsOfForLoop_.tokenSequence_.push_back(Token("(", 4, 4, "leftparen"));

  {
    argumentsOfForLoop_.statementSequence_.push_back(Statement());
    Statement& current = argumentsOfForLoop_.statementSequence_.back();

    current.tokenSequence_.push_back(Token("int", 4, 5, "int"));
    current.tokenSequence_.push_back(Token(" ", 4, 8, "space"));
    current.tokenSequence_.push_back(Token("i", 4, 9, "identifier"));
    current.tokenSequence_.push_back(Token(" ", 4, 10, "space"));
    current.tokenSequence_.push_back(Token("=", 4, 11, "assign"));
    current.tokenSequence_.push_back(Token(" ", 4, 12, "space"));
    current.tokenSequence_.push_back(Token("0", 4, 13, "intlit"));
    current.tokenSequence_.push_back(Token(",", 4, 14, "comma"));
    current.tokenSequence_.push_back(Token(" ", 4, 15, "space"));
    current.tokenSequence_.push_back(Token("j", 4, 16, "identifier"));
    current.tokenSequence_.push_back(Token(" ", 4, 17, "space"));
    current.tokenSequence_.push_back(Token("=", 4, 18, "assign"));
    current.tokenSequence_.push_back(Token(" ", 4, 19, "space"));
    current.tokenSequence_.push_back(Token("1", 4, 20, "intlit"));
    current.tokenSequence_.push_back(Token(";", 4, 21, "semicolon"));
  }

  {
    argumentsOfForLoop_.statementSequence_.push_back(Statement());
    Statement& current = argumentsOfForLoop_.statementSequence_.back();
    current.tokenSequence_.push_back(Token(" ", 4, 22, "space"));
    current.tokenSequence_.push_back(Token("i", 4, 23, "identifier"));
    current.tokenSequence_.push_back(Token("<", 4, 24, "less"));
    current.tokenSequence_.push_back(Token("104", 4, 25, "intlit"));
    current.tokenSequence_.push_back(Token(" ", 4, 28, "space"));
    current.tokenSequence_.push_back(Token("&&", 4, 29, "andand"));
    current.tokenSequence_.push_back(Token(" ", 4, 31, "space"));
    current.tokenSequence_.push_back(Token("j", 4, 32, "identifier"));
    current.tokenSequence_.push_back(Token("%", 4, 33, "percent"));
    current.tokenSequence_.push_back(Token("i", 4, 34, "identifier"));
    current.tokenSequence_.push_back(Token(" ", 4, 35, "space"));
    current.tokenSequence_ .push_back(Token("==", 4, 36, "equal"));
    current.tokenSequence_.push_back(Token(" ", 4, 38, "space"));
    current.tokenSequence_.push_back(Token("0", 4, 39, "intlit"));
    current.tokenSequence_.push_back(Token(";", 4, 40, "semicolon"));
  }

  {
    argumentsOfForLoop_.statementSequence_.push_back(Statement());
    Statement& current = argumentsOfForLoop_.statementSequence_.back();
    current.tokenSequence_.push_back(Token(" ", 4, 41, "space"));
    current.tokenSequence_.push_back(Token("i", 4, 42, "identifier"));
    current.tokenSequence_.push_back(Token("++", 4, 43, "plusplus"));
    current.tokenSequence_.push_back(Token(" ", 4, 45, "space"));
  }

  argumentsOfForLoop_.tokenSequence_.push_back(Token(")", 4, 46, "rightparen"));

  scopeOfForLoop_.tokenSequence_.push_back(Token("\n", 4, 47, "newline"));
  scopeOfForLoop_.tokenSequence_.push_back(Token("{", 5, 0, "leftbrace"));
  scopeOfForLoop_.tokenSequence_.push_back(Token("\n", 5, 1, "newline"));
  scopeOfForLoop_.tokenSequence_.push_back(Token("   ", 6, 0, "space"));

  {
    scopeOfForLoop_.statementSequence_.push_back(Statement());
    Statement& current = scopeOfForLoop_.statementSequence_.back();
    current.tokenSequence_.push_back(Token("if", 6, 3, "if"));
    current.tokenSequence_.push_back(Token(" ", 6, 5, "space"));

    {
      current.statementSequence_.push_back(Statement());
      Statement& currentArguments = current.statementSequence_.back();

      currentArguments.tokenSequence_.push_back(Token("(", 6, 6, "leftparen"));
      currentArguments.tokenSequence_.push_back(Token("i", 6, 7, "identifier"));
      currentArguments.tokenSequence_.push_back(Token(" ", 6, 8, "space"));
      currentArguments.tokenSequence_.push_back(Token("%", 6, 9, "percent"));
      currentArguments.tokenSequence_.push_back(Token(" ", 6, 10, "space"));
      currentArguments.tokenSequence_.push_back(Token("3", 6, 11, "intlit"));
      currentArguments.tokenSequence_.push_back(Token(" ", 6, 12, "space"));
      currentArguments.tokenSequence_.push_back(Token("==", 6, 13, "equal"));
      currentArguments.tokenSequence_.push_back(Token(" ", 6, 15, "space"));
      currentArguments.tokenSequence_.push_back(Token("0", 6, 16, "intlit"));
      currentArguments.tokenSequence_.push_back(Token(")", 6, 17, "rightparen"));
    }

    {
      current.statementSequence_.push_back(Statement());
      Statement& currentScope = current.statementSequence_.back();
      currentScope.tokenSequence_.push_back(Token("\n", 6, 18, "newline"));
      currentScope.tokenSequence_.push_back(Token("     ", 7, 0, "space"));
      currentScope.tokenSequence_.push_back(Token("j", 7, 5, "identifier"));
      currentScope.tokenSequence_.push_back(Token("++", 7, 6, "plusplus"));
      currentScope.tokenSequence_.push_back(Token(";", 7, 8, "semicolon"));
    }
  }
  scopeOfForLoop_.tokenSequence_.push_back(Token("\n", 7, 9, "newline"));
  scopeOfForLoop_.tokenSequence_.push_back(Token("}", 8, 0, "rightbrace"));
}

void
FixtureOfLoopStatements::TearDown()
{
}
