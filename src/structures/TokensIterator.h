//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TOKENSITERATOR_H_INCLUDED
#define TOKENSITERATOR_H_INCLUDED

#include "Tokens.h"
#include "Document.h"
#include <string>
#include <vector>
#include <boost/smart_ptr.hpp>
#include <iterator>

namespace Vera
{

namespace Structures
{

class TokensIterator
: public std::iterator<std::input_iterator_tag, Token,std::ptrdiff_t,const Token*,const Token&>
{
  public:

    TokensIterator(const TokensIterator& it);

    TokensIterator(const Tokens::TokenSequence& sequence);

    ~TokensIterator();

    TokensIterator& operator = (const TokensIterator& it);

    void swap(TokensIterator& it);

    const Token& operator*() const;

    const Token* operator->() const;

    TokensIterator& operator ++ ();

    TokensIterator operator ++ (int);

    TokensIterator operator + (int n) const;

    bool operator == (const TokensIterator& it) const;

    bool operator < (const TokensIterator& it) const;

    bool equal(const TokensIterator& it) const;

    bool operator != (const TokensIterator& it) const;

  protected:

    bool isEnd() const;

  protected:

    TokenSequenceConstIterator begin_;
    TokenSequenceConstIterator end_;
    std::size_t size_;
    std::size_t index_;
    boost::shared_ptr<TokensIterator> sub_;
};

typedef struct Position_ {} Begin,*End;

template<typename T>
class MakerOfTokensIterator: public TokensIterator
{
  public:
    MakerOfTokensIterator(const Tokens::TokenSequence& sequence);

  private:
    void initialize();
};

typedef struct Initilize_ {} Begin_,*End_;

template<>
class MakerOfTokensIterator<Begin_>: public TokensIterator
{
  public:

    MakerOfTokensIterator(const Tokens::TokenSequence& sequence)
    : TokensIterator(sequence)
    {
      initialize();
    }

  private:

    void initialize()
    {
      index_ = 0;
    }
};


template<>
class MakerOfTokensIterator<End_>: public TokensIterator
{
  public:

    MakerOfTokensIterator(const Tokens::TokenSequence& sequence)
    : TokensIterator(sequence)
    {
      initialize();
    }

  private:

    void initialize()
    {
      index_ = size_;
    }
};


} // namespace Structures

} // namespace Vera

#endif // TOKENSITERATOR_H_INCLUDED
