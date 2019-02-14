//
//  Tokenizer.hpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#pragma once

#include "Json/Keywords.hpp"
#include "Json/Token.hpp"
#include <iostream>

namespace json::token
{
template<typename CharT>
class Tokenizer
{
public:
    Tokenizer();

    template<typename Iter, typename Callback>
    void tokenize(Iter begin, Iter end, Callback &callback);

private:
    // Tokenizer pipeline
    // 1. Call _inspectCharacter
    // 2. Call _inspectExistingToken if _inspectCharacter returns true

    enum class _Context
    {
        singleLineComment,
        multiLineComment,
        singleQuoteString,
        doubleQuoteString,
        undefined,
    };

    /**
     * Inspect a letter
     * @returns true if further action is needed
     */
    template<typename Callback>
    bool _inspectCharacter(CharT letter, Callback &callback);

    /**
     * Inspect existing token
     * @returns true if further action is needed
     */
    template<typename Callback>
    bool _inspectExistingToken(Callback &callback);

    void _reset();
    void _skip(int count = 1);
    bool _shouldAppendLetter(CharT letter);

    Token<CharT> _token;
    _Context _context;
    int _skipCount;
};
} // namespace json::token

// Implementation

namespace json::token
{
template<typename CharT>
Tokenizer<CharT>::Tokenizer()
    : _context(_Context::undefined)
    , _skipCount(0)
{
}

template<typename CharT>
template<typename Iter, typename Callback>
void Tokenizer<CharT>::tokenize(Iter begin, Iter end, Callback &callback)
{
    bool result = true;

    while (begin != end)
    {
        if (_skipCount == 0)
        {
            result = result ? _inspectCharacter(*begin, callback) : false;
            result = result ? _inspectExistingToken(callback) : false;
        }
        else
        {
            --_skipCount;
        }

        ++begin;
        result = true;
    }

    // Handle json text with a single string
    //   _token is not empty after loop is done
    if (!_token.data.empty())
    {
        _token.type = Token<CharT>::Type::value;
        callback(_token);
    }
}

template<typename CharT>
template<typename Callback>
bool Tokenizer<CharT>::_inspectCharacter(CharT letter, Callback &callback)
{
    using namespace json;

    if (_shouldAppendLetter(letter))
    {
        _token.append(letter);
        
        // Only these two scnearios requries furthur actions
        if (_context == _Context::undefined
            || _context == _Context::multiLineComment)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    switch (letter)
    {
    case keywords::endline<CharT>:
    {
        // \n represnets the end of a single line comment
        if (_context == _Context::singleLineComment)
        {
            callback(_token);
            _reset();
        }

        return false;
    }
    case keywords::beginObject<CharT>:
    {
        _token.type = Token<CharT>::Type::beginObject;
        callback(_token);
        _reset();

        return false;
    }
    case keywords::endObject<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            callback(_token);

            _reset();
        }

        _token.type = Token<CharT>::Type::endObject;
        callback(_token);
        _reset();

        return false;
    }
    case keywords::beginArray<CharT>:
    {
        _token.type = Token<CharT>::Type::beginArray;
        callback(_token);
        _reset();

        return false;
    }
    case keywords::endArray<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            callback(_token);

            _reset();
        }

        _token.type = Token<CharT>::Type::endArray;
        callback(_token);
        _reset();

        return false;
    }
    case keywords::colon<CharT>:
    {
        _token.type = Token<CharT>::Type::key;
        callback(_token);
        _reset();

        return false;
    }
    case keywords::comma<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            callback(_token);
            _reset();
        }

        return false;
    }
    case keywords::singleQuote<CharT>:
    {
        switch (_context)
        {
        case _Context::singleQuoteString:
            _context = _Context::undefined;
            break;
        default:
            _context = _Context::singleQuoteString;
            break;
        }

        return false;
    }
    case keywords::doubleQuote<CharT>:
    {
        switch (_context)
        {
        case _Context::doubleQuoteString:
            _context = _Context::undefined;
            break;
        default:
            _context = _Context::doubleQuoteString;
            break;
        }

        return false;
    }
    default:
    {
        return false;
    }
    }
}

template<typename CharT>
template<typename Callback>
bool Tokenizer<CharT>::_inspectExistingToken(Callback &callback)
{
    using namespace json;

    if (_token.data == keywords::singleLineComment<CharT>)
    {
        if (_context != _Context::singleQuoteString
            && _context != _Context::doubleQuoteString
            && _context != _Context::multiLineComment)
        {
            _token.reset();
            _token.type = Token<CharT>::Type::comment;
            _context = _Context::singleLineComment;
        }

        return false;
    }
    else if (_token.data == keywords::beginMultiLineComment<CharT>)
    {
        _context = _Context::multiLineComment;
        _token.reset();
        _token.type = Token<CharT>::Type::comment;

        return false;
    }

    if (_context == _Context::multiLineComment)
    {
        // If ends with */
        std::basic_string<CharT> &buffer = _token.data;

        if (buffer.size() >= 2
            && buffer[buffer.size() - 2]
                == keywords::endMultiLineComment<CharT>[0]
            && buffer[buffer.size() - 1]
                == keywords::endMultiLineComment<CharT>[1])
        {
            // remove trailing */
            buffer.pop_back();
            buffer.pop_back();

            callback(_token);
            _reset();
        }
    }

    return false;
}

template<typename CharT>
void Tokenizer<CharT>::_reset()
{

    _token.reset();
    _context = _Context::undefined;
}

template<typename CharT>
void Tokenizer<CharT>::_skip(int count)
{
    _skipCount += count;
}

template<typename CharT>
bool Tokenizer<CharT>::_shouldAppendLetter(CharT letter)
{
    switch (_context)
    {
    case _Context::singleQuoteString:
    {
        switch (letter)
        {
        case keywords::singleQuote<CharT>:
            return false;
        case keywords::doubleQuote<CharT>:
            return true;
        case keywords::carriageReturn<CharT>:
            return false;
        default:
            return true;
        }
    }
    case _Context::doubleQuoteString:
    {
        switch (letter)
        {
        case keywords::singleQuote<CharT>:
            return true;
        case keywords::doubleQuote<CharT>:
            return false;
        case keywords::carriageReturn<CharT>:
            return false;
        default:
            return true;
        }
    }
    case _Context::multiLineComment:
    {
        switch (letter)
        {
        case keywords::carriageReturn<CharT>:
            return false;
        default:
            return true;
        }
    }
    case _Context::singleLineComment:
    {
        switch (letter)
        {
        case keywords::endline<CharT>:
            return false;
        case keywords::carriageReturn<CharT>:
            return false;
        default:
            return true;
        }
    }
    case _Context::undefined:
    {
        switch (letter)
        {
        case keywords::beginObject<CharT>:
            return false;
        case keywords::endObject<CharT>:
            return false;
        case keywords::beginArray<CharT>:
            return false;
        case keywords::endArray<CharT>:
            return false;
        case keywords::carriageReturn<CharT>:
            return false;
        case keywords::colon<CharT>:
            return false;
        case keywords::tab<CharT>:
            return false;
        case keywords::endline<CharT>:
            return false;
        case keywords::space<CharT>:
            return false;
        case keywords::singleQuote<CharT>:
            return false;
        case keywords::doubleQuote<CharT>:
            return false;
        case keywords::comma<CharT>:
            return false;
        }

        return true;
    }
    default:
        return false;
    }
}
} // namespace json::token