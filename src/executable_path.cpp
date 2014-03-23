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

#include "executable_path.h"

#if defined(_WIN32)

#  include <Windows.h>

std::string executable_path(const char *argv0)
{
    char buf[1024] = {0};
    DWORD ret = GetModuleFileNameA(NULL, buf, sizeof(buf));
    if (ret == 0 || ret == sizeof(buf))
    {
        return executable_path_fallback(argv0);
    }
    return buf;
}

#elif defined(__APPLE__)

#  include <mach-o/dyld.h>

std::string executable_path(const char *argv0)
{
    char buf[1024] = {0};
    uint32_t size = sizeof(buf);
    int ret = _NSGetExecutablePath(buf, &size);
    if (0 != ret)
    {
        return executable_path_fallback(argv0);
    }
    boost::filesystem::path full_path(
        boost::filesystem::system_complete(
            boost::filesystem::path(buf).normalize()));
    return full_path.string();
}

#elif defined(sun) || defined(__sun)

#  include <stdlib.h>

std::string executable_path(const char *argv0)
{
    return getexecname();
}

#elif defined(__FreeBSD__)

#  include <sys/sysctl.h>

std::string executable_path(const char *argv0)
{
    int mib[4] = {0};
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = -1;
    char buf[1024] = {0};
    size_t size = sizeof(buf);
    sysctl(mib, 4, buf, &size, NULL, 0);
    if (size == 0 || size == sizeof(buf))
    {
        return executable_path_fallback(argv0);
    }
    std::string path(buf, size);
    boost::filesystem::path full_path(
        boost::filesystem::system_complete(
            boost::filesystem::path(path).normalize()));
    return full_path.string();
}

#elif defined(__linux__)

#  include <unistd.h>

std::string executable_path(const char *argv0)
{
    char buf[1024] = {0};
    ssize_t size = readlink("/proc/self/exe", buf, sizeof(buf));
    if (size == 0 || size == sizeof(buf))
    {
        return executable_path_fallback(argv0);
    }
    std::string path(buf, size);
    boost::filesystem::path full_path(
        boost::filesystem::system_complete(
            boost::filesystem::path(path).normalize()));
    return full_path.string();
}

#else

std::string executable_path(const char *argv0)
{
    return executable_path_fallback(argv0);
}

#endif

std::string executable_path_fallback(const char *argv0)
{
    if (0 == argv0 || 0 == argv0[0])
    {
        return "";
    }
    boost::filesystem::path full_path(
        boost::filesystem::system_complete(
            boost::filesystem::path(argv0)));
    return full_path.string();
}
