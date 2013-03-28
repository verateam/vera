//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PARAMETERS_H_INCLUDED
#define PARAMETERS_H_INCLUDED

#include <string>
#include <stdexcept>


namespace Vera
{
namespace Plugins
{


class ParametersError : public std::runtime_error
{
public:
    ParametersError(const std::string & msg) : std::runtime_error(msg) {}
};


class Parameters
{
public:
    typedef std::string ParamName;
    typedef std::string ParamValue;
    typedef std::string ParamAssoc;
    typedef std::string FileName;

    static void set(const ParamAssoc & assoc);
    static void readFromFile(const FileName & name);

    static ParamValue get(const ParamName & name, const ParamValue & defaultValue);
};

} // namespace Plugins

} // namespace Vera

#endif // PARAMETERS_H_INCLUDED
