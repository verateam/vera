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
#include <boost/wave/token_ids.hpp>

namespace underlyImpl
{

struct TokenRef
{
    TokenRef(boost::wave::token_id id, int line, int column, int length);

    TokenRef(boost::wave::token_id id, int line, int column, int length, const std::string & value);

    std::string getTokenValue(const Vera::Structures::SourceFiles::FileName & fileName) const;

    boost::wave::token_id id_;
    int line_;
    int column_;
    int length_;

    // if >= 0, it is the index into the physicalTokens collection,
    // used only for line continuation
    // and when line_ and column_ do not reflect the physical layout:
    int index_;
};

} // underlyImpl namespace


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

     /**
      * @brief Gets every token contained in the specified file.
      * @param fileName The name of the file to be parsed.
      * @return The token collection obtained from the specified file.
      */
    static TokenSequence getEachTokenFromFile(const std::string& fileName);
};

/**
 * @brief Const iterator type for the token collection.
 */
typedef Tokens::TokenSequence::const_iterator TokenSequenceConstIterator;

} // namespace Structures

} // namespace Vera

#endif // TOKENS_H_INCLUDED
