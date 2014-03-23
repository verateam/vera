//
// Copyright (C) 2014 Ben Key
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXECUTABLE_PATH_H_INCLUDED
#define EXECUTABLE_PATH_H_INCLUDED

#pragma once

#include <string>

std::string executable_path(const char * argv0);
std::string executable_path_fallback(const char * argv0);

#endif // EXECUTABLE_PATH_H_INCLUDED
