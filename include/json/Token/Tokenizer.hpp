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

    void _string();

    Token<CharT> _token;
    IterT _begin;
    IterT _end;
};
} // namespace json::token

// Implementations

namespace json::token
{
template<typename CharT, typename IterT>
Tokenizer<CharT, IterT>::Tokenizer(IterT begin, IterT end)
    : _begin(begin)
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
        case letters::doubleQuote<CharT>:
            return _string();
        }
    }
}

template<typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::_string()
{
    enum class State
    {
        regular,
        escape
    };
    
    using namespace json::utils;
    using TType = typename Token<CharT>::Type;
    
    State state = State::regular;
    std::basic_string<CharT> buffer;
    
    while (_begin != _end)
    {
        CharT letter = *_begin;
        ++_begin;
        
        switch (state)
        {
        case State::regular:
            switch (letter)
            {
            // end of string
            case letters::doubleQuote<CharT>:
                _token.type = TType::string;
                _token.data = std::move(buffer);
                return;
            // start of escape sequence
            case letters::solidus<CharT>:
                state = State::escape;
                break;
            // regular text
            default:
                buffer += letter;
            }
            
            break;
        case State::escape:
            switch (letter)
            {
            case letters::b<CharT>:
                state = State::regular;
                buffer += '\b';
                break;
            case letters::f<CharT>:
                state = State::regular;
                buffer += '\f';
                break;
            case letters::n<CharT>:
                state = State::regular;
                buffer += '\n';
                break;
            case letters::r<CharT>:
                state = State::regular;
                buffer += '\r';
                break;
            case letters::t<CharT>:
                state = State::regular;
                buffer += '\t';
                break;
            case letters::solidus<CharT>:
                state = State::regular;
                buffer += '\\';
                break;
            case letters::backSolidus<CharT>:
                state = State::regular;
                buffer += '/';
                break;
            }
            break;
        }
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
