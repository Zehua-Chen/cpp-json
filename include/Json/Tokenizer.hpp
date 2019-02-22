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
template<typename CharT, typename OutputT>
class Tokenizer
{
public:
    // Tokenizer();

    /**
     * Tokenize strings in [begin, end)
     * @param begin inclusive start of string
     * @param end exclusive end of string
     */
    template<typename Iter>
    void tokenize(Iter begin, Iter end);
    
    OutputT &output();

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
     */
    void _inspectCurrentLetter();

    // State handlers

    void _handleOtherState();

    void _handleSingleQuoteStringState();

    void _handleDoubleQuoteStringState();

    void _handleNonStringLiteralState();

    void _handleMaybeCommentState();

    void _handleSingleLineCommentState();

    void _handleMultiLineCommentState();

    void _handleMultiLineCommentEndingState();

    /**
     * Determine if a letter can be inside a string literals
     * @returns true if letter is between A-Z, a-z, 0-9
     */
    bool _canLetterBeNonStringLiteral(CharT letter);

    Token<CharT> _token;
    _State _state = _State::other;
    CharT _currentLetter;
    OutputT _output;
};
} // namespace json::token

// Implementation

namespace json::token
{

/**
 * Tokenize strings in [begin, end)
 * @param begin inclusive start of string
 * @param end exclusive end of string
 */
template<typename CharT, typename OutputT>
template<typename Iter>
void Tokenizer<CharT, OutputT>::tokenize(Iter begin, Iter end)
{
    bool result = true;

    while (begin != end)
    {
        _currentLetter = *begin;
        _inspectCurrentLetter();

        ++begin;
    }

    // Handle json text with a single string
    //   _token is not empty after loop is done
    if (!_token.data.empty())
    {
        _token.type = Token<CharT>::Type::value;
        _output(_token);
    }
}

template<typename CharT, typename OutputT>
OutputT &Tokenizer<CharT, OutputT>::output()
{
    return _output;
}

/**
 * Given a the current state, invoke the appropriate state handler
 * @param letter the input
 */
template<typename CharT, typename OutputT>
void Tokenizer<CharT, OutputT>::_inspectCurrentLetter()
{
    switch (_state)
    {
    case _State::other:
        _handleOtherState();
        break;
    case _State::singleQuoteString:
        _handleSingleQuoteStringState();
        break;
    case _State::doubleQuoteString:
        _handleDoubleQuoteStringState();
        break;
    case _State::nonStringLiteral:
        _handleNonStringLiteralState();
        break;
    case _State::maybeComment:
        _handleMaybeCommentState();
        break;
    case _State::singleLineComment:
        _handleSingleLineCommentState();
        break;
    case _State::multiLineComment:
        _handleMultiLineCommentState();
        break;
    case _State::multiLineCommentEnding:
        _handleMultiLineCommentEndingState();
        break;
    }
}

template<typename CharT, typename OutputT>
void Tokenizer<CharT, OutputT>::_handleOtherState()
{
    using namespace json;

    switch (_currentLetter)
    {
    case keywords::leftCurlyBrace<CharT>:
    {
        _token.type = Token<CharT>::Type::beginObject;
        _output(_token);

        return;
    }
    case keywords::rightCurlyBrace<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            _output(_token);
            _token.data.clear();
        }

        _token.type = Token<CharT>::Type::endObject;
        _output(_token);

        return;
    }
    case keywords::leftSquareBracket<CharT>:
    {
        _token.type = Token<CharT>::Type::beginArray;
        _output(_token);

        return;
    }
    case keywords::rightSquareBracket<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            _output(_token);
            _token.data.clear();
        }

        _token.type = Token<CharT>::Type::endArray;
        _output(_token);

        return;
    }
    case keywords::colon<CharT>:
    {
        _token.type = Token<CharT>::Type::key;
        _output(_token);
        _token.data.clear();

        return;
    }
    case keywords::comma<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            _output(_token);
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

    if (_canLetterBeNonStringLiteral(_currentLetter))
    {
        _token.data += _currentLetter;
        _state = _State::nonStringLiteral;
    }
}

template<typename CharT, typename OutputT>
void Tokenizer<CharT, OutputT>::_handleSingleQuoteStringState()
{
    switch (_currentLetter)
    {
    case keywords::singleQuote<CharT>:
        _state = _State::other;
        break;
    default:
        _token.data += _currentLetter;
        break;
    }
}

template<typename CharT, typename OutputT>
void Tokenizer<CharT, OutputT>::_handleDoubleQuoteStringState()
{
    switch (_currentLetter)
    {
    case keywords::doubleQuote<CharT>:
        _state = _State::other;
        break;
    default:
        _token.data += _currentLetter;
        break;
    }
}

template<typename CharT, typename OutputT>
void Tokenizer<CharT, OutputT>::_handleNonStringLiteralState()
{
    if (_canLetterBeNonStringLiteral(_currentLetter))
    {
        _token.data += _currentLetter;
    }
    else
    {
        // Due to the fact that non string literals do not have border letters,
        // the next letter cannot be discarded.
        _state = _State::other;
        _handleOtherState();
    }
}

template<typename CharT, typename OutputT>
void Tokenizer<CharT, OutputT>::_handleMaybeCommentState()
{
    switch (_currentLetter)
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

template<typename CharT, typename OutputT>
void Tokenizer<CharT, OutputT>::_handleSingleLineCommentState()
{
    switch (_currentLetter)
    {
    case keywords::endline<CharT>:
        _state = _State::other;
        _token.type = Token<CharT>::Type::comment;
        _output(_token);
        _token.data.clear();
        break;
    case keywords::carriageReturn<CharT>:
        break;
    default:
        _token.data += _currentLetter;
        break;
    }
}

template<typename CharT, typename OutputT>
void Tokenizer<CharT, OutputT>::_handleMultiLineCommentState()
{
    switch (_currentLetter)
    {
    case keywords::star<CharT>:
        _state = _State::multiLineCommentEnding;
        _token.data += _currentLetter;
        break;
    // case keywords::carriageReturn<CharT>:
    //     break;
    default:
        _token.data += _currentLetter;
        break;
    }
}

template<typename CharT, typename OutputT>
void Tokenizer<CharT, OutputT>::_handleMultiLineCommentEndingState()
{
    switch (_currentLetter)
    {
    case keywords::backSlash<CharT>:
        _token.data.pop_back();
        _state = _State::other;
        _token.type = Token<CharT>::Type::comment;
        _output(_token);
        _token.data.clear();
        break;
    default:
        _token.data += _currentLetter;
        _state = _State::multiLineComment;
        break;
    }
}

/**
 * Determine if a letter can be inside a string literals
 * @returns true if letter is between A-Z, a-z, 0-9
 */
template<typename CharT, typename OutputT>
bool Tokenizer<CharT, OutputT>::_canLetterBeNonStringLiteral(CharT letter)
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