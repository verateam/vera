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

using namespace std;
using namespace Vera;
using namespace Structures;
using namespace Plugins;


namespace // unnamed
{

// helper function that checks whether the given file name names the C or C++ source file
bool isSourceFileName(const SourceFiles::FileName & name)
{
    const string suffixes[] =
        { ".cpp", ".cxx", ".cc", ".c", ".C", ".h", ".hh", ".hpp", ".hxx", ".ipp" };

    const int numOfSuffixes = sizeof(suffixes) / sizeof(string);
    for (int i = 0; i != numOfSuffixes; ++i)
    {
        const string suf = suffixes[i];
        const SourceFiles::FileName::size_type pos = name.rfind(suf);

        if (pos != SourceFiles::FileName::npos &&
            pos == name.size() - suf.size())
        {
            return true;
        }
    }

    return false;
}

} // unnamed namespace


int legacy_main(int argc, char * argv[])
{
    int exitCodeOnFailure = EXIT_FAILURE;

    try
    {
        Profiles::ProfileName profile("default");

        // the directory containing the profile and rule definitions
        // by default it is (in this order, first has highest precedence):
        // - VERA_ROOT (if VERA_ROOT is defined)
        // - HOME/.vera++ (if HOME is defined)
        // - current directory (if scripts and profile are present)
        // - /usr/lib/vera++/ default debian directory

        RootDirectory::DirectoryName veraRoot(VERA_INSTALL_DIR "/lib/vera++/");

        struct stat St;
        bool isInCurrent = ( stat( "./scripts", &St ) == 0
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
          RootDirectory::DirectoryName veraRootTmp(veraRootEnv);
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

        RootDirectory::setRootDirectory(veraRoot);

        // collect all source file names and interpret options

        Rules::RuleName singleRule;
        Transformations::TransformationName singleTransformation;

        bool omitDuplicates = false;

        int i = 1;
        while (i != argc)
        {
            const string arg(argv[i]);

            if (arg == "-version")
            {
                cout << VERA_VERSION << '\n';
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
                SourceFiles::FileName name;
                while (cin >> name)
                {
                    SourceFiles::addFileName(name);
                }
            }
            else if (arg == "-showrules")
            {
                Reports::setShowRules(true);
            }
            else if (arg == "-xmlreport")
            {
                Reports::setXMLReport(true);
            }
            else if (arg == "-vcformat")
            {
                Reports::setVCFormat(true);
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
                    // cerr << "error: option -rule provided with no rule name\n";
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
                    // cerr << "error: option -profile provided with no profile name\n";
                    return exitCodeOnFailure;
                }
            }
            else if (arg == "-exclusions")
            {
                ++i;
                if (argv[i] != NULL)
                {
                    Exclusions::ExclusionFileName file(argv[i]);
                    Exclusions::setExclusions(file);
                }
                else
                {
                    cerr << "error: option -exclusions provided without name of file\n";
                    return exitCodeOnFailure;
                }
            }
            else if (arg == "-param")
            {
                ++i;
                if (argv[i] != NULL)
                {
                    Parameters::ParamAssoc assoc(argv[i]);
                    Parameters::set(assoc);
                }
                else
                {
                    cerr << "error: option -param provided without name and value\n";
                    return exitCodeOnFailure;
                }
            }
            else if (arg == "-paramfile")
            {
                ++i;
                if (argv[i] != NULL)
                {
                    Parameters::FileName file(argv[i]);
                    Parameters::readFromFile(file);
                }
                else
                {
                    cerr << "error: option -paramfile provided without name of file\n";
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
                    cerr << "error: option -transform provided without name of transformation\n";
                    return exitCodeOnFailure;
                }
            }
            else if (isSourceFileName(arg))
            {
                SourceFiles::addFileName(arg);
            }
            else
            {
                // the option was not recognized as a name of the source file
                // or a vera-specific option

                // cerr << "error: option " << arg << " not recognized\n";
                return EXIT_FAILURE;
            }

            ++i;
        }

        if (SourceFiles::empty())
        {
            // cerr << "vera++: no input files\n";
            return exitCodeOnFailure;
        }

        if (singleTransformation.empty() == false)
        {
            if (singleRule.empty() == false || profile != "default")
            {
                cerr <<
                    "error: transformation cannot be specified together with rules or profiles\n";
                return exitCodeOnFailure;
            }

            Transformations::executeTransformation(singleTransformation);
        }
        else if (singleRule.empty() == false)
        {
            // single rule requested
            Rules::executeRule(singleRule);
        }
        else
        {
            Profiles::executeProfile(profile);
        }

        Reports::dumpAll(cout, omitDuplicates);
    }
    catch (const exception & e)
    {
        cerr << "error: " << e.what() << '\n';
        exit(exitCodeOnFailure);
    }
    return 0;
}
