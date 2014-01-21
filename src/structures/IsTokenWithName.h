//
// Copyright (C) 2006s-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ISTOKENWITHNAME_H_INCLUDED
#define ISTOKENWITHNAME_H_INCLUDED

#include "Tokens.h"
#include <functional>

namespace Vera
{
namespace Structures
{

/**
 *  @brief Binary function object class whose call returns whether a item is equal
 *  to the given token.
 */
class IsTokenEqual
: public std::unary_function<const Token, bool>
{
  public:

    /**
     * @brief Initializes a new instance of the IsTokenEqual class.
     * @param token The token desired of the collection.
     */
    IsTokenEqual(Token token)
    : token_(token)
    {
    }

    /**
     * @brief Member function that returns true if the element is equal to the given token.
     *
     * @param token The token to be compare
     *
     * @return True if the item is equal to the given token, otherwise, false.
     */
    result_type operator()(argument_type item) const
    {
      return token_ == item;
    }

  private:

    Token token_;
};

/**
 * @brief Binary function object class whose call returns whether an item contains the given name.
 */
class IsTokenWithName
: public std::unary_function<const Token&, bool>
{
  public:

    /**
     * @brief Initializes a new instance of the IsTokenWithName class.
     * @param name The name desired.
     */
    IsTokenWithName(const std::string& name)
    : name_(name)
    {
    }

    /**
     * @brief Member function that returns true if the element contains the given name.
     *
     * @param token The token to be compare
     *
     * @return True if the item contains the given name, otherwise, false.
     */
    result_type operator()(argument_type item) const
    {
      return name_ == item.name_;
    }

  private:

    std::string name_;
};

/**
 * @brief Binary function object class whose call returns whether a
 * token is valid to build a statement.
 */
class IsValidTokenForStatement
: public std::unary_function<const Token, bool>
{
  public:

    /**
     * @brief Initializes a new instance of the IsValidTokenForStatement class.
     */
    IsValidTokenForStatement(){}

    /**
     * @brief Member function that returns true if the token is valid to build statement.
     *
     * @param token The token to be compare
     *
     * @return True if the token is valid, otherwise, false.
     */
    result_type operator()(argument_type item) const
    {
      if (item.name_.compare("space") == 0)
      {
        return false;
      }
      else if (item.name_.compare("space2") == 0)
      {
        return false;
      }
      else if (item.name_.compare("newline") == 0)
      {
        return false;
      }
      else if (item.name_.compare("ccomment") == 0)
      {
        return false;
      }
      else if (item.name_.compare("cppcomment") == 0)
      {
        return false;
      }

      return true;
    }
};

struct EndsWithCorrectPattern
: public std::unary_function<const Token, bool>
{
  public:

    EndsWithCorrectPattern(const std::string& beginType, const std::string& endType)
    : beginType_(beginType)
    , endType_(endType)
    , braces_(0)
    {
    }

    EndsWithCorrectPattern(const boost::wave::token_id id)
    : braces_(0)
    {
      if (id == boost::wave::T_LEFTPAREN)
      {
        beginType_ = "leftparen";
        endType_ = "rightparen";
      }
      else if (id == boost::wave::T_LEFTBRACE)
      {
        beginType_ = "leftbrace";
        endType_ = "rightbrace";
      }
      else if (id == boost::wave::T_LEFTBRACKET)
      {
        beginType_ = "leftbracket";
        endType_ = "rightbracket";
      }
      else if (id == boost::wave::T_LESS)
      {
        beginType_ = "less";
        endType_ = "greater";
      }
    }

    result_type operator()(argument_type item) const
    {
      bool response = false;

      if (item.name_ == beginType_)
      {
        ++braces_;
      }

      if (item.name_ == endType_)
      {
        if(braces_ == 0)
        {
          response = true;
        }
        else
        {
          --braces_;
        }
      }

      return response;
    }

    std::string beginType_;
    std::string endType_;
    mutable int braces_;
};

} // Strutures namespace
} // Vera namespace

#endif // ISTOKENWITHNAME_H_INCLUDED
