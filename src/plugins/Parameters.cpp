//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Parameters.h"
#include <fstream>
#include <sstream>
#include <map>
#include <cstring>
#include <cerrno>


namespace // unnamed
{

typedef std::map<Vera::Plugins::Parameters::ParamName, Vera::Plugins::Parameters::ParamValue>
    ParametersCollection;

ParametersCollection parameters_;

} // unnamed namespace

namespace Vera
{
namespace Plugins
{

void Parameters::set(const ParamAssoc & assoc)
{
    std::string::size_type pos = assoc.find("=");
    if (pos != std::string::npos)
    {
        ParamName name = assoc.substr(0, pos);
        ParamValue value = assoc.substr(pos + 1);

        parameters_[name] = value;
    }
    else
    {
        std::ostringstream ss;
        ss << "Invalid parameter association: " << assoc;
        throw ParametersError(ss.str());
    }
}


Parameters::ParamValue Parameters::get(const ParamName & name, const ParamValue & defaultValue)
{
    ParametersCollection::iterator it = parameters_.find(name);
    if (it != parameters_.end())
    {
        return it->second;
    }
    else
    {
        return defaultValue;
    }
}


void Parameters::readFromFile(const FileName & name)
{
    std::ifstream file(name.c_str());
    if (file.is_open() == false)
    {
        std::ostringstream ss;
        ss << "Cannot open parameters file " << name << ": "
           << strerror(errno);
        throw ParametersError(ss.str());
    }

    std::string line;
    int lineNumber = 0;
    while (getline(file, line))
    {
        ++lineNumber;

        if (line.empty())
        {
            continue;
        }

        if (line[0] != '#')
        {
            set(line);
        }
    }
    if (file.bad())
    {
        throw std::runtime_error(
            "Cannot read from " + name + ": " + strerror(errno));
    }
    file.close();
}

}
}
