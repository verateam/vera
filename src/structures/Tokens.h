//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TOKENS_H_INCLUDED
#define TOKENS_H_INCLUDED

#include "SourceFiles.h"
#include <string>
#include <vector>


namespace Vera
{
namespace Structures
{

class TokensError : public std::runtime_error
{
public:
    TokensError(const std::string & msg) : std::runtime_error(msg) {}
};

struct Token
{
    Token(const std::string & v, int ln, int cl, const std::string & n)
        : value_(v), line_(ln), column_(cl), name_(n) {}

    Token()
        : value_(""), line_(0), column_(0), name_("") {}

    bool operator==(Token const& t) const
    {
      return value_ == t.value_
          && line_ == t.line_
          && column_ == t.column_
          && name_ == t.name_;
    }

    std::string value_;
    int line_;
    int column_;
    std::string name_;
};


class Tokens
{
public:
    typedef std::string FileContent;

    typedef std::vector<Token> TokenSequence;

    typedef std::string TokenFilter;

    typedef std::vector<TokenFilter> FilterSequence;

    static void parse(const SourceFiles::FileName & name, const FileContent & src);

    static TokenSequence getTokens(const SourceFiles::FileName & name,
        int fromLine, int fromColumn, int toLine, int toColumn,
        const FilterSequence & filter);
};

} // namespace Structures

} // namespace Vera

#endif // TOKENS_H_INCLUDED
