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
/**
 * The tokenizer works like a state machine.
 *
 * At X state, _handleXState method will be called to handle the input at
 * X state.
 */
template<typename CharT>
class Tokenizer
{
public:
    // Tokenizer();

    /**
     * Tokenize strings in [begin, end), and spit the input through callback
     * @param begin inclusive start of string
     * @param end exclusive end of string
     * @param callback a function object that takes Token as parameter.
     */
    template<typename Iter, typename Callback>
    void tokenize(Iter begin, Iter end, Callback &callback);

private:
    enum class _State
    {
        singleQuoteString,
        doubleQuoteString,
        nonStringLiteral,
        maybeComment,
        singleLineComment,
        multiLineComment,
        multiLineCommentEnding,
        other,
    };

    /**
     * Given a the current state, invoke the appropriate state handler
     * @param letter the input
     * @param callback the callback through which tokens are spit out.
     */
    template<typename Callback>
    void _inspectLetter(CharT letter, Callback &callback);

    // State handlers

    template<typename Callback>
    void _handleOtherState(CharT letter, Callback &callback);

    template<typename Callback>
    void _handleSingleQuoteStringState(CharT letter, Callback &callback);

    template<typename Callback>
    void _handleDoubleQuoteStringState(CharT letter, Callback &callback);

    template<typename Callback>
    void _handleNonStringLiteralState(CharT letter, Callback &callback);

    template<typename Callback>
    void _handleMaybeCommentState(CharT letter, Callback &callback);

    template<typename Callback>
    void _handleSingleLineCommentState(CharT letter, Callback &callback);

    template<typename Callback>
    void _handleMultiLineCommentState(CharT letter, Callback &callback);

    template<typename Callback>
    void _handleMultiLineCommentEndingState(CharT letter, Callback &callback);

    /**
     * Determine if a letter can be inside a string literals
     * @returns true if letter is between A-Z, a-z, 0-9
     */
    bool _canLetterBeNonStringLiteral(CharT letter);

    Token<CharT> _token;
    _State _state = _State::other;
};
} // namespace json::token

// Implementation

namespace json::token
{

/**
 * Tokenize strings in [begin, end), and spit the input through callback
 * @param begin inclusive start of string
 * @param end exclusive end of string
 * @param callback a function object that takes Token as parameter.
 */
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

/**
 * Given a the current state, invoke the appropriate state handler
 * @param letter the input
 * @param callback the callback through which tokens are spit out.
 */
template<typename CharT>
template<typename Callback>
void Tokenizer<CharT>::_inspectLetter(CharT letter, Callback &callback)
{
    switch (_state)
    {
    case _State::other:
        _handleOtherState(letter, callback);
        break;
    case _State::singleQuoteString:
        _handleSingleQuoteStringState(letter, callback);
        break;
    case _State::doubleQuoteString:
        _handleDoubleQuoteStringState(letter, callback);
        break;
    case _State::nonStringLiteral:
        _handleNonStringLiteralState(letter, callback);
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
void Tokenizer<CharT>::_handleOtherState(CharT letter, Callback &callback)
{
    using namespace json;

    switch (letter)
    {
    case keywords::leftCurlyBrace<CharT>:
    {
        _token.type = Token<CharT>::Type::beginObject;
        callback(_token);

        return;
    }
    case keywords::rightCurlyBrace<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            callback(_token);
            _token.data.clear();
        }

        _token.type = Token<CharT>::Type::endObject;
        callback(_token);

        return;
    }
    case keywords::leftSquareBracket<CharT>:
    {
        _token.type = Token<CharT>::Type::beginArray;
        callback(_token);

        return;
    }
    case keywords::rightSquareBracket<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            callback(_token);
            _token.data.clear();
        }

        _token.type = Token<CharT>::Type::endArray;
        callback(_token);

        return;
    }
    case keywords::colon<CharT>:
    {
        _token.type = Token<CharT>::Type::key;
        callback(_token);
        _token.data.clear();

        return;
    }
    case keywords::comma<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            callback(_token);
            _token.data.clear();
        }

        return;
    }
    case keywords::singleQuote<CharT>:
    {
        _state = _State::singleQuoteString;

        return;
    }
    case keywords::doubleQuote<CharT>:
    {
        _state = _State::doubleQuoteString;
        
        return;
    }
    case keywords::backSlash<CharT>:
        _state = _State::maybeComment;
        return;  
    default:
        break;
    }

    if (_canLetterBeNonStringLiteral(letter))
    {
        _token.data += letter;
        _state = _State::nonStringLiteral;
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
        _state = _State::other;
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
        _state = _State::other;
        break;
    default:
        _token.data += letter;
        break;
    }
}

template<typename CharT>
template<typename Callback>
void Tokenizer<CharT>::_handleNonStringLiteralState(
    CharT letter, Callback &callback)
{
    if (_canLetterBeNonStringLiteral(letter))
    {
        _token.data += letter;
    }
    else
    {
        // Due to the fact that non string literals do not have border letters,
        // the next letter cannot be discarded.
        _state = _State::other;
        _handleOtherState(letter, callback);
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
        break;
    case keywords::star<CharT>:
        _state = _State::multiLineComment;
        break;
    default:
        break;
    }
}

template<typename CharT>
template<typename Callback>
void Tokenizer<CharT>::_handleSingleLineCommentState(
    CharT letter, Callback &callback)
{
    switch (letter)
    {
    case keywords::endline<CharT>:
        _state = _State::other;
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
void Tokenizer<CharT>::_handleMultiLineCommentState(
    CharT letter, Callback &callback)
{
    switch (letter)
    {
    case keywords::star<CharT>:
        _state = _State::multiLineCommentEnding;
        _token.data += letter;
        break;
    // case keywords::carriageReturn<CharT>:
    //     break;
    default:
        _token.data += letter;
        break;
    }
}

template<typename CharT>
template<typename Callback>
void Tokenizer<CharT>::_handleMultiLineCommentEndingState(
    CharT letter, Callback &callback)
{
    switch (letter)
    {
    case keywords::backSlash<CharT>:
        _token.data.pop_back();
        _state = _State::other;
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

/**
 * Determine if a letter can be inside a string literals
 * @returns true if letter is between A-Z, a-z, 0-9
 */
template<typename CharT>
bool Tokenizer<CharT>::_canLetterBeNonStringLiteral(CharT letter)
{
    if (letter >= keywords::A<CharT> && letter <= keywords::Z<CharT>)
    {
        return true;
    }

    if (letter >= keywords::a<CharT> && letter <= keywords::z<CharT>)
    {
        return true;
    }

    if (letter >= keywords::zero<CharT> && letter <= keywords::nine<CharT>)
    {
        return true;
    }
    
    if (letter == keywords::dot<CharT>)
    {
        return true;
    }

    return false;
}
} // namespace json::token