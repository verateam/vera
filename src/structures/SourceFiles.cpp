//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "SourceFiles.h"
#include <boost/wave/util/filesystem_compatibility.hpp>


namespace // unnamed
{

Vera::Structures::SourceFiles::FileNameSet files_;

} // unnamed namespace

namespace Vera
{
namespace Structures
{

int SourceFiles::count()
{
    return files_.size();
}

void SourceFiles::addFileName(const FileName & name)
{
  boost::filesystem::path current_dir(boost::wave::util::initial_path());

  boost::filesystem::path filepath (boost::wave::util::create_path(name.c_str()));
  boost::filesystem::path filename = boost::wave::util::complete_path(filepath, current_dir);
  std::string item = boost::wave::util::normalize(filename).c_str();

  files_.insert(item);
}

bool SourceFiles::empty()
{
    return files_.empty();
}

const SourceFiles::FileNameSet & SourceFiles::getAllFileNames()
{
    return files_;
}

}
}
