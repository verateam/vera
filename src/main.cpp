//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//


#include <cstdlib>
#include <string>
#include <boost/algorithm/string.hpp>

int legacy_main(int argc, char * argv[], bool silent = false);

int boost_main(int argc, char * argv[]);

int main(int argc, char * argv[])
{
    char * veraLegacyEnv = getenv("VERA_LEGACY");
    if (veraLegacyEnv != NULL)
    {
        std::string legacy = veraLegacyEnv;
        boost::to_lower(legacy);
        if (legacy == "yes" || legacy == "on" || legacy == "1")
        {
            // use the legacy main, and nothing else
            return legacy_main(argc, argv);
        }
        // use the boost main and nothing else
        return boost_main(argc, argv);
    }
    // look at the arg if we can find some options that looks like legacy
    // options
    int i = 1;
    bool useLegacy = false;
    while (i != argc)
    {
        const std::string arg(argv[i]);
        if (arg == "-version"
            || arg == "-help"
            || arg == "-nofail"
            || arg == "-nodup"
            || arg == "-showrules"
            || arg == "-xmlreport"
            || arg == "-vcformat"
            || arg == "-rule"
            || arg == "-profile"
            || arg == "-exclusions"
            || arg == "-param"
            || arg == "-paramfile"
            || arg == "-transform")
        {
            useLegacy = true;
            break;
        }
        ++i;
    }
    // if legacy main fail, we silently pass to boost main in order to display
    // the new usage message
    if (useLegacy && legacy_main(argc, argv, true) == EXIT_SUCCESS)
    {
        return EXIT_SUCCESS;
    }
    return boost_main(argc, argv);
}
