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
      return name_.compare(item.name_) == 0;
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

} // Strutures namespace
} // Vera namespace

#endif // ISTOKENWITHNAME_H_INCLUDED
