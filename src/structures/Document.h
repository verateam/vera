//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef DOCUMENT_H_INCLUDED
#define DOCUMENT_H_INCLUDED

#include "Tokens.h"
#include <string>
#include <vector>
#include "Statements.h"
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class
#include <boost/python/object.hpp>
#include <boost/smart_ptr.hpp>

typedef boost::wave::cpplexer::lex_token<> token_type;
typedef boost::wave::cpplexer::lex_iterator<token_type> lex_iterator_type;
typedef boost::wave::context<std::string::iterator, lex_iterator_type> PrecompilerContext;

namespace Vera
{
namespace Structures
{

class DocumentError : public std::runtime_error
{
public:
    DocumentError(const std::string & msg) : std::runtime_error(msg) {}
};

/**
 * @brief TODO
 */
class Document
//: private boost::noncopyable
{
  public:

    Document(std::string& content, std::string& name);

    void initialize();

    /**
     * @brief Reads the config file which contains the "include" of the directories and the system.
     * This file also contains the predefined macros list.
     * @param fileName The file name.
     */
    static void readConfigFile(const std::string& fileName);

    static void readConfigFile(std::istream& in);

    void parse();

    Statement getRoot();

    static Document*  createCppDocument(std::string fileName);

  private:

    std::string content_;
    std::string fileName_;
    Statement root_;
    PrecompilerContext context_;
    Tokens::TokenSequence collection_;
};

} // namespace Structures
} // namespace Vera

#endif // DOCUMENT_H_INCLUDED
