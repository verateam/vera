//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Reports.h"
#include "Rules.h"
#include "Exclusions.h"
#include <sstream>
#include <map>
#include <utility>
#include <stdexcept>
#include <boost/algorithm/string/replace.hpp>


namespace // unnamed
{

// for a single report
typedef std::pair<Vera::Plugins::Rules::RuleName, Vera::Plugins::Reports::Message> SingleReport;

// for a single file (line number -> single report)
typedef std::multimap<int, SingleReport> FileMessagesCollection;

// for all reports
typedef std::map<Vera::Plugins::Reports::FileName, FileMessagesCollection> MessagesCollection;

MessagesCollection messages_;

bool showRules_;
bool vcFormat_;
bool xmlReport_;
std::string prefix_;

} // unnamed namespace

namespace Vera
{
namespace Plugins
{

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

void Reports::setPrefix(std::string prefix)
{
    prefix_ = prefix;
}

int Reports::count()
{
    return messages_.size();
}

void Reports::add(const FileName & name, int lineNumber, const Message & msg)
{
    if (lineNumber <= 0)
    {
      std::ostringstream ss;
      ss << "Line number out of range: " << lineNumber;
      throw std::out_of_range(ss.str());
    }
    const Rules::RuleName currentRule = Rules::getCurrentRule();
    if (Exclusions::isExcluded(name, lineNumber, currentRule, msg) == false)
    {
        messages_[name].insert(make_pair(lineNumber, make_pair(currentRule, msg)));
    }
}

void Reports::internal(const FileName & name, int lineNumber, const Message & msg)
{
    const Rules::RuleName currentRule = "vera++ internal";

    messages_[name].insert(make_pair(lineNumber, make_pair(currentRule, msg)));
}

void Reports::dumpAll(std::ostream & os, bool omitDuplicates)
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

void Reports::dumpAllNormal(std::ostream & os, bool omitDuplicates)
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

void Reports::dumpAllXML(std::ostream & os, bool omitDuplicates)
{
    writeXml(os, omitDuplicates);
}

void Reports::writeStd(std::ostream & os, bool omitDuplicates)
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
                os << name;
                os << ':' << lineNumber << ":";
                if (prefix_ != "")
                {
                    os << " " << prefix_;
                }
                if (showRules_)
                {
                    os << " " << rule;
                }
                if (showRules_ || prefix_ != "")
                {
                    os << ":";
                }
                os << " " << msg << std::endl;

                lastLineNumber = lineNumber;
                lastReport = report;
            }
        }
    }
}

void Reports::writeVc(std::ostream & os, bool omitDuplicates)
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
                os << name;
                os << '(' << lineNumber << "):";
                if (prefix_ != "")
                {
                    os << " " << prefix_;
                }
                if (showRules_)
                {
                    os << " " << rule;
                }
                if (showRules_ || prefix_ != "")
                {
                    os << ":";
                }
                os << " " << msg << std::endl;

                lastLineNumber = lineNumber;
                lastReport = report;
            }
        }
    }
}

void Reports::writeXml(std::ostream & os, bool omitDuplicates)
{
    os<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
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
                    os << "        <report rule=\"" << xmlEscape(rule)
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

void Reports::writeCheckStyle(std::ostream & os, bool omitDuplicates)
{
    std::string severity = prefix_;
    if (severity == "")
    {
        severity = "info";
    }
    os<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    os << "<checkstyle version=\"5.0\">\n";

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
                os << "        <error source=\"" << xmlEscape(rule)
                    << "\" severity=\"" << xmlEscape(severity)
                    << "\" line=\"" << lineNumber
                    << "\" message=\"" << xmlEscape(msg)
                    << "\" />\n";

                lastLineNumber = lineNumber;
                lastReport = report;
            }
        }

        os << "    </file>\n";
    }

    os << "</checkstyle>\n";
}

std::string Reports::xmlEscape(const std::string & msg)
{
    std::string res = msg;
    boost::algorithm::replace_all(res, "&",  "&amp;");
    boost::algorithm::replace_all(res, "\"", "&quot;");
    boost::algorithm::replace_all(res, "\'", "&apos;");
    boost::algorithm::replace_all(res, "<",  "&lt;");
    boost::algorithm::replace_all(res, ">",  "&gt;");
    return res;
}

}
}
