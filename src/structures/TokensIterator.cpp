//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "TokensIterator.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <fstream>

namespace Vera
{

namespace Structures
{

TokensIterator::TokensIterator(const TokensIterator& it)
: begin_(it.begin_)
, end_(it.end_)
, size_(it.size_)
, index_(it.index_)
, sub_(it.sub_)
{
}

TokensIterator::TokensIterator(const Tokens::TokenSequence& sequence)
: begin_(sequence.begin())
, end_(sequence.end())
, size_(sequence.size())
, index_(0)
{
}

TokensIterator::~TokensIterator()
{
}

TokensIterator&
TokensIterator::operator =(const TokensIterator& it)
{
  begin_ = it.begin_;
  end_ = it.end_;
  size_ = it.size_;
  index_ = it.index_;
  sub_ = it.sub_;
}

void
TokensIterator::swap(TokensIterator& it)
{
  std::swap(begin_, it.begin_);
  std::swap(end_, it.end_);
  std::swap(size_, it.size_);
  std::swap(index_, it.index_);
  sub_.swap(it.sub_);
}

const Token&
TokensIterator::operator*() const
{
  return *(begin_ + index_);
}

const Token*
TokensIterator::operator->() const
{
  TokenSequenceConstIterator it = (begin_ + index_);
  const Token& ptr = *it;

  return &ptr;
}

TokensIterator&
TokensIterator::operator++()
{

  if (sub_)
  {
    if (sub_->isEnd())
    {
      return sub_->operator++();
    }
  }

  if (index_ < size_)
  {
    ++index_;
  }

  return *this;
}

TokensIterator
TokensIterator::operator++(int)
{
  TokensIterator previus(*this);
  operator ++ ();
  return previus;
}

TokensIterator
TokensIterator::operator+(int n) const
{
  TokensIterator it(*this);

  for (int i = 0; i < n; ++i)
  {
    ++it;
  }

  return it;
}

bool
TokensIterator::operator == (const TokensIterator& it) const
{
  return equal(it);
}

bool
TokensIterator::operator < (const TokensIterator& it) const
{
  return index_ < it.index_;
}

bool
TokensIterator::equal(const TokensIterator& it) const
{
  if (begin_ != it.begin_)
  {
    return false;
  }

  if (end_ != it.end_)
  {
    return false;
  }

  if (index_ != it.index_)
  {
    return false;
  }

  return true;
}

bool
TokensIterator::isEnd() const
{
  return index_ == size_;
}

bool
TokensIterator::operator != (const TokensIterator& it) const
{
  return !equal(it);
}

}
}
