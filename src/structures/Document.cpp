//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Document.h"
#include "IsTokenWithName.h"

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
namespace {
std::vector<std::string> includes_;
std::vector<std::string> sysIncludes_;
std::vector<std::string> defines_;
} //unname

Document::Document(std::string& content, std::string& name)
: content_(content)
, fileName_(name)
, context_(content_.begin(), content_.end(), fileName_.c_str())
{
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

static Token convertToToken(boost::wave::cpplexer::lex_token<>& token )
{
  const boost::wave::token_id id(token);
  const token_type::position_type pos = token.get_position();
  const std::string value = token.get_value().c_str();
  const int line = pos.get_line();
  const int column = pos.get_column() - 1;

  std::string tokenName = boost::wave::get_token_name(id).c_str();
  boost::algorithm::to_lower(tokenName);

  return Token(value, line, column, tokenName);
}

void
Document::initialize()
{
  std::for_each(includes_.begin(),
      includes_.end(),
      boost::bind(&PrecompilerContext::add_include_path, boost::ref(context_), boost::bind(&std::string::c_str, _1)));

  std::for_each(sysIncludes_.begin(),
      sysIncludes_.end(),
      boost::bind(&PrecompilerContext::add_sysinclude_path, boost::ref(context_), boost::bind(&std::string::c_str, _1)));

  std::for_each(defines_.begin(),
    defines_.end(),
    boost::bind(Structures::addDefine, boost::ref(context_), _1 ));

  PrecompilerContext::iterator_type it = context_.begin();
  PrecompilerContext::iterator_type end =  context_.end();

  try
  {
    while (it != end)
    {
      collection_.push_back(convertToToken(*it));
      ++it;
    }
  }
  catch (boost::wave::cpp_exception const& e) {
  // some preprocessing error
    std::ostringstream ss;
    ss<<e.file_name() << "(" << e.line_no() << "): "
        << e.description() << std::endl;

    throw DocumentError(e.description());
  }
  catch (std::exception const& e) {
  // use last recognized token to retrieve the error position
      std::cerr
          << "  "
          << "(" << " " << "): "
          << "exception caught: " << e.what()
          << std::endl;

  }
  catch(...)
  {
    std::cout<<"\nexception"<<std::endl;
  }
  std::cout<<std::endl;
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

    if (name.compare("include") == 0)
    {
      includes_.push_back(value);
    }
    else if (name.compare("sysInclude") == 0)
    {
      sysIncludes_.push_back(value);
    }
    else if (name.compare("macro") == 0)
    {
      sysIncludes_.push_back(value);
    }
  }
  else
  {
      std::ostringstream ss;
      ss << "Invalid parameter association: " << line;
      throw Vera::Structures::DocumentError(ss.str());
  }
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
    response = line;

  return response;
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
    return;

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

  for (;it != end; ++it)
  {
    StatementsBuilder partial(root_);
    partial.addEachInvalidToken(root_, it, end);

    if(it->name_ == EOF_TOKEN_NAME)
    {
      continue;
    }

    StatementsBuilder(root_).builder(root_, it, end);

  }
}

Statement
Document::getRoot()
{
  return root_;
}

Document*
Document::createCppDocument(std::string fileName)
{
  std::ifstream file(fileName.c_str());
  std::string content;
  std::string line;

  if (file.is_open() == false)
  {
    std::ostringstream ss;
    ss << "Cannot open file " << fileName << ": "
       << strerror(errno);
    throw DocumentError(ss.str());
  }

  while(getline(file, line))
  {
    content += line;
    content += '\n';
  }

  Document* doc = new Document(content, fileName);

  return doc;
}

} // Vera namespace
} // Structures namespace


