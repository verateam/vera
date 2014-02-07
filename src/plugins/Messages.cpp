//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Messages.h"
#include <map>
#include <boost/algorithm/string/replace.hpp>
#include <boost/smart_ptr.hpp>
#include <iostream>

namespace // unnamed
{

class IMessage{

  public:
    virtual ~IMessage(){}
    virtual void show(const std::string& content) = 0;
};

class NullMessage : public virtual IMessage{
  public:

    NullMessage(){}

    ~NullMessage(){}

    virtual void
    show(const std::string&) {}

    static std::string name_;

    static boost::shared_ptr<IMessage> create()
    {
      IMessage* instance = new NullMessage();

      return boost::shared_ptr<IMessage>(instance);
    }
};

class StandardMessage : public virtual IMessage{

  public:
    StandardMessage(){}

    ~StandardMessage(){}

    virtual void
    show(const std::string& content)
    {
      std::cerr<<content;
    }

    static std::string name_;

    static boost::shared_ptr<StandardMessage> create()
    {
     return boost::shared_ptr<StandardMessage>(new StandardMessage());
    }
};

} // unnamed namespace

static boost::shared_ptr<StandardMessage> provider;

namespace Vera
{
namespace Plugins
{

void
Message::show(const std::string& content)
{
  if (provider)
  {
    provider->show(content);
  }
}

void
Message::setStandardProvider()
{
  provider = StandardMessage::create();
}


Message::Message()
{
}

}
}
