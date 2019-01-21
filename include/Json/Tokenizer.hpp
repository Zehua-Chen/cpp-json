#pragma once

#include "Json/Keywords.hpp"
#include "Json/Token.hpp"
#include <iostream>

namespace json::internals
{
enum class Context
{
    singeLineComment,
    multiLineComment,
    string,
    undefined,
};
}

namespace json
{
template<typename CharT>
class Tokenizer
{
public:
    Tokenizer();

    template<typename Iter, typename Callback>
    void tokenize(Iter begin, Iter end, const Callback &callback);

private:
    // Tokenizer pipeline
    // 1. Call _inspectCharacter
    // 2. Call _inspectExistingToken if _inspectCharacter returns true

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

    Token<CharT> _token;
    internals::Context _context;
    int _skipCount;
};
} // namespace json

// Implementation

namespace json
{
template<typename CharT>
Tokenizer<CharT>::Tokenizer()
    : _context(internals::Context::undefined)
    , _skipCount(0)
{
}

template<typename CharT>
template<typename Iter, typename Callback>
void Tokenizer<CharT>::tokenize(Iter begin, Iter end, const Callback &callback)
{
    using std::bitset;

    using namespace internals;

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
        _token.type = TokenType::value;
        callback(_token);
    }
}

template<typename CharT>
template<typename Callback>
bool Tokenizer<CharT>::_inspectCharacter(CharT letter, Callback &callback)
{
    using namespace internals;

    switch (letter)
    {
    case Keywords<CharT>::carriageReturn:
    {
        return false;
    }
    case Keywords<CharT>::endline:
    {
        // \n represnets the end of a single line comment
        if (_context == Context::singeLineComment)
        {
            callback(_token);
            _reset();
        }
        // \n does not mean the end of a multiline comment
        if (_context == Context::multiLineComment)
        {
            _token.append(letter);
        }

        return false;
    }
    case Keywords<CharT>::space:
    {
        if (_context == Context::singeLineComment
            || _context == Context::multiLineComment
            || _context == Context::string)
        {
            _token.append(letter);
        }

        return false;
    }
    case Keywords<CharT>::tab:
    {
        return false;
    }
    case Keywords<CharT>::beginObject:
    {
        _token.type = TokenType::beginObject;
        callback(_token);
        _reset();

        return false;
    }
    case Keywords<CharT>::endObject:
    {
        if (!_token.data.empty())
        {
            _token.type = TokenType::value;
            callback(_token);

            _reset();
        }

        _token.type = TokenType::endObject;
        callback(_token);
        _reset();

        return false;
    }
    case Keywords<CharT>::beginArray:
    {
        _token.type = TokenType::beginArray;
        callback(_token);
        _reset();

        return false;
    }
    case Keywords<CharT>::endArray:
    {
        if (!_token.data.empty())
        {
            _token.type = TokenType::value;
            callback(_token);

            _reset();
        }

        _token.type = TokenType::endArray;
        callback(_token);
        _reset();

        return false;
    }
    case Keywords<CharT>::keyValuesSeparator:
    {
        _token.type = TokenType::key;
        callback(_token);
        _reset();

        return false;
    }
    case Keywords<CharT>::propertiesSeparator:
    {
        _token.type = TokenType::value;
        callback(_token);
        _reset();

        return false;
    }
    case Keywords<CharT>::singleQuote:
    {
        // if (_context == Context::string)
        // {
        //     _context = Context::undefined;
        // }
        // else
        // {
        //     _context = Context::string;
        // }
        switch (_context)
        {
        case Context::string:
            _context = Context::undefined;
            break;
        default:
            _context = Context::string;
            break;
        }

        return false;
    }
    case Keywords<CharT>::doubleQuote:
    {
        if (_context == Context::string)
        {
            _context = Context::undefined;
        }
        else
        {
            _context = Context::string;
        }

        return false;
    }
    // Other characters
    default:
    {
        _token.append(letter);
        return true;
    }
    }
}

template<typename CharT>
template<typename Callback>
bool Tokenizer<CharT>::_inspectExistingToken(Callback &callback)
{
    using namespace internals;

    if (_token.data == Keywords<CharT>::singleLineComment)
    {
        _token.reset();
        _token.type = TokenType::comment;
        _context = Context::singeLineComment;
        _skip();

        return false;
    }
    else if (_token.data == Keywords<CharT>::beginMultilineComment)
    {
        _context = Context::multiLineComment;
        _token.reset();
        _token.type = TokenType::comment;

        return false;
    }

    if (_context == Context::multiLineComment)
    {
        auto found
            = _token.data.rfind(Keywords<CharT>::endMultilineComment.data());
        // If ends with */
        if (found != std::basic_string_view<CharT>::npos)
        {
            // remove trailing */
            _token.data.erase(found);
            callback(_token);
            _reset();
        }
    }

    return false;
}

template<typename CharT>
void Tokenizer<CharT>::_reset()
{
    using namespace internals;

    _token.reset();
    _context = Context::undefined;
}

template<typename CharT>
void Tokenizer<CharT>::_skip(int count)
{
    _skipCount += count;
}
} // namespace json