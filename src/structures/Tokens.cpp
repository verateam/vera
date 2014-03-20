//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Tokens.h"
#include "SourceLines.h"
#include "../plugins/Reports.h"
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpplexer_exceptions.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <cctype>


namespace // unnamed
{

typedef std::vector<std::string> PhysicalTokenCollection;
PhysicalTokenCollection physicalTokens;

struct TokenRef
{
    TokenRef(boost::wave::token_id id, int line, int column, int length)
        : id_(id), line_(line), column_(column), length_(length), index_(-1) {}

    TokenRef(boost::wave::token_id id, int line, int column, int length, const std::string & value)
        : id_(id), line_(line), column_(column), length_(length)
    {
        // newline is optimized as the most common case

        if (id_ != boost::wave::T_NEWLINE)
        {
            // value of the token is stored in physicalTokens collection
            // (because it has no physical representation in the source code)

            index_ = static_cast<int>(physicalTokens.size());
            physicalTokens.push_back(value);
        }
    }

    std::string getTokenValue(const Vera::Structures::SourceFiles::FileName & fileName) const
    {
        if (id_ == boost::wave::T_NEWLINE)
        {
            return "\n";
        }
        else if (index_ >= 0)
        {
            // token value stored in the physicalTokens structure
            // (this is used with line continuation and other cases
            // where the token has no representation in physical lines)

            return physicalTokens[static_cast<size_t>(index_)];
        }
        else
        {
            // token value has to be retrieved from the physical line collection

            return Vera::Structures::SourceLines::getLine(fileName, line_).substr(column_, length_);
        }
    }

    boost::wave::token_id id_;
    int line_;
    int column_;
    int length_;

