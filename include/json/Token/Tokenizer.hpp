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
        finished
    };

    void _start();

    Token<CharT> _token;
    _State _state;
    IterT _begin;
    IterT _end;
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
    switch (_state)
    {
    case _State::start:
        return _start();
    case _State::finished:
        break;
    }
}

template<typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::_start()
{
    using namespace utils;
    using TType = typename Token<CharT>::Type;
    
    switch (*_begin)
    {
    case letters::leftCurleyBrace<CharT>:
        _token.type = TType::beginObject;
        break;
    case letters::rightCurleyBrace<CharT>:
        _token.type = TType::endObject;
        break;
    case letters::leftSquareBracket<CharT>:
        _token.type = TType::beginArray;
        break;
    case letters::rightSquareBracket<CharT>:
        _token.type = TType::endArray;
        break;
    case letters::comma<CharT>:
        _token.type = TType::valueSeparator;
        break;
    case letters::colon<CharT>:
        _token.type = TType::keyValueSeparator;
        break;
    }
    
    ++_begin;
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
