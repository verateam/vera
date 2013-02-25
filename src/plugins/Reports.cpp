//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Reports.h"
#include "Rules.h"
#include <map>
#include <utility>

using namespace std;
using namespace Vera;
using namespace Plugins;


namespace // unnamed
{

// for a single report
typedef pair<Rules::RuleName, Reports::Message> SingleReport;

// for a single file (line number -> single report)
typedef multimap<int, SingleReport> FileMessagesCollection;

// for all reports
typedef map<Reports::FileName, FileMessagesCollection> MessagesCollection;

MessagesCollection messages_;

bool showRules_;
bool vcFormat_;
bool xmlReport_;

} // unnamed namespace


void Reports::setShowRules(bool show)
{
    showRules_ = show;
}

void Reports::setXMLReport(bool xmlReport)
{
    xmlReport_ = xmlReport;
}

void Reports::setVCFormat(bool vc)
{
    vcFormat_ = vc;
}

void Reports::add(const FileName & name, int lineNumber, const Message & msg)
{
    const Rules::RuleName currentRule = Rules::getCurrentRule();

    messages_[name].insert(make_pair(lineNumber, make_pair(currentRule, msg)));
}

void Reports::dumpAll(ostream & os, bool omitDuplicates)
{
    if (xmlReport_)
    {
        dumpAllXML(os, omitDuplicates);
    }
    else
    {
        dumpAllNormal(os, omitDuplicates);
    }
}

void Reports::dumpAllNormal(ostream & os, bool omitDuplicates)
{

    for (MessagesCollection::iterator it = messages_.begin(), end = messages_.end();
         it != end; ++it)
    {
        const FileName & name = it->first;

        FileMessagesCollection & fileMessages = it->second;

        FileMessagesCollection::iterator fit = fileMessages.begin();
        FileMessagesCollection::iterator fend = fileMessages.end();

        int lastLineNumber = 0;
        SingleReport lastReport;
        for ( ; fit != fend; ++fit)
        {
            int lineNumber = fit->first;
            const SingleReport & report = fit->second;
            const Rules::RuleName & rule = report.first;
            const Message & msg = report.second;

            if (omitDuplicates == false ||
                lineNumber != lastLineNumber || report != lastReport)
            {                
                if (showRules_)
                {
                    if (vcFormat_)
                    {
                        os << name << '(' << lineNumber << ") : "
                            << '(' << rule << ") " << msg << '\n';
                    }
                    else
                    {
                        os << name << ':' << lineNumber << ": "
                            << '(' << rule << ") " << msg << '\n';
                    }
                }
                else
                {
                    if (vcFormat_)
                    {
                        os << name << '(' << lineNumber << ") : "
                            << msg << '\n';
                    }
                    else
                    {
                        os << name << ':' << lineNumber << ": "
                            << msg << '\n';
                    }
                }

                lastLineNumber = lineNumber;
                lastReport = report;
            }
        }
    }
}

void Reports::dumpAllXML(ostream & os, bool omitDuplicates)
{
    os << "<vera>\n";

    for (MessagesCollection::iterator it = messages_.begin(), end = messages_.end();
         it != end; ++it)
    {
        const FileName & name = it->first;

        os << "    <file name=\"" << name << "\">\n";

        FileMessagesCollection & fileMessages = it->second;

        FileMessagesCollection::iterator fit = fileMessages.begin();
        FileMessagesCollection::iterator fend = fileMessages.end();

        int lastLineNumber = 0;
        SingleReport lastReport;
        for ( ; fit != fend; ++fit)
        {
            int lineNumber = fit->first;
            const SingleReport & report = fit->second;
            const Rules::RuleName & rule = report.first;
            const Message & msg = report.second;

            if (omitDuplicates == false ||
                lineNumber != lastLineNumber || report != lastReport)
            {
                if (showRules_)
                {
                    os << "        <report rule=\"" << rule
                        << "\" line=\"" << lineNumber
                        << "\">![CDATA[" << msg << "]]</report>\n";
                }
                else
                {
                    os << "        <report line=\"" << lineNumber
                        << "\">![CDATA[" << msg << "]]</report>\n";
                }

                lastLineNumber = lineNumber;
                lastReport = report;
            }
        }

        os << "    </file>\n";
    }

    os << "</vera>\n";
}