    // if >= 0, it is the index into the physicalTokens collection,
    // used only for line continuation
    // and when line_ and column_ do not reflect the physical layout:
    int index_;
};

typedef std::vector<TokenRef> TokenCollection;

typedef std::map<Vera::Structures::SourceFiles::FileName, TokenCollection> FileTokenCollection;

FileTokenCollection fileTokens_;

typedef std::vector<boost::function<bool (boost::wave::token_id)> > CompiledFilterSequence;

boost::wave::token_id tokenIdFromTokenFilter(const Vera::Structures::Tokens::TokenFilter & filter)
{
    typedef std::map<Vera::Structures::Tokens::TokenFilter, boost::wave::token_id>
        TokenFilterToIdMap;

    static TokenFilterToIdMap tokenMap;
    if (tokenMap.empty())
    {
        tokenMap["and"] = static_cast<boost::wave::token_id>(256);
        tokenMap["andand"] = static_cast<boost::wave::token_id>(257);
        tokenMap["assign"] = static_cast<boost::wave::token_id>(258);
        tokenMap["andassign"] = static_cast<boost::wave::token_id>(259);
        tokenMap["or"] = static_cast<boost::wave::token_id>(260);
        tokenMap["orassign"] = static_cast<boost::wave::token_id>(261);
        tokenMap["xor"] = static_cast<boost::wave::token_id>(262);
        tokenMap["xorassign"] = static_cast<boost::wave::token_id>(263);
        tokenMap["comma"] = static_cast<boost::wave::token_id>(264);
        tokenMap["colon"] = static_cast<boost::wave::token_id>(265);
        tokenMap["divide"] = static_cast<boost::wave::token_id>(266);
        tokenMap["divideassign"] = static_cast<boost::wave::token_id>(267);
        tokenMap["dot"] = static_cast<boost::wave::token_id>(268);
        tokenMap["dotstar"] = static_cast<boost::wave::token_id>(269);
        tokenMap["ellipsis"] = static_cast<boost::wave::token_id>(270);
        tokenMap["equal"] = static_cast<boost::wave::token_id>(271);
        tokenMap["greater"] = static_cast<boost::wave::token_id>(272);
        tokenMap["greaterequal"] = static_cast<boost::wave::token_id>(273);
        tokenMap["leftbrace"] = static_cast<boost::wave::token_id>(274);
        tokenMap["less"] = static_cast<boost::wave::token_id>(275);
        tokenMap["lessequal"] = static_cast<boost::wave::token_id>(276);
        tokenMap["leftparen"] = static_cast<boost::wave::token_id>(277);
        tokenMap["leftbracket"] = static_cast<boost::wave::token_id>(278);
        tokenMap["minus"] = static_cast<boost::wave::token_id>(279);
        tokenMap["minusassign"] = static_cast<boost::wave::token_id>(280);
        tokenMap["minusminus"] = static_cast<boost::wave::token_id>(281);
        tokenMap["percent"] = static_cast<boost::wave::token_id>(282);
        tokenMap["percentassign"] = static_cast<boost::wave::token_id>(283);
        tokenMap["not"] = static_cast<boost::wave::token_id>(284);
        tokenMap["notequal"] = static_cast<boost::wave::token_id>(285);
        tokenMap["oror"] = static_cast<boost::wave::token_id>(286);
        tokenMap["plus"] = static_cast<boost::wave::token_id>(287);
        tokenMap["plusassign"] = static_cast<boost::wave::token_id>(288);
        tokenMap["plusplus"] = static_cast<boost::wave::token_id>(289);
        tokenMap["arrow"] = static_cast<boost::wave::token_id>(290);
        tokenMap["arrowstar"] = static_cast<boost::wave::token_id>(291);
        tokenMap["question_mark"] = static_cast<boost::wave::token_id>(292);
        tokenMap["rightbrace"] = static_cast<boost::wave::token_id>(293);
        tokenMap["rightparen"] = static_cast<boost::wave::token_id>(294);
        tokenMap["rightbracket"] = static_cast<boost::wave::token_id>(295);
        tokenMap["colon_colon"] = static_cast<boost::wave::token_id>(296);
        tokenMap["semicolon"] = static_cast<boost::wave::token_id>(297);
        tokenMap["shiftleft"] = static_cast<boost::wave::token_id>(298);
        tokenMap["shiftleftassign"] = static_cast<boost::wave::token_id>(299);
        tokenMap["shiftright"] = static_cast<boost::wave::token_id>(300);
        tokenMap["shiftrightassign"] = static_cast<boost::wave::token_id>(301);
        tokenMap["star"] = static_cast<boost::wave::token_id>(302);
        tokenMap["compl"] = static_cast<boost::wave::token_id>(303);
        tokenMap["starassign"] = static_cast<boost::wave::token_id>(304);
        tokenMap["asm"] = static_cast<boost::wave::token_id>(305);
        tokenMap["auto"] = static_cast<boost::wave::token_id>(306);
        tokenMap["bool"] = static_cast<boost::wave::token_id>(307);
        tokenMap["false"] = static_cast<boost::wave::token_id>(308);
        tokenMap["true"] = static_cast<boost::wave::token_id>(309);
        tokenMap["break"] = static_cast<boost::wave::token_id>(310);
        tokenMap["case"] = static_cast<boost::wave::token_id>(311);
        tokenMap["catch"] = static_cast<boost::wave::token_id>(312);
        tokenMap["char"] = static_cast<boost::wave::token_id>(313);
        tokenMap["class"] = static_cast<boost::wave::token_id>(314);
        tokenMap["const"] = static_cast<boost::wave::token_id>(315);
        tokenMap["constcast"] = static_cast<boost::wave::token_id>(316);
        tokenMap["continue"] = static_cast<boost::wave::token_id>(317);
        tokenMap["default"] = static_cast<boost::wave::token_id>(318);
        tokenMap["delete"] = static_cast<boost::wave::token_id>(319);
        tokenMap["do"] = static_cast<boost::wave::token_id>(320);
        tokenMap["double"] = static_cast<boost::wave::token_id>(321);
        tokenMap["dynamiccast"] = static_cast<boost::wave::token_id>(322);
        tokenMap["else"] = static_cast<boost::wave::token_id>(323);
        tokenMap["enum"] = static_cast<boost::wave::token_id>(324);
        tokenMap["explicit"] = static_cast<boost::wave::token_id>(325);
        tokenMap["export"] = static_cast<boost::wave::token_id>(326);
        tokenMap["extern"] = static_cast<boost::wave::token_id>(327);
        tokenMap["float"] = static_cast<boost::wave::token_id>(328);
        tokenMap["for"] = static_cast<boost::wave::token_id>(329);
        tokenMap["friend"] = static_cast<boost::wave::token_id>(330);
        tokenMap["goto"] = static_cast<boost::wave::token_id>(331);
        tokenMap["if"] = static_cast<boost::wave::token_id>(332);
        tokenMap["inline"] = static_cast<boost::wave::token_id>(333);
        tokenMap["int"] = static_cast<boost::wave::token_id>(334);
        tokenMap["long"] = static_cast<boost::wave::token_id>(335);
        tokenMap["mutable"] = static_cast<boost::wave::token_id>(336);
        tokenMap["namespace"] = static_cast<boost::wave::token_id>(337);
        tokenMap["new"] = static_cast<boost::wave::token_id>(338);
        tokenMap["operator"] = static_cast<boost::wave::token_id>(339);
        tokenMap["private"] = static_cast<boost::wave::token_id>(340);
        tokenMap["protected"] = static_cast<boost::wave::token_id>(341);
        tokenMap["public"] = static_cast<boost::wave::token_id>(342);
        tokenMap["register"] = static_cast<boost::wave::token_id>(343);
        tokenMap["reinterpretcast"] = static_cast<boost::wave::token_id>(344);
        tokenMap["return"] = static_cast<boost::wave::token_id>(345);
        tokenMap["short"] = static_cast<boost::wave::token_id>(346);
        tokenMap["signed"] = static_cast<boost::wave::token_id>(347);
        tokenMap["sizeof"] = static_cast<boost::wave::token_id>(348);
        tokenMap["static"] = static_cast<boost::wave::token_id>(349);
        tokenMap["staticcast"] = static_cast<boost::wave::token_id>(350);
        tokenMap["struct"] = static_cast<boost::wave::token_id>(351);
        tokenMap["switch"] = static_cast<boost::wave::token_id>(352);
        tokenMap["template"] = static_cast<boost::wave::token_id>(353);
        tokenMap["this"] = static_cast<boost::wave::token_id>(354);
        tokenMap["throw"] = static_cast<boost::wave::token_id>(355);
        tokenMap["try"] = static_cast<boost::wave::token_id>(356);
        tokenMap["typedef"] = static_cast<boost::wave::token_id>(357);
        tokenMap["typeid"] = static_cast<boost::wave::token_id>(358);
        tokenMap["typename"] = static_cast<boost::wave::token_id>(359);
        tokenMap["union"] = static_cast<boost::wave::token_id>(360);
        tokenMap["unsigned"] = static_cast<boost::wave::token_id>(361);
        tokenMap["using"] = static_cast<boost::wave::token_id>(362);
        tokenMap["virtual"] = static_cast<boost::wave::token_id>(363);
        tokenMap["void"] = static_cast<boost::wave::token_id>(364);
        tokenMap["volatile"] = static_cast<boost::wave::token_id>(365);
        tokenMap["wchart"] = static_cast<boost::wave::token_id>(366);
        tokenMap["while"] = static_cast<boost::wave::token_id>(367);
        tokenMap["pp_define"] = static_cast<boost::wave::token_id>(368);
        tokenMap["pp_if"] = static_cast<boost::wave::token_id>(369);
        tokenMap["pp_ifdef"] = static_cast<boost::wave::token_id>(370);
        tokenMap["pp_ifndef"] = static_cast<boost::wave::token_id>(371);
        tokenMap["pp_else"] = static_cast<boost::wave::token_id>(372);
        tokenMap["pp_elif"] = static_cast<boost::wave::token_id>(373);
        tokenMap["pp_endif"] = static_cast<boost::wave::token_id>(374);
        tokenMap["pp_error"] = static_cast<boost::wave::token_id>(375);
        tokenMap["pp_line"] = static_cast<boost::wave::token_id>(376);
        tokenMap["pp_pragma"] = static_cast<boost::wave::token_id>(377);
        tokenMap["pp_undef"] = static_cast<boost::wave::token_id>(378);
        tokenMap["pp_warning"] = static_cast<boost::wave::token_id>(379);
        tokenMap["identifier"] = static_cast<boost::wave::token_id>(380);
        tokenMap["octalint"] = static_cast<boost::wave::token_id>(381);
        tokenMap["decimalint"] = static_cast<boost::wave::token_id>(382);
        tokenMap["hexaint"] = static_cast<boost::wave::token_id>(383);
        tokenMap["intlit"] = static_cast<boost::wave::token_id>(384);
        tokenMap["longintlit"] = static_cast<boost::wave::token_id>(385);
        tokenMap["floatlit"] = static_cast<boost::wave::token_id>(386);
        tokenMap["ccomment"] = static_cast<boost::wave::token_id>(387);
        tokenMap["cppcomment"] = static_cast<boost::wave::token_id>(388);
        tokenMap["charlit"] = static_cast<boost::wave::token_id>(389);
        tokenMap["stringlit"] = static_cast<boost::wave::token_id>(390);
        tokenMap["contline"] = static_cast<boost::wave::token_id>(391);
        tokenMap["space"] = static_cast<boost::wave::token_id>(392);
        tokenMap["space2"] = static_cast<boost::wave::token_id>(393);
        tokenMap["newline"] = static_cast<boost::wave::token_id>(394);
        tokenMap["pound_pound"] = static_cast<boost::wave::token_id>(395);
        tokenMap["pound"] = static_cast<boost::wave::token_id>(396);
        tokenMap["any"] = static_cast<boost::wave::token_id>(397);
        tokenMap["pp_include"] = static_cast<boost::wave::token_id>(398);
        tokenMap["pp_qheader"] = static_cast<boost::wave::token_id>(399);
        tokenMap["pp_hheader"] = static_cast<boost::wave::token_id>(400);
        tokenMap["eof"] = static_cast<boost::wave::token_id>(401);
        tokenMap["eoi"] = static_cast<boost::wave::token_id>(402);
        tokenMap["pp_number"] = static_cast<boost::wave::token_id>(403);
        tokenMap["msext_int8"] = static_cast<boost::wave::token_id>(404);
        tokenMap["msext_int16"] = static_cast<boost::wave::token_id>(405);
        tokenMap["msext_int32"] = static_cast<boost::wave::token_id>(406);
        tokenMap["msext_int64"] = static_cast<boost::wave::token_id>(407);
        tokenMap["msext_based"] = static_cast<boost::wave::token_id>(408);
        tokenMap["msext_declspec"] = static_cast<boost::wave::token_id>(409);
        tokenMap["msext_cdecl"] = static_cast<boost::wave::token_id>(410);
        tokenMap["msext_fastcall"] = static_cast<boost::wave::token_id>(411);
        tokenMap["msext_stdcall"] = static_cast<boost::wave::token_id>(412);
        tokenMap["msext_try"] = static_cast<boost::wave::token_id>(413);
        tokenMap["msext_except"] = static_cast<boost::wave::token_id>(414);
        tokenMap["msext_finally"] = static_cast<boost::wave::token_id>(415);
        tokenMap["msext_leave"] = static_cast<boost::wave::token_id>(416);
        tokenMap["msext_inline"] = static_cast<boost::wave::token_id>(417);
        tokenMap["msext_asm"] = static_cast<boost::wave::token_id>(418);
        tokenMap["msext_region"] = static_cast<boost::wave::token_id>(419);
        tokenMap["msext_endregion"] = static_cast<boost::wave::token_id>(420);
        tokenMap["import"] = static_cast<boost::wave::token_id>(421);
    }

    const TokenFilterToIdMap::const_iterator it = tokenMap.find(filter);
    if (it != tokenMap.end())
    {
        return it->second;
    }
    else
    {
        throw Vera::Structures::TokensError("unknown token filter requested");
    }
}

bool matchAll(boost::wave::token_id)
{
    return true;
}

bool matchTokenBaseId(boost::wave::token_id id, boost::wave::token_id ref)
{
    return BASEID_FROM_TOKEN(id) == ref;
}

CompiledFilterSequence prepareCompiledFilter(
    const Vera::Structures::Tokens::FilterSequence & filterSeq)
{
    CompiledFilterSequence ret;

    if (filterSeq.empty())
    {
        // with empty filter all tokens should be accepted
        ret.push_back(matchAll);
    }
    else
    {
        Vera::Structures::Tokens::FilterSequence::const_iterator it = filterSeq.begin();
        const Vera::Structures::Tokens::FilterSequence::const_iterator end = filterSeq.end();

        for ( ; it != end; ++it)
        {
            ret.push_back(bind(matchTokenBaseId, _1, tokenIdFromTokenFilter(*it)));
        }
    }

    return ret;
}

bool match(const CompiledFilterSequence & compiledFilter, boost::wave::token_id id)
{
    CompiledFilterSequence::const_iterator it = compiledFilter.begin();
    const CompiledFilterSequence::const_iterator end = compiledFilter.end();

    for ( ; it != end; ++it)
    {
        if ((*it)(id))
        {
            return true;
        }
    }

    return false;
}

struct LineNumberComparator
{
    bool operator()(const TokenRef & left, const TokenRef & right) const
    {
        return left.line_ < right.line_;
    }
};

void findRange(const TokenCollection & tokens, int fromLine, int toLine,
    TokenCollection::const_iterator & beg, TokenCollection::const_iterator & end)
{
    const TokenRef tokenToCompareFrom(boost::wave::token_id(), fromLine, 0, 0);
    beg = lower_bound(tokens.begin(), tokens.end(), tokenToCompareFrom, LineNumberComparator());

    if (toLine < 0)
    {
        end = tokens.end();
    }
    else
    {
        const TokenRef tokenToCompareTo(boost::wave::token_id(), toLine, 0, 0);
        end = upper_bound(tokens.begin(), tokens.end(), tokenToCompareTo, LineNumberComparator());
    }
}

} // unnamed namespace

