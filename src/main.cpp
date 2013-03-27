//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "structures/SourceFiles.h"
#include "plugins/Profiles.h"
#include "plugins/Rules.h"
#include "plugins/Exclusions.h"
#include "plugins/Transformations.h"
#include "plugins/Parameters.h"
#include "plugins/Reports.h"
#include "plugins/RootDirectory.h"
#include <iostream>
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

const char *programVersion = "Vera++ 1.1.2 (Community Edition)";

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


int main(int argc, char * argv[])
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

        RootDirectory::DirectoryName veraRoot("/usr/lib/vera++/");

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

            if (arg == "-help")
            {
                cout << "vera++ [options] [list-of-files]\n\n"
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
                cout << programVersion << '\n';
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
                    cerr << "error: option -rule provided with no rule name\n";
                    exit(exitCodeOnFailure);
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
                    cerr << "error: option -profile provided with no profile name\n";
                    exit(exitCodeOnFailure);
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
                    exit(exitCodeOnFailure);
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
                    exit(exitCodeOnFailure);
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
                    exit(exitCodeOnFailure);
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
                    exit(exitCodeOnFailure);
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

                cerr << "error: option " << arg << " not recognized\n";
            }

            ++i;
        }

        if (SourceFiles::empty())
        {
            cerr << "vera++: no input files\n";
            exit(exitCodeOnFailure);
        }

        if (singleTransformation.empty() == false)
        {
            if (singleRule.empty() == false || profile != "default")
            {
                cerr <<
                    "error: transformation cannot be specified together with rules or profiles\n";
                exit(exitCodeOnFailure);
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

        Reports::dumpAll(cerr, omitDuplicates);
    }
    catch (const exception & e)
    {
        cerr << "error: " << e.what() << '\n';
        exit(exitCodeOnFailure);
    }
}
