//
//  Tokenizer.hpp
//  CPP
//
//  Created by Zehua Chen on 4/15/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#pragma once

#include "json/token/token.h"
#include "json/utils/convert.h"
#include "json/utils/letters.h"
#include <bitset>
#include <cmath>
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
    void _number();
    void _true();
    void _false();
    void _null();

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

        switch (letter)
        {
        case ' ':
        case '\r':
        case '\t':
        case '\n':
            ++_begin;
            continue;
        case '{':
            _token.type = TType::beginObject;
            ++_begin;
            return;
        case '}':
            _token.type = TType::endObject;
            ++_begin;
            return;
        case '[':
            _token.type = TType::beginArray;
            ++_begin;
            return;
        case ']':
            _token.type = TType::endArray;
            ++_begin;
            return;
        case ',':
            _token.type = TType::valueSeparator;
            ++_begin;
            return;
        case ':':
            _token.type = TType::keyValueSeparator;
            ++_begin;
            return;
        case '\"':
            ++_begin;
            return _string();
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return _number();
        case 't':
            return _true();
        case 'f':
            return _false();
        case 'n':
            return _null();
        default:
            // TODO: Error handling
            ++_begin;
            break;
        }
    }
}

template<typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::_string()
{
    enum class State
    {
        regular,
        escape,
        hex,
    };

    using namespace json::utils;
    using namespace json::utils::convert;

    using TType = typename Token<CharT>::Type;

    int8_t hexCounter = 0;
    int8_t hexAppendCount = 0;
    int32_t hexValue = 0;
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
            case '\"':
                _token.type = TType::string;
                _token.data = std::move(buffer);
                return;
            // start of escape sequence
            case '\\':
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
                buffer += letters::backspace<CharT>;
                break;
            case letters::f<CharT>:
                state = State::regular;
                buffer += letters::formfeed<CharT>;
                break;
            case letters::n<CharT>:
                state = State::regular;
                buffer += letters::endline<CharT>;
                break;
            case letters::r<CharT>:
                state = State::regular;
                buffer += letters::carriageReturn<CharT>;
                break;
            case letters::t<CharT>:
                state = State::regular;
                buffer += letters::tab<CharT>;
                break;
            case letters::solidus<CharT>:
                state = State::regular;
                buffer += letters::solidus<CharT>;
                break;
            case letters::backSolidus<CharT>:
                state = State::regular;
                buffer += letters::backSolidus<CharT>;
                break;
            case letters::doubleQuote<CharT>:
                state = State::regular;
                buffer += letters::doubleQuote<CharT>;
                break;
            case letters::u<CharT>:
                state = State::hex;
                break;
            }
            break;
        case State::hex:
        {
            ++hexCounter;
            // stop after enough hex
            auto currentHexValue = number::fromHex<int8_t>(letter);

            // TODO:
            // handle error if currentHexValue is -1

            // utf8
            if constexpr (sizeof(CharT) == 1)
            {
                hexValue <<= 4;
                hexValue |= currentHexValue;

                ++hexAppendCount;

                if (hexAppendCount == 2)
                {
                    buffer += hexValue;

                    hexValue = 0;
                    hexAppendCount = 0;
                }
            }
            // utf16 and utf32
            else
            {
                hexValue <<= 4;
                hexValue |= currentHexValue;

                ++hexAppendCount;

                if (hexAppendCount == 4)
                {
                    buffer += hexValue;

                    hexValue = 0;
                    hexAppendCount = 0;
                }
            }

            if (hexCounter == 4)
            {
                state = State::regular;
                hexCounter = 0;
            }
            break;
        }
        }
    }
}

template<typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::_number()
{
    using namespace utils::convert;
    // Number format:
    // -010.2e+3
    enum class State
    {
        undefined,
        beforeDecimalPoint,
        afterDecimalPoint,
        afterE,
        afterESign,
    };

    State state = State::undefined;
    double value = 0.0;
    double sign = 1;
    double postDecimalScale = 0.1;
    double scaleSign = 1.0;
    double scale = 0.0;

    const auto exportNumber = [&]() {
        scale *= scaleSign;
        return value * sign * std::pow(10, scale);
    };

    while (_begin != _end)
    {
        CharT letter = *_begin;

        switch (letter)
        {
        case ']':
        case '}':
        case ' ':
        case ',':
        case '\t':
        case '\r':
        case '\n':
            return _token.formNumber(exportNumber());
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            switch (state)
            {
            case State::undefined:
                value *= 10.0;
                value += number::fromDec<double>(letter);
                state = State::beforeDecimalPoint;
                ++_begin;
                break;
            case State::beforeDecimalPoint:
                value *= 10.0;
                value += number::fromDec<double>(letter);
                ++_begin;
                break;
            case State::afterDecimalPoint:
                value += number::fromDec<double>(letter) * postDecimalScale;
                postDecimalScale /= 10.0;
                ++_begin;
                break;
            case State::afterE:
                state = State::afterESign;
                break;
            case State::afterESign:
                scale *= 10;
                scale += number::fromDec<double>(letter);
                ++_begin;
                break;
            default:
                ++_begin;
                break;
            }
            break;
        case 'e':
        case 'E':
            switch (state)
            {
            case State::undefined:
            case State::beforeDecimalPoint:
            case State::afterDecimalPoint:
                state = State::afterE;
                break;
            default:
                break;
            }
            ++_begin;
            break;
        case '.':
            switch (state)
            {
            case State::undefined:
            case State::beforeDecimalPoint:
                state = State::afterDecimalPoint;
                break;
            default:
                break;
            }
            ++_begin;
            break;
        case '-':
            switch (state)
            {
            case State::undefined:
                sign = -1;
                state = State::beforeDecimalPoint;
                break;
            case State::afterE:
                scaleSign = -1;
                state = State::afterESign;
                break;
            default:
                break;
            }
            ++_begin;
            break;
        default:
            break;
        }
    }

    _token.formNumber(exportNumber());
}

template<typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::_true()
{
    const char letters[] = { 't', 'r', 'u', 'e' };
    int i = 0;

    while (_begin != _end)
    {
        CharT letter = *_begin;

        if (letter != static_cast<CharT>(letters[i]))
        {
            // TODO: Error handling
        }

        if (i == 3)
        {
            ++_begin;
            return _token.formBoolean(true);
        }

        ++i;
        ++_begin;
    }

    // TODO: Error handling
}

template<typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::_false()
{
    const char letters[] = { 'f', 'a', 'l', 's', 'e' };
    int i = 0;

    while (_begin != _end)
    {
        CharT letter = *_begin;

        if (letter != static_cast<CharT>(letters[i]))
        {
            // TODO: Error handling
        }

        if (i == 4)
        {
            ++_begin;
            return _token.formBoolean(false);
        }

        ++i;
        ++_begin;
    }

    // TODO: Error handling
}

template<typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::_null()
{
    const char letters[] = { 'n', 'u', 'l', 'l' };
    int i = 0;

    while (_begin != _end)
    {
        CharT letter = *_begin;

        if (letter != static_cast<CharT>(letters[i]))
        {
            // TODO: Error handling
        }

        if (i == 3)
        {
            _token.type = Token<CharT>::Type::null;
            ++_begin;
            return;
        }

        ++i;
        ++_begin;
    }

    // TODO: Error handling
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
