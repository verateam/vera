//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Interpreter.h"
#include "Exclusions.h"
#include "Reports.h"
#include "Parameters.h"
#include "../structures/SourceFiles.h"
#include "../structures/SourceLines.h"
#include "../structures/Tokens.h"
#include "cpptcl-1.1.4/cpptcl.h"
#include <fstream>
#include <iterator>

using namespace std;
using namespace Vera;
using namespace Plugins;
using namespace Structures;
using namespace Tcl;


namespace // unnamed
{

// helper global pointer
// - for those functions that might modify the interpreter's state

interpreter *pInter;

void report(const string & fileName, int lineNumber, const string & message)
{
    Reports::add(fileName, lineNumber, message);
}

string getParameter(const string & name, const string & defaultValue)
{
    return Parameters::get(name, defaultValue);
}

object getSourceFileNames()
{
    object obj;

    const SourceFiles::FileNameSet & files = SourceFiles::getAllFileNames();

    typedef SourceFiles::FileNameSet::const_iterator iterator;
    const iterator end = files.end();
    for (iterator it = files.begin(); it != end; ++it)
    {
        const SourceFiles::FileName & name = *it;

        if (Exclusions::isExcluded(name) == false)
        {
            obj.append(*pInter, object(name));
        }
    }

    return obj;
}

int getLineCount(const string & sourceName)
{
    return SourceLines::getLineCount(sourceName);
}

string getLine(const string & sourceName, int lineNumber)
{
    return SourceLines::getLine(sourceName, lineNumber);
}

object getAllLines(const string & sourceName)
{
    object obj;

    const SourceLines::LineCollection & lines = SourceLines::getAllLines(sourceName);

    typedef SourceLines::LineCollection::const_iterator iterator;
    const iterator end = lines.end();
    for (iterator it = lines.begin(); it != end; ++it)
    {
        obj.append(*pInter, object(*it));
    }

    return obj;
}

object getTokens(const string & sourceName, int fromLine, int fromColumn,
    int toLine, int toColumn, const object & filter)
{
    Tokens::FilterSequence filterSeq;

    size_t filterLength = filter.length(*pInter);
    for (size_t i = 0; i != filterLength; ++i)
    {
        filterSeq.push_back(filter.at(*pInter, i).get());
    }

    Tokens::TokenSequence tokenSeq = Tokens::getTokens(sourceName,
        fromLine, fromColumn, toLine, toColumn, filterSeq);

    object ret;
    Tokens::TokenSequence::iterator it = tokenSeq.begin();
    Tokens::TokenSequence::iterator end = tokenSeq.end();
    for ( ; it != end; ++it)
    {
        object singleToken;
        singleToken.append(*pInter, object(it->value_));
        singleToken.append(*pInter, object(it->line_));
        singleToken.append(*pInter, object(it->column_));
        singleToken.append(*pInter, object(it->name_));

        ret.append(*pInter, singleToken);
    }

    return ret;
}

void registerCommands(interpreter & inter)
{
    pInter = &inter;

    // commands related to source files and plain source code
    inter.def("report", report);
    inter.def("getParameter", getParameter);
    inter.def("getSourceFileNames", getSourceFileNames);
    inter.def("getLineCount", getLineCount);
    inter.def("getLine", getLine);
    inter.def("getAllLines", getAllLines);
    inter.def("getTokens", getTokens);
}

} // unnamed namespace


void Interpreter::execute(const DirectoryName & root,
    ScriptType type, const ScriptName & name)
{
    string fileName = root + "/scripts/";
    switch (type)
    {
    case rule:
        fileName += "rules/";
        break;
    case transformation:
        fileName += "transformations/";
        break;
    }

    fileName += name;
    fileName += ".tcl";

    ifstream scriptFile(fileName.c_str());
    if (scriptFile.is_open() == false)
    {
        ostringstream ss;
        ss << "cannot open script " << fileName;
        throw ScriptError(ss.str());
    }

    string scriptBody;
    scriptBody.assign(istreambuf_iterator<char>(scriptFile), istreambuf_iterator<char>());

    interpreter inter;
    registerCommands(inter);
    inter.eval(scriptBody);
}
