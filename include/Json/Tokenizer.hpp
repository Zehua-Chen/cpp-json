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
template<typename CharT, typename AssemblerT>
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
    
    AssemblerT &assembler();

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
    void _inspectLetter(CharT letter);

    // State handlers

    void _handleOtherState(CharT letter);

    void _handleSingleQuoteStringState(CharT letter);

    void _handleDoubleQuoteStringState(CharT letter);

    void _handleNonStringLiteralState(CharT letter);

    void _handleMaybeCommentState(CharT letter);

    void _handleSingleLineCommentState(CharT letter);

    void _handleMultiLineCommentState(CharT letter);

    void _handleMultiLineCommentEndingState(CharT letter);

    /**
     * Determine if a letter can be inside a string literals
     * @returns true if letter is between A-Z, a-z, 0-9
     */
    bool _canLetterBeNonStringLiteral(CharT letter);

    Token<CharT> _token;
    _State _state = _State::other;
    AssemblerT _assembler;
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
template<typename CharT, typename AssemblerT>
template<typename Iter>
void Tokenizer<CharT, AssemblerT>::tokenize(Iter begin, Iter end)
{
    bool result = true;

    while (begin != end)
    {
        _inspectLetter(*begin);

        ++begin;
    }

    // Handle json text with a single string
    //   _token is not empty after loop is done
    if (!_token.data.empty())
    {
        _token.type = Token<CharT>::Type::value;
        _assembler(_token);
    }
}

template<typename CharT, typename AssemblerT>
AssemblerT &Tokenizer<CharT, AssemblerT>::assembler()
{
    return _assembler;
}

/**
 * Given a the current state, invoke the appropriate state handler
 * @param letter the input
 */
template<typename CharT, typename AssemblerT>
void Tokenizer<CharT, AssemblerT>::_inspectLetter(CharT letter)
{
    switch (_state)
    {
    case _State::other:
        _handleOtherState(letter);
        break;
    case _State::singleQuoteString:
        _handleSingleQuoteStringState(letter);
        break;
    case _State::doubleQuoteString:
        _handleDoubleQuoteStringState(letter);
        break;
    case _State::nonStringLiteral:
        _handleNonStringLiteralState(letter);
        break;
    case _State::maybeComment:
        _handleMaybeCommentState(letter);
        break;
    case _State::singleLineComment:
        _handleSingleLineCommentState(letter);
        break;
    case _State::multiLineComment:
        _handleMultiLineCommentState(letter);
        break;
    case _State::multiLineCommentEnding:
        _handleMultiLineCommentEndingState(letter);
        break;
    }
}

template<typename CharT, typename AssemblerT>
void Tokenizer<CharT, AssemblerT>::_handleOtherState(CharT letter)
{
    using namespace json;

    switch (letter)
    {
    case keywords::leftCurlyBrace<CharT>:
    {
        _token.type = Token<CharT>::Type::beginObject;
        _assembler(_token);

        return;
    }
    case keywords::rightCurlyBrace<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            _assembler(_token);
            _token.data.clear();
        }

        _token.type = Token<CharT>::Type::endObject;
        _assembler(_token);

        return;
    }
    case keywords::leftSquareBracket<CharT>:
    {
        _token.type = Token<CharT>::Type::beginArray;
        _assembler(_token);

        return;
    }
    case keywords::rightSquareBracket<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            _assembler(_token);
            _token.data.clear();
        }

        _token.type = Token<CharT>::Type::endArray;
        _assembler(_token);

        return;
    }
    case keywords::colon<CharT>:
    {
        _token.type = Token<CharT>::Type::key;
        _assembler(_token);
        _token.data.clear();

        return;
    }
    case keywords::comma<CharT>:
    {
        if (!_token.data.empty())
        {
            _token.type = Token<CharT>::Type::value;
            _assembler(_token);
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

template<typename CharT, typename AssemblerT>
void Tokenizer<CharT, AssemblerT>::_handleSingleQuoteStringState(CharT letter)
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

template<typename CharT, typename AssemblerT>
void Tokenizer<CharT, AssemblerT>::_handleDoubleQuoteStringState(CharT letter)
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

template<typename CharT, typename AssemblerT>
void Tokenizer<CharT, AssemblerT>::_handleNonStringLiteralState(CharT letter)
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
        _handleOtherState(letter);
    }
}

template<typename CharT, typename AssemblerT>
void Tokenizer<CharT, AssemblerT>::_handleMaybeCommentState(CharT letter)
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

template<typename CharT, typename AssemblerT>
void Tokenizer<CharT, AssemblerT>::_handleSingleLineCommentState(CharT letter)
{
    switch (letter)
    {
    case keywords::endline<CharT>:
        _state = _State::other;
        _token.type = Token<CharT>::Type::comment;
        _assembler(_token);
        _token.data.clear();
        break;
    case keywords::carriageReturn<CharT>:
        break;
    default:
        _token.data += letter;
        break;
    }
}

template<typename CharT, typename AssemblerT>
void Tokenizer<CharT, AssemblerT>::_handleMultiLineCommentState(CharT letter)
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

template<typename CharT, typename AssemblerT>
void Tokenizer<CharT, AssemblerT>::_handleMultiLineCommentEndingState(
    CharT letter)
{
    switch (letter)
    {
    case keywords::backSlash<CharT>:
        _token.data.pop_back();
        _state = _State::other;
        _token.type = Token<CharT>::Type::comment;
        _assembler(_token);
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
template<typename CharT, typename AssemblerT>
bool Tokenizer<CharT, AssemblerT>::_canLetterBeNonStringLiteral(CharT letter)
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