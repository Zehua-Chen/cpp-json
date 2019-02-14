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
#include <type_traits>

namespace json::token
{
template<typename CharT>
class Tokenizer
{
public:
    // Tokenizer();

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

    enum class _State
    {
        undefined,
        singleQuoteString,
        doubleQuoteString,
        maybeComment,
        singleLineComment,
        multiLineComment,
        multiLineCommentEnding,
    };

    template<typename Callback>
    void _handleUndefinedState(CharT letter, Callback &callback);

    template<typename Callback>
    void _handleSingleQuoteStringState(CharT letter, Callback &callback);

    template<typename Callback>
    void _handleDoubleQuoteStringState(CharT letter, Callback &callback);
    
    template<typename Callback>
    void _handleMaybeCommentState(CharT letter, Callback &callback);
    
    template<typename Callback>
    void _handleSingleLineCommentState(CharT letter, Callback &callback);
    
    template<typename Callback>
    void _handleMultiLineCommentState(CharT letter, Callback &callback);
    
    template<typename Callback>
    void _handleMultiLineCommentEndingState(CharT letter, Callback &callback);

    template<typename Callback>
    void _inspectLetter(CharT letter, Callback &callback);


    void _reset();
    void _skip(int count = 1);
    bool _shouldAppendLetter(CharT letter);

    Token<CharT> _token;
    std::basic_string<CharT> _keyword;
    _Context _context = _Context::undefined;
    int _skipCount = 0;

    _State _state = _State::undefined;
};
} // namespace json::token

// Implementation

namespace json::token
{
template<typename CharT>
template<typename Iter, typename Callback>
void Tokenizer<CharT>::tokenize(Iter begin, Iter end, Callback &callback)
{
    bool result = true;

    while (begin != end)
    {
        _inspectLetter(*begin, callback);

        ++begin;
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
void Tokenizer<CharT>::_inspectLetter(CharT letter, Callback &callback)
{
    switch (_state)
    {
    case _State::undefined:
        _handleUndefinedState(letter, callback);
        break;
    case _State::singleQuoteString:
        _handleSingleQuoteStringState(letter, callback);
        break;
    case _State::doubleQuoteString:
        _handleDoubleQuoteStringState(letter, callback);
        break;
    case _State::maybeComment:
        _handleMaybeCommentState(letter, callback);
        break;
    case _State::singleLineComment:
        _handleSingleLineCommentState(letter, callback);
        break;
    case _State::multiLineComment:
        _handleMultiLineCommentState(letter, callback);
        break;
    case _State::multiLineCommentEnding:
        _handleMultiLineCommentEndingState(letter, callback);
        break;
    }
}

template<typename CharT>
template<typename Callback>
void Tokenizer<CharT>::_handleUndefinedState(CharT letter, Callback &callback)
{
    using namespace json;
    
    switch (letter)
    {
    case keywords::beginObject<CharT>:
    {
        _token.type = Token<CharT>::Type::beginObject;
        callback(_token);

        break;
    }
    case keywords::endObject<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            callback(_token);
            _token.data.clear();
        }

        _token.type = Token<CharT>::Type::endObject;
        callback(_token);

        break;
    }
    case keywords::beginArray<CharT>:
    {
        _keyword.clear();

        _token.type = Token<CharT>::Type::beginArray;
        callback(_token);
        _reset();

        break;
    }
    case keywords::endArray<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            callback(_token);
            _token.data.clear();
        }

        _token.type = Token<CharT>::Type::endArray;
        callback(_token);
        _reset();

        break;
    }
    case keywords::colon<CharT>:
    {
        _token.type = Token<CharT>::Type::key;
        callback(_token);
        _token.data.clear();

        break;
    }
    case keywords::comma<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            callback(_token);
            _token.data.clear();
        }

        break;
    }
    case keywords::singleQuote<CharT>:
    {
        switch (_state)
        {
        case _State::undefined:
            _state = _State::singleQuoteString;
            break;
        case _State::singleQuoteString:
            _state = _State::undefined;
            break;
        default:
            break;
        }
        
        break;
    }
    case keywords::doubleQuote<CharT>:
    {
        switch (_state)
        {
        case _State::undefined:
            _state = _State::doubleQuoteString;
            break;
        case _State::doubleQuoteString:
            _state = _State::undefined;
            break;
        default:
            break;
        }

        break;
    }
    case keywords::backSlash<CharT>:
        _state = _State::maybeComment;
        _token.data += letter;
        break;
    case keywords::space<CharT>:
        break;
    case keywords::tab<CharT>:
        break;
    case keywords::star<CharT>:
        break;
    case keywords::endline<CharT>:
        break;
    case keywords::carriageReturn<CharT>:
        break;
    default:
        _token.data += letter;
        break;
    }
}

template<typename CharT>
template<typename Callback>
void Tokenizer<CharT>::_handleSingleQuoteStringState(
    CharT letter, Callback &callback)
{
    switch (letter)
    {
    case keywords::singleQuote<CharT>:
        _state = _State::undefined;
        break;
    default:
        _token.data += letter;
        break;
    }
}

template<typename CharT>
template<typename Callback>
void Tokenizer<CharT>::_handleDoubleQuoteStringState(
    CharT letter, Callback &callback)
{
    switch (letter)
    {
    case keywords::doubleQuote<CharT>:
        _state = _State::undefined;
        break;
    default:
        _token.data += letter;
        break;
    }
}

template<typename CharT>
template<typename Callback>
void Tokenizer<CharT>::_handleMaybeCommentState(
    CharT letter, Callback &callback)
{
    switch (letter)
    {
    case keywords::backSlash<CharT>:
        _state = _State::singleLineComment;
        _token.data.clear();
        break;
    case keywords::star<CharT>:
        _state = _State::multiLineComment;
        _token.data.clear();
        break;
    default:
        _state = _State::undefined;
        _token.data += letter;
        break;
    }
}

template<typename CharT>
template<typename Callback>
void Tokenizer<CharT>::_handleSingleLineCommentState(CharT letter, Callback &callback)
{
    switch (letter)
    {
    case keywords::endline<CharT>:
        _state = _State::undefined;
        _token.type = Token<CharT>::Type::comment;
        callback(_token);
        _token.data.clear();
        break;
    case keywords::carriageReturn<CharT>:
        break;
    default:
        _token.data += letter;
        break;
    }
}

template<typename CharT>
template<typename Callback>
void Tokenizer<CharT>::_handleMultiLineCommentState(CharT letter, Callback &callback)
{
    switch (letter)
    {
    case keywords::star<CharT>:
        _state = _State::multiLineCommentEnding;
        _token.data += letter;
        break;
    case keywords::carriageReturn<CharT>:
        break;
    default:
        _token.data += letter;
        break;
    }
}

template<typename CharT>
template<typename Callback>
void Tokenizer<CharT>::_handleMultiLineCommentEndingState(CharT letter, Callback &callback)
{
    switch (letter)
    {
    case keywords::backSlash<CharT>:
        _token.data.pop_back();
        _state = _State::undefined;
        _token.type = Token<CharT>::Type::comment;
        callback(_token);
        _token.data.clear();
        break;
    default:
        _token.data += letter;
        _state = _State::multiLineComment;
        break;
    }
}

template<typename CharT>
void Tokenizer<CharT>::_reset()
{
    _keyword.clear();
    _token.reset();
    _context = _Context::undefined;
}
} // namespace json::token