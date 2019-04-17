//
//  Tokenizer.hpp
//  CPP
//
//  Created by Zehua Chen on 4/15/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#pragma once

#include "json/Token/Token.hpp"
#include "json/Utils/Letters.hpp"
#include <string>

namespace json::token
{
template<typename CharT, typename IterT>
class Tokenizer
{
public:
    Tokenizer(IterT begin, IterT end);

    /**
     * Take an input iterator to extract letter to process
     * @param iter the input iterator to extract the letter from.
     * @param end the end input iterator
     * @returns returns after a iterator has been found
     */
    void extract();

    bool isAtEndOfString();

    /**
     * Get a reference to the current token
     * @returns a reference to the current token
     */
    Token<CharT> &token();

    operator bool();

private:
    enum class _State
    {
        start,
        string,
        finished
    };

    void _string(CharT boundary);

    Token<CharT> _token;
    _State _state;
    IterT _begin;
    IterT _end;

    // State data
    std::basic_string<CharT> _stringBuffer;
};
} // namespace json::token

// Implementations

namespace json::token
{
template<typename CharT, typename IterT>
Tokenizer<CharT, IterT>::Tokenizer(IterT begin, IterT end)
    : _state(_State::start)
    , _begin(begin)
    , _end(end)
{
}

template<typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::extract()
{
    using namespace json::utils;
    using TType = typename Token<CharT>::Type;

    while (_begin != _end)
    {
        CharT letter = *_begin;
        ++_begin;

        switch (letter)
        {
        case letters::space<CharT>:
        case letters::carriageReturn<CharT>:
        case letters::tab<CharT>:
            continue;
        case letters::leftCurleyBrace<CharT>:
            _token.type = TType::beginObject;
            return;
        case letters::rightCurleyBrace<CharT>:
            _token.type = TType::endObject;
            return;
        case letters::leftSquareBracket<CharT>:
            _token.type = TType::beginArray;
            return;
        case letters::rightSquareBracket<CharT>:
            _token.type = TType::endArray;
            return;
        case letters::comma<CharT>:
            _token.type = TType::valueSeparator;
            return;
        case letters::colon<CharT>:
            _token.type = TType::keyValueSeparator;
            return;
        case letters::singleQuote<CharT>:
        case letters::doubleQuote<CharT>:
            return _string(letter);
        }
    }
}

template<typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::_string(CharT boundary)
{
    using TType = typename Token<CharT>::Type;

    while (_begin != _end)
    {
        CharT letter = *_begin;
        ++_begin;

        if (letter == boundary)
        {
            _token.type = TType::string;
            _token.data = std::move(_stringBuffer);
            return;
        }

        _stringBuffer += letter;
    }
}

template<typename CharT, typename IterT>
Token<CharT> &Tokenizer<CharT, IterT>::token()
{
    return _token;
}

template<typename CharT, typename IterT>
Tokenizer<CharT, IterT>::operator bool()
{
    return _begin != _end;
}
} // namespace json::token
