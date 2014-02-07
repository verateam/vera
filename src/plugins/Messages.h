//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MESSAGES_H_INCLUDED
#define MESSAGES_H_INCLUDED

#include <string>
#include <ostream>
#include <boost/serialization/singleton.hpp>


namespace Vera
{
namespace Plugins
{

class IMessage;

class Message : public boost::serialization::singleton<Message>
{
  friend boost::serialization::singleton<Message>;

  public:

    void show(const std::string& content);

    void setStandardProvider();

  protected:
    Message();
};

} // namespace Plugins

} // namespace Vera

#endif // MESSAGES_H_INCLUDED
