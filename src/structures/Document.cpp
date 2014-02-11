//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Document.h"
#include "IsTokenWithName.h"
#include "TokensIterator.h"

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include "SourceFiles.h"
#include "../plugins/Messages.h"

#define SYSTEM_INCLUDE_LABEL "sysInclude"
#define INCLUDE_LABEL "include"
#define MACRO_LABEL "macro"

#define EOF_TOKEN_NAME  "eof"

#define IS_EQUAL_RETURN(left, right) \
  {\
    if (left == right) \
    { \
      return;\
    }\
  }
#define IS_EQUAL_RETURN_FALSE(left, right) \
  {\
    if (left == right) \
    { \
      return false;\
    }\
  }
#define IS_EQUAL_BREAK(left, right) \
  {\
    if (left == right) \
    { \
      break;\
    }\
  }

namespace Vera
{
namespace Structures
{
namespace
{

std::vector<std::string> includes_;
std::vector<std::string> sysIncludes_;
std::vector<std::string> preDefines_;
std::vector<std::string> types_;
std::vector<std::string> structures_;
typedef std::map< std::string, boost::shared_ptr<Document> > DocumentSequence;
typedef std::map<std::size_t, StatementOfDefine> StatementOfDefineSequence;
typedef std::map< std::size_t, StatementOfStruct> StatementOfStructSequence;
typedef std::map<std::size_t, StatementOfEnum> StatementOfEnumSequence;
typedef std::map<std::size_t, StatementOfUnion> StatementOfUnionSequence;
DocumentSequence documents_;
StatementOfDefineSequence defines_;
StatementOfEnumSequence enums_;
StatementOfUnionSequence unions_;

} // unname namespace


void
Documents::initialize()
{
   const SourceFiles::FileNameSet& files = SourceFiles::getAllFileNames();

   for (SourceFiles::FileNameSet::const_iterator it = files.begin(); it != files.end();  ++it )
   {
     std::string fileName = *it;
     Tokens::TokenSequence tokens = Tokens::getEachTokenFromFile(fileName);

     boost::shared_ptr<Document> doc = boost::make_shared<Document>(fileName);
     doc->initialize();
     documents_[fileName] = doc;
   }
}

Document::Document(const std::string& name)
: fileName_(name)
, root_(Token("root",0,0,"unknown"))
{
  StatementsBuilder::addNodeToCollection(root_);
  root_.parentId_ = root_.id_;
  root_.parent_ = &root_;
  root_.doc_ = this;
  root_.type_ = Statement::TYPE_ITEM_ROOT;
}

void
Document::initialize()
{
 Tokens::TokenSequence& collection = collection_;
 Tokens::TokenSequence tokens = Tokens::getEachTokenFromFile(fileName_);

 std::copy(tokens.begin(), tokens.end(), std::back_inserter(collection_));

 parse();
}

static std::string toString(const std::string& path)
{
  return path.c_str();
}

static void
addDefine(PrecompilerContext& context, const std::string& macro)
{
  context.add_macro_definition(macro);
}

std::string
removeCommentsOfConfigFile(const std::string& line)
{
  std::string response;

  if (line.empty())
  {
    return response;
  }
  std::string::size_type pos = line.find("#");


  if (pos < line.size())
  {
    response = line.substr(0, pos);
  }
  else
  {
    response = line;
  }

  return response;
}

void addParameterToContext(const std::string& line)
{
  if (line.empty())
   {
     return;
   }

  std::string::size_type pos = line.find("+=");
  if (pos != std::string::npos)
  {
    std::string name = line.substr(0, pos);
    std::string value = line.substr(pos + 2);

    if (name.compare(INCLUDE_LABEL) == 0)
    {
      includes_.push_back(value);
    }
    else if (name.compare(SYSTEM_INCLUDE_LABEL) == 0)
    {
      sysIncludes_.push_back(value);
    }
    else if (name.compare(MACRO_LABEL) == 0)
    {
      preDefines_.push_back(value);
    }
  }
  else
  {
      std::ostringstream ss;
      ss << "Invalid parameter association: " << line;
      throw Vera::Structures::DocumentError(ss.str());
  }
}

void
Document::readConfigFile(std::istream& in)
{
  std::string line;
  int lineNumber = 0;
  while (std::getline(in, line))
  {
      ++lineNumber;

      if (line.empty())
      {
          continue;
      }

      std::string content = removeCommentsOfConfigFile(line);
      addParameterToContext(content);
  }
}

void
Document::readConfigFile(const std::string& fileName)
{
  if (fileName.empty())
  {
    return;
  }

  std::ifstream file(fileName.c_str());
  if (file.is_open() == false)
  {
      std::ostringstream ss;
      ss << "Cannot open config file " << fileName << ": "
         << strerror(errno);
      throw DocumentError(ss.str());
  }

  readConfigFile(file);

  if (file.bad())
  {
      throw std::ios::failure(
          "Cannot read from " + fileName + ": " + strerror(errno));
  }

  file.close();
}

void
Document::parse()
{

  Tokens::TokenSequence::const_iterator it = collection_.begin();
  Tokens::TokenSequence::const_iterator end = collection_.end();

  std::stringstream out;

  out << "Parse: "<<fileName_<<std::endl;

  Plugins::Message::get_mutable_instance().show(out.str());
  for (;it < end; ++it)
  {
    if (it->name_ == EOF_TOKEN_NAME)
    {
      continue;
    }

    if (IsValidTokenForStatement()(*it) == true)
    {
      StatementsBuilder partial(root_);
      partial.builder(it, end);
      if (it != end && IsValidTokenForStatement()(*it) == false)
      {
        root_.push(*it);
      }
    }
    else
    {
      root_.push(*it);
    }
  }
}

const Statement&
Document::getRoot()
{
  return root_;
}

boost::shared_ptr<Document>
Document::create(std::string fileName)
{
  DocumentSequence::const_iterator it = documents_.find(fileName);

  if (it != documents_.end())
  {
    return it->second;
  }

  return boost::shared_ptr<Document>();
}

} // Vera namespace
} // Structures namespace
