//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef REPORTS_H_INCLUDED
#define REPORTS_H_INCLUDED

#include "Reports.h"
#include <string>
#include <ostream>


namespace Vera
{
namespace Plugins
{


class Reports
{
public:
    typedef std::string FileName;
    typedef std::string Message;

    static void setShowRules(bool show);
    static void setVCFormat(bool vc);
    static void setXMLReport(bool xmlReport);
    static void setPrefix(std::string prefix);

    static int count();

    static void add(const FileName & name, int lineNumber, const Message & msg);
    static void internal(const FileName & name, int lineNumber,
      const Message & msg);

    static void dumpAll(std::ostream & os, bool omitDuplicates);

    static void writeStd(std::ostream & os, bool omitDuplicates);
    static void writeVc(std::ostream & os, bool omitDuplicates);
    static void writeXml(std::ostream & os, bool omitDuplicates);
    static void writeCheckStyle(std::ostream & os, bool omitDuplicates);

private:
    static void dumpAllNormal(std::ostream & os, bool omitDuplicates);
    static void dumpAllXML(std::ostream & os, bool omitDuplicates);
    static std::string xmlEscape(const std::string & msg);
};

} // namespace Plugins

} // namespace Vera

#endif // REPORTS_H_INCLUDED
