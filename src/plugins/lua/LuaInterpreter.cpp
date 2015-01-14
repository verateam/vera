//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifdef VERA_LUA

#include "LuaInterpreter.h"
#include "../Exclusions.h"
#include "../Reports.h"
#include "../Parameters.h"
#include "../../structures/SourceFiles.h"
#include "../../structures/SourceLines.h"
#include "../../structures/Tokens.h"
#include <fstream>
#include <iterator>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/iterator_policy.hpp>
extern "C"
{
#include "lualib.h"
}

namespace luabind
{

template<typename T>
struct default_converter<std::vector<T> > : native_converter_base<std::vector<T> >
{
    static int compute_score(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TTABLE ? 0 : -1;
    }

    std::vector<T> from(lua_State* L, int index)
    {
        std::vector<T> list;

        for (luabind::iterator i(luabind::object(luabind::from_stack(L, index))), end;
            i != end; ++i)
        {
            list.push_back(luabind::object_cast<T>(*i));
        }

        return list;
    }

    void to(lua_State* L, const std::vector<T>& vector)
    {
        luabind::object list = luabind::newtable(L);

        for (std::size_t i = 0; i < vector.size(); ++i)
        {
            list[i + 1] = vector[i];
        }

        list.push(L);
    }
};

template<typename T>
struct default_converter<std::vector<T> const&>
        : default_converter<std::vector<T> >
{
};

} // namespace luabind

namespace Vera
{
namespace Plugins
{

// Structures::SourceFiles::getAllFileNames() returns a std::set that is not
// easily wrapped with luabind. It also lack the filtering of the excluded
// files
std::vector<std::string> sourceFileNames;
std::vector<std::string> const& luaGetSourceFileNames()
{
    if (sourceFileNames.empty())
    {
        const Vera::Structures::SourceFiles::FileNameSet & files =
                Vera::Structures::SourceFiles::getAllFileNames();

        typedef Vera::Structures::SourceFiles::FileNameSet::const_iterator iterator;
        const iterator end = files.end();
        for (iterator it = files.begin(); it != end; ++it)
        {
            const Vera::Structures::SourceFiles::FileName & name = *it;

            if (Vera::Plugins::Exclusions::isExcluded(name) == false)
            {
                sourceFileNames.push_back(name);
            }
        }
    }
    return sourceFileNames;
}

// Structures::Tokens::getTokens does not return a reference, so we must cache
// the result to be able to use it with return_stl_iterator
std::map<std::string, Structures::Tokens::TokenSequence> tokens;
Structures::Tokens::TokenSequence const& getTokens(
    const Structures::SourceFiles::FileName & fileName,
    int fromLine, int fromColumn, int toLine, int toColumn,
    const Structures::Tokens::FilterSequence & filter)
{
  // create a key unique for all these parameters
  std::ostringstream keyStream;
  keyStream << fileName;
  keyStream << "$" << fromLine;
  keyStream << "$" << fromColumn;
  keyStream << "$" << toLine;
  keyStream << "$" << toColumn;
  foreach(std::string const& s, filter)
  {
    keyStream << "$" << s;
  }
  std::string key = keyStream.str();
  std::map<std::string, Structures::Tokens::TokenSequence>::iterator it = tokens.find(key);
  if (it != tokens.end())
  {
    return it->second;
  }
  tokens[key] = Structures::Tokens::getTokens(fileName, fromLine, fromColumn,
    toLine, toColumn, filter);
  return tokens[key];
}

void LuaInterpreter::execute(const std::string & fileName)
{
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  luabind::open(L);

  luabind::module(L)
  [

      luabind::class_<Structures::Token>("token")
          .def_readonly("value", &Structures::Token::value_)
          .def_readonly("line", &Structures::Token::line_)
          .def_readonly("column", &Structures::Token::column_)
          .def_readonly("name", &Structures::Token::name_)
          .def_readonly("type", &Structures::Token::name_),


      luabind::def("getTokens", &getTokens, luabind::return_stl_iterator),

      luabind::def("report", &Plugins::Reports::add),

      luabind::def("getParameter", &Plugins::Parameters::get),

      luabind::def("getSourceFileNames", &luaGetSourceFileNames, luabind::return_stl_iterator),

      luabind::def("getLineCount", &Structures::SourceLines::getLineCount),

      luabind::def("getLine", &Structures::SourceLines::getLine),

      luabind::def("getAllLines", &Structures::SourceLines::getAllLines,
          luabind::return_stl_iterator)

  ];

  lua_getglobal(L, "debug");
  lua_getfield(L, -1, "traceback");
  lua_replace(L, -2);
  if (luaL_loadfile(L, fileName.c_str()) || lua_pcall(L, 0, LUA_MULTRET, -2))
  {
      throw std::runtime_error(lua_tostring(L, lua_gettop(L)));
  }
  lua_pop(L, 1); /* remove debug.traceback from the stack */
  lua_close(L); // TODO: fix leak!
}

}
}

#endif
