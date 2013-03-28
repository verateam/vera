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


using namespace std;
using namespace Vera;
using namespace Plugins;


namespace // unnamed
{

typedef map<Parameters::ParamName, Parameters::ParamValue> ParametersCollection;

ParametersCollection parameters_;

} // unnamed namespace


void Parameters::set(const ParamAssoc & assoc)
{
    string::size_type pos = assoc.find("=");
    if (pos != string::npos)
    {
        ParamName name = assoc.substr(0, pos);
        ParamValue value = assoc.substr(pos + 1);

        parameters_[name] = value;
    }
    else
    {
        ostringstream ss;
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
    ifstream file(name.c_str());
    if (file.is_open() == false)
    {
        ostringstream ss;
        ss << "cannot open parameters file " << name;
        throw ParametersError(ss.str());
    }

    string line;
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
}
