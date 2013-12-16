#include "FixtureOfDefineStaments.h"

using namespace Vera::Structures;

void
FixtureOfDefineStaments::SetUp()
{
  collection_.push_back(Token("\n", 1, 0, "newline"));
  collection_.push_back(Token("#define", 2, 0, "pp_define"));
  collection_.push_back(Token(" ", 2, 7, "space"));
  collection_.push_back(Token("OK_", 2, 8, "identifier"));
  collection_.push_back(Token(" ", 2, 11, "space"));
  collection_.push_back(Token("\"OK\"", 2, 12, "stringlit"));
  collection_.push_back(Token("\n", 2, 16, "newline"));
  collection_.push_back(Token("#define", 3, 0, "pp_define"));
  collection_.push_back(Token(" ", 3, 7, "space"));
  collection_.push_back(Token("OK_", 3, 8, "identifier"));
  collection_.push_back(Token("    ", 3, 11, "space"));
  collection_.push_back(Token("\"FAIL\"", 3, 15, "stringlit"));
  collection_.push_back(Token("\n", 3, 21, "newline"));
  collection_.push_back(Token("\n", 4, 0, "newline"));
  collection_.push_back(Token("#define", 5, 0, "pp_define"));
  collection_.push_back(Token(" ", 5, 7, "space"));
  collection_.push_back(Token("PRINT", 5, 8, "identifier"));
  collection_.push_back(Token("(", 5, 13, "leftparen"));
  collection_.push_back(Token("OK", 5, 14, "identifier"));
  collection_.push_back(Token(")", 5, 16, "rightparen"));
  collection_.push_back(Token(" ", 5, 17, "space"));
  collection_.push_back(Token("{", 6, 18, "leftbrace"));
  collection_.push_back(Token("   ", 6, 19, "space"));
  collection_.push_back(Token("if", 7, 22, "if"));
  collection_.push_back(Token(" ", 7, 24, "space"));
  collection_.push_back(Token("(", 7, 25, "leftparen"));
  collection_.push_back(Token("OK", 7, 26, "identifier"));
  collection_.push_back(Token(" ", 7, 28, "space"));
  collection_.push_back(Token("==", 7, 29, "equal"));
  collection_.push_back(Token(" ", 7, 31, "space"));
  collection_.push_back(Token("true", 7, 32, "true"));
  collection_.push_back(Token(")", 7, 36, "rightparen"));
  collection_.push_back(Token("   ", 7, 37, "space"));
  collection_.push_back(Token("{", 8, 40, "leftbrace"));
  collection_.push_back(Token("    ", 9, 41, "space"));
  collection_.push_back(Token("std", 9, 45, "identifier"));
  collection_.push_back(Token("::", 9, 48, "colon_colon"));
  collection_.push_back(Token("cout", 9, 50, "identifier"));
  collection_.push_back(Token("<<", 9, 54, "shiftleft"));
  collection_.push_back(Token("OK_", 9, 56, "identifier"));
  collection_.push_back(Token(";", 9, 59, "semicolon"));
  collection_.push_back(Token("   ", 9, 60, "space"));
  collection_.push_back(Token("}", 10, 63, "rightbrace"));
  collection_.push_back(Token("  ", 11, 64, "space"));
  collection_.push_back(Token("else", 11, 66, "else"));
  collection_.push_back(Token("     ", 11, 70, "space"));
  collection_.push_back(Token("std", 12, 75, "identifier"));
  collection_.push_back(Token("::", 12, 78, "colon_colon"));
  collection_.push_back(Token("cout", 12, 80, "identifier"));
  collection_.push_back(Token("<<", 12, 84, "shiftleft"));
  collection_.push_back(Token("FAIL_", 12, 86, "identifier"));
  collection_.push_back(Token(";", 12, 91, "semicolon"));
  collection_.push_back(Token(" ", 12, 92, "space"));
  collection_.push_back(Token("}", 13, 93, "rightbrace"));
  collection_.push_back(Token(" ", 13, 94, "space"));
  collection_.push_back(Token("\n", 13, 95, "newline"));
  collection_.push_back(Token("\n", 14, 0, "newline"));
}

void
FixtureOfDefineStaments::TearDown()
{
}
