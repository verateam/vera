#include "FixtureForDefineStaments.h"

using namespace Vera::Structures;

void
FixtureForDefineStaments::SetUp()
{
  collection_.push_back(Token("\n", 1, 0, "newline"));
  collection_.push_back(Token("#define", 2, 0, "pp_define"));
  collection_.push_back(Token(" ", 2, 7, "space"));
  collection_.push_back(Token("OK_", 2, 8, "identifier"));
  collection_.push_back(Token(" ", 2, 11, "space"));
  collection_.push_back(Token("i\"OK\"", 2, 12, "stringlit"));
  collection_.push_back(Token("\n", 2, 16, "newline"));
  collection_.push_back(Token("#define", 3, 0, "pp_define"));
  collection_.push_back(Token(" ", 3, 7, "space"));
  collection_.push_back(Token("OK_", 3, 8, "identifier"));
  collection_.push_back(Token("    ", 3, 11, "space"));
  collection_.push_back(Token("i\"FAIL\"", 3, 15, "stringlit"));
  collection_.push_back(Token("\n", 3, 21, "newline"));
  collection_.push_back(Token("\n", 4, 0, "newline"));
  collection_.push_back(Token("#define", 5, 0, "pp_define"));
  collection_.push_back(Token(" ", 5, 7, "space"));
  collection_.push_back(Token("OK", 5, 8, "identifier"));
  collection_.push_back(Token("(", 5, 10, "leftparen"));
  collection_.push_back(Token(")", 5, 11, "rightparen"));
  collection_.push_back(Token(" ", 5, 12, "space"));
  collection_.push_back(Token("{", 6, 13, "leftbrace"));
  collection_.push_back(Token("   ", 6, 14, "space"));
  collection_.push_back(Token("if", 7, 17, "if"));
  collection_.push_back(Token(" ", 7, 19, "space"));
  collection_.push_back(Token("(", 7, 20, "leftparen"));
  collection_.push_back(Token("OK", 7, 21, "identifier"));
  collection_.push_back(Token(" ", 7, 23, "space"));
  collection_.push_back(Token("==", 7, 24, "equal"));
  collection_.push_back(Token(" ", 7, 26, "space"));
  collection_.push_back(Token("true", 7, 27, "true"));
  collection_.push_back(Token(")", 7, 31, "rightparen"));
  collection_.push_back(Token("   ", 7, 32, "space"));
  collection_.push_back(Token("{", 8, 35, "leftbrace"));
  collection_.push_back(Token("    ", 9, 36, "space"));
  collection_.push_back(Token("std", 9, 40, "identifier"));
  collection_.push_back(Token("::", 9, 43, "colon_colon"));
  collection_.push_back(Token("cout", 9, 45, "identifier"));
  collection_.push_back(Token("<<", 9, 49, "shiftleft"));
  collection_.push_back(Token("OK_", 9, 51, "identifier"));
  collection_.push_back(Token(";", 9, 54, "semicolon"));
  collection_.push_back(Token("   ", 9, 55, "space"));
  collection_.push_back(Token("}", 10, 58, "rightbrace"));
  collection_.push_back(Token("  ", 11, 59, "space"));
  collection_.push_back(Token("else", 11, 61, "else"));
  collection_.push_back(Token("     ", 11, 65, "space"));
  collection_.push_back(Token("std", 12, 70, "identifier"));
  collection_.push_back(Token("::", 12, 73, "colon_colon"));
  collection_.push_back(Token("cout", 12, 75, "identifier"));
  collection_.push_back(Token("<<", 12, 79, "shiftleft"));
  collection_.push_back(Token("FAIL_", 12, 81, "identifier"));
  collection_.push_back(Token(";", 12, 86, "semicolon"));
  collection_.push_back(Token(" ", 12, 87, "space"));
  collection_.push_back(Token("}", 13, 88, "rightbrace"));
  collection_.push_back(Token(" ", 13, 89, "space"));
  collection_.push_back(Token("\n", 13, 90, "newline"));
  collection_.push_back(Token("\n", 14, 0, "newline"));
}

void
FixtureForDefineStaments::TearDown()
{
}
