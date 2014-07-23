//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "config.h"
#include "structures/SourceFiles.h"
#include "plugins/Profiles.h"
#include "plugins/Rules.h"
#include "plugins/Exclusions.h"
#include "plugins/Transformations.h"
#include "plugins/Parameters.h"
#include "plugins/Reports.h"
#include "plugins/RootDirectory.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>
#include "get_vera_root_default.h"

namespace // unnamed
{

// helper function that checks whether the given file name names the C or C++ source file
bool isSourceFileName(const Vera::Structures::SourceFiles::FileName & name)
{
    const std::string suffixes[] =
        { ".cpp", ".cxx", ".cc", ".c", ".C", ".h", ".hh", ".hpp", ".hxx", ".ipp" };

    const int numOfSuffixes = sizeof(suffixes) / sizeof(std::string);
    for (int i = 0; i != numOfSuffixes; ++i)
    {
        const std::string suf = suffixes[i];
        const Vera::Structures::SourceFiles::FileName::size_type pos = name.rfind(suf);

        if (pos != Vera::Structures::SourceFiles::FileName::npos &&
            pos == name.size() - suf.size())
        {
            return true;
        }
    }

    return false;
}

} // unnamed namespace


int legacy_main(int argc, char * argv[], bool silent = false)
{
    int exitCodeOnFailure = EXIT_FAILURE;

    try
    {
        Vera::Plugins::Profiles::ProfileName profile("default");

        // the directory containing the profile and rule definitions
        // by default it is (in this order, first has highest precedence):
        // - VERA_ROOT (if VERA_ROOT is defined)
        // - HOME/.vera++ (if HOME is defined)
        // - current directory (if scripts and profile are present)
        // - /usr/lib/vera++/ default debian directory

        Vera::Plugins::RootDirectory::DirectoryName veraRoot(get_vera_root_default(argv[0]));

        struct stat St;
        bool isInCurrent = ( (stat( "./scripts", &St ) == 0 || stat( "./rules", &St ) == 0
                                 || stat( "./transforms", &St ) == 0)
                             && stat( "./profiles", &St ) == 0 );

        // scripts and profiles folders are inside current directory
        if (isInCurrent)
        {
            // we can override /usr/lib/vera++
            veraRoot = ".";
        }
        char * veraRootEnv = getenv("HOME");
        if (veraRootEnv != NULL)
        {
          Vera::Plugins::RootDirectory::DirectoryName veraRootTmp(veraRootEnv);
          veraRootTmp += "/.vera++";
          bool isInHome = ( stat( veraRootTmp.c_str(), &St ) == 0 );
            if (isInHome)
            {
                // We assume that if the user has a .vera++ folder in
                // their home then we can override the current
                // directory
                // We don't want to override the current directory
                // only because $HOME is defined.
                veraRoot = veraRootEnv;
                veraRoot += "/.vera++";
            }
        }
        veraRootEnv = getenv("VERA_ROOT");
        if (veraRootEnv != NULL)
        {
            veraRoot = veraRootEnv;
        }

        Vera::Plugins::RootDirectory::setRootDirectory(veraRoot);

        // collect all source file names and interpret options

        Vera::Plugins::Rules::RuleName singleRule;
        Vera::Plugins::Transformations::TransformationName singleTransformation;

        bool omitDuplicates = false;

        int i = 1;
        while (i != argc)
        {
            const std::string arg(argv[i]);

            if (arg == "-help")
            {
                std::cout << "vera++ [options] [list-of-files]\n\n"
                    "This command line interface is deprecated and is planned to be removed.\n\n"
                    "Recognized options:\n\n"
                    "-                  (a single minus sign) indicates that the list of\n"
                    "                   source file names will be provided on the stdin.\n\n"
                    "-exclusions file   read exclusions from file\n\n"
                    "-help              print this message\n\n"
                    "-nofail            do not fail even when finding rule violations\n\n"
                    "-nodup             do not duplicate messages if a single rule is violated\n"
                    "                   many times in a single line of code\n\n"
                    "-profile name      execute all rules from the given profile\n\n"
                    "-param name=value  provide parameters to scripts (can be used many times)\n\n"
                    "-paramfile file    read parameters from file\n\n"
                    "-rule name         execute the given rule\n"
                    "                   (note: the .tcl extension is added automatically)\n\n"
                    "-showrules         include rule name in each report\n\n"
                    "-vcformat          report in Visual C++ format\n\n"
                    "-xmlreport         produce report in the XML format\n\n"
                    "-transform name    execute the given transformation\n\n"
                    "-version           print version number\n\n";
                exit(EXIT_SUCCESS);
            }
            else if (arg == "-version")
            {
                std::cout << VERA_VERSION << '\n';
                exit(EXIT_SUCCESS);
            }
            else if (arg == "-nofail")
            {
                exitCodeOnFailure = EXIT_SUCCESS;
            }
            else if (arg == "-nodup")
            {
                omitDuplicates = true;
            }
            else if (arg == "-")
            {
                // list of source files is provided on stdin
                Vera::Structures::SourceFiles::FileName name;
                while (std::cin >> name)
                {
                    Vera::Structures::SourceFiles::addFileName(name);
                }
            }
            else if (arg == "-showrules")
            {
                Vera::Plugins::Reports::setShowRules(true);
            }
            else if (arg == "-xmlreport")
            {
                Vera::Plugins::Reports::setXMLReport(true);
            }
            else if (arg == "-vcformat")
            {
                Vera::Plugins::Reports::setVCFormat(true);
            }
            else if (arg == "-rule")
            {
                ++i;
                if (argv[i] != NULL)
                {
                    singleRule = argv[i];
                }
                else
                {
                    if (silent == false)
                    {
                        std::cerr << "error: option -rule provided with no rule name\n";
                    }
                    return exitCodeOnFailure;
                }
            }
            else if (arg == "-profile")
            {
                ++i;
                if (argv[i] != NULL)
                {
                    profile = argv[i];
                }
                else
                {
                    if (silent == false)
                    {
                        std::cerr << "error: option -profile provided with no profile name\n";
                    }
                    return exitCodeOnFailure;
                }
            }
            else if (arg == "-exclusions")
            {
                ++i;
                if (argv[i] != NULL)
                {
                    Vera::Plugins::Exclusions::ExclusionFileName file(argv[i]);
                    Vera::Plugins::Exclusions::setExclusions(file);
                }
                else
                {
                    if (silent == false)
                    {
                        std::cerr << "error: option -exclusions provided without name of file\n";
                    }
                    return exitCodeOnFailure;
                }
            }
            else if (arg == "-param")
            {
                ++i;
                if (argv[i] != NULL)
                {
                    Vera::Plugins::Parameters::ParamAssoc assoc(argv[i]);
                    Vera::Plugins::Parameters::set(assoc);
                }
                else
                {
                    if (silent == false)
                    {
                        std::cerr << "error: option -param provided without name and value\n";
                    }
                    return exitCodeOnFailure;
                }
            }
            else if (arg == "-paramfile")
            {
                ++i;
                if (argv[i] != NULL)
                {
                    Vera::Plugins::Parameters::FileName file(argv[i]);
                    Vera::Plugins::Parameters::readFromFile(file);
                }
                else
                {
                    if (silent == false)
                    {
                        std::cerr << "error: option -paramfile provided without name of file\n";
                    }
                    return exitCodeOnFailure;
                }
            }
            else if (arg == "-transform")
            {
                ++i;
                if (argv[i] != NULL)
                {
                    singleTransformation = argv[i];
                }
                else
                {
                    if (silent == false)
                    {
                        std::cerr
                            << "error: option -transform provided without name of transformation\n";
                    }
                    return exitCodeOnFailure;
                }
            }
            else if (isSourceFileName(arg))
            {
                Vera::Structures::SourceFiles::addFileName(arg);
            }
            else
            {
                // the option was not recognized as a name of the source file
                // or a vera-specific option
                if (silent == false)
                {
                    std::cerr << "error: option " << arg << " not recognized\n";
                }
                return EXIT_FAILURE;
            }

            ++i;
        }

        if (Vera::Structures::SourceFiles::empty())
        {
            if (silent == false)
            {
                std::cerr << "vera++: no input files\n";
            }
            return exitCodeOnFailure;
        }

        if (singleTransformation.empty() == false)
        {
            if (singleRule.empty() == false || profile != "default")
            {
                if (silent == false)
                {
                    std::cerr << "error: "
                        "transformation cannot be specified together with rules or profiles\n";
                }
                return exitCodeOnFailure;
            }

            Vera::Plugins::Transformations::executeTransformation(singleTransformation);
        }
        else if (singleRule.empty() == false)
        {
            // single rule requested
            Vera::Plugins::Rules::executeRule(singleRule);
        }
        else
        {
            Vera::Plugins::Profiles::executeProfile(profile);
        }

        Vera::Plugins::Reports::dumpAll(std::cerr, omitDuplicates);
    }
    catch (const std::exception & e)
    {
        std::cerr << "error: " << e.what() << '\n';
        exit(exitCodeOnFailure);
    }
    return 0;
}
