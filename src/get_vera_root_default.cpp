//
// Copyright (C) 2014 Ben Key
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <stdio.h>
#include <stdlib.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "get_vera_root_default.h"
#include "executable_path.h"

std::string get_vera_install_dir(const char *argv0)
{
    boost::filesystem::path veraPath(executable_path(argv0));
    veraPath = veraPath.remove_filename();
    boost::filesystem::path fileName = veraPath.filename();
    if (fileName.string() == "bin")
    {
        veraPath = veraPath.remove_filename();
    }
    return veraPath.string();
}

std::string get_vera_root_default(const char *argv0)
{
    boost::filesystem::path veraRoot(get_vera_install_dir(argv0));
    veraRoot /= "lib";
    veraRoot /= "vera++";
    return veraRoot.string();
}
