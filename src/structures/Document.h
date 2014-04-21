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
#ifdef THREAD_SUPPORT
#include <boost/thread/mutex.hpp>
#endif
#include <boost/wave/util/cpp_include_paths.hpp>

#include <string>
#include <vector>
#include <map>
#include "Statements.h"

#include <boost/smart_ptr.hpp>

#ifdef VERA_PYTHON
#include <boost/python.hpp>
#endif

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
    typedef std::vector<std::string> Headers;
    typedef std::vector<std::size_t> Externs;

#ifdef VERA_PYTHON
    typedef boost::python::dict  Dictionary;
#else
    typedef Document::RegisterItems Dictionary;
#endif

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

    void addDefine(const std::string& name, std::size_t id);
    void addStruct(const std::string& name, std::size_t id);
    void addEnum(const std::string& name, std::size_t id);
    void addClass(const std::string& name, std::size_t id);
    void addUnion(const std::string& name, std::size_t id);
    void addTypedef(const std::string& name, std::size_t id);
    void addExtern(std::size_t id);
    void addNamespace(const std::string& name, std::size_t id);
    void addHeader(const std::string& name);

    bool addIncludePath(const std::string& path);
    bool addSysIncludePath(const std::string& path);

    Dictionary getDefineStack();
    Dictionary getStructStack();
    Dictionary getEnumStack();
    Dictionary getUnionStack();
    Dictionary getClassStack();
    Dictionary getTypedefStack();
    Dictionary getNamespaceStack();
    Externs getExternStack();
    Headers getHeaderStack();

    Statement getNode(std::size_t id);

    void parseHeader(const std::string& content);

    void initialize();

    void enableUnion();

    void disableUnion();

    void enableFunction();

    void disableFunction();

    bool isFunction();

    bool isUnion();

    bool isInitialize();

    std::string getFileName();

  private:

    std::string fileName_;
    Statement root_;
    RegisterItems defineMap_;
    RegisterItems structMap_;
    RegisterItems enumMap_;
    RegisterItems classMap_;
    RegisterItems unionMap_;
    RegisterItems typedefMap_;
    RegisterItems namespaceMap_;
    Externs externMap_;
    boost::wave::util::include_paths paths_;
    Headers headers_;

    Tokens::TokenSequence collection_;
    bool wasInitialized_;
    bool isFunction_;
    bool isUnion_;
#ifdef THREAD_SUPPORT
    boost::mutex mutex_;
#endif
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