namespace Vera
{
namespace Structures
{

void Tokens::parse(const SourceFiles::FileName & name, const FileContent & src)
{
    TokenCollection & tokensInFile = fileTokens_[name];

    // wave throws exceptions when given an empty file
    if (src.empty() == false)
    {
        try
        {
            typedef boost::wave::cpplexer::lex_token<> token_type;
            typedef boost::wave::cpplexer::lex_iterator<token_type> lexer_type;
            typedef token_type::position_type position_type;

            const position_type pos(name.c_str());
            lexer_type it = lexer_type(src.begin(), src.end(), pos,
                boost::wave::language_support(
                    boost::wave::support_cpp | boost::wave::support_option_long_long));
            const lexer_type end = lexer_type();

            const int lineCount = SourceLines::getLineCount(name);

            for ( ; it != end; ++it)
            {
                const boost::wave::token_id id(*it);

                const token_type::position_type pos = it->get_position();
                const std::string value = it->get_value().c_str();
                const int line = pos.get_line();
                const int column = pos.get_column() - 1;
                const int length = static_cast<int>(value.size());

                bool useReference = true;
                if (id == boost::wave::T_NEWLINE || id == boost::wave::T_EOF || line > lineCount)
                {
                    useReference = false;
                }
                else
                {
                    const std::string & sourceLine = SourceLines::getLine(name, line);
                    if (column > static_cast<int>(sourceLine.size()) ||
                        value != sourceLine.substr(column, length))
                    {
                        useReference = false;
                    }
                }

                if (useReference)
                {
                    // the reference representation of the token is stored

                    tokensInFile.push_back(TokenRef(id, line, column, length));
                }
                else
                {
                    // value of the token has no representation in the physical line
                    // so the real token value is stored in physicalTokens

                    tokensInFile.push_back(TokenRef(id, line, column, length, value));
                }
            }
        }
        catch (const boost::wave::cpplexer::cpplexer_exception & e)
        {
            std::ostringstream ss;
            ss << "illegal token in column " << e.column_no()
                << ", giving up (hint: fix the file or remove it from the working set)";
            Plugins::Reports::internal(name, e.line_no(), ss.str());
        }
    }
}

Tokens::TokenSequence Tokens::getTokens(const SourceFiles::FileName & fileName,
    int fromLine, int fromColumn, int toLine, int toColumn,
    const FilterSequence & filter)
{
    if ((fromLine < 1) ||
        (fromColumn < 0) ||
        (toLine > 0 && fromLine > toLine) ||
        (fromLine == toLine && toColumn >= 0 && fromColumn > toColumn))
    {
        throw TokensError("illegal range of tokens requested by the script");
    }

    {
        // lazy load and parse
        const FileTokenCollection::const_iterator fit = fileTokens_.find(fileName);
        if (fit == fileTokens_.end())
        {
            SourceLines::loadFile(fileName);
        }
    }

    // here we know that the file is already loaded and parsed
    // (or the exception was thrown in the above)

    const CompiledFilterSequence compiledFilter = prepareCompiledFilter(filter);

    const TokenCollection & tokensInFile = fileTokens_[fileName];

    TokenSequence ret;

    TokenCollection::const_iterator begin;
    TokenCollection::const_iterator end;

    findRange(tokensInFile, fromLine, toLine, begin, end);

    for (TokenCollection::const_iterator it = begin; it != end; ++it)
    {
        const TokenRef & token = *it;

        const int line = token.line_;
        const int column = token.column_;

        if ((line > fromLine || (line == fromLine && column >= fromColumn)) &&
            (toLine <= 0 || (line < toLine || (line == toLine && column < toColumn))))
        {
            if (match(compiledFilter, token.id_))
            {
                std::string tokenName = boost::wave::get_token_name(token.id_).c_str();
                boost::algorithm::to_lower(tokenName);

                std::string value;
                if (tokenName != "eof")
                {
                    value = token.getTokenValue(fileName);
                }

                ret.push_back(Token(value, line, column, tokenName));
            }
        }
    }

    return ret;
}

}
}
