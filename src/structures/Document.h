//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef DOCUMENT_H_INCLUDED
#define DOCUMENT_H_INCLUDED

#include "Tokens.h"
#include "StatementOfDefine.h"
#include "StatementOfStruct.h"
#include "StatementOfUnion.h"
#include "StatementOfEnum.h"
#include <boost/thread/mutex.hpp>
#include <boost/wave/util/cpp_include_paths.hpp>

#include <string>
#include <vector>
#include "Statements.h"
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class
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
: private boost::noncopyable
{
  public:

    typedef std::map<std::size_t, std::string> RegisterItems;

  public:

    Document(const std::string& name);

    /**
     * @brief Reads the config file which contains the "include" of the directories and the system.
     * This file also contains the predefined macros list.
     * 
     * @param fileName The file name.
     */
    static void readConfigFile(const std::string& fileName);

    /**
     * @brief Reads the config file which contains the "include" of the directories and the system.
     * This file also contains the predefined macros list.
     * 
     * @param in The reference to the file.
     */
    static void readConfigFile(std::istream& in);
    
    /**
     * @brief Analyze the cpp document.
     */
    void parse();

    /**
     * @brief Retrieves the root statement of the document.
     * 
     * @return The root statement of the document.
     */
    const Statement& getRoot();
    
    /**
     * @brief Creates a pointer to the cpp document.
     * 
     * @return A pointer to the cpp document.
     */
    static boost::shared_ptr<Document> create(std::string fileName);

    void addDefine(std::string name, std::size_t id);
    void addStruct(std::string name, std::size_t id);
    void addEnum(std::string name, std::size_t id);
    void addClass(std::string name, std::size_t id);
    void addUnion(std::string name, std::size_t id);
    void addTypedef(std::string name, std::size_t id);


    bool addIncludePath(const std::string& path);
    bool addSysIncludePath(const std::string& path);

    RegisterItems getRegisterDefine();
    RegisterItems getRegisterStruct();
    RegisterItems getRegisterEnum();
    RegisterItems getRegisterClass();
    RegisterItems getRegisterTypedef();

    void parseHeader(const std::string& content);

    void initialize();

    void enableUnion();

    void disableUnion();

    void enableFunction();

    void disableFunction();


    bool isFunction();

    bool isUnion();

  private:

    std::string fileName_;
    Statement root_;
    RegisterItems defineMap_;
    RegisterItems structMap_;
    RegisterItems enumMap_;
    RegisterItems classMap_;
    RegisterItems unionMap_;
    RegisterItems typedefMap_;
    boost::wave::util::include_paths paths_;


    Tokens::TokenSequence collection_;
    bool wasInitialized_;
    bool isFunction_;
    bool isUnion_;
    boost::mutex mutex_;
};


class Documents
{
  public:
   void initialize();

   void parse(std::string fileName);
};

} // namespace Structures
} // namespace Vera

#endif // DOCUMENT_H_INCLUDED
