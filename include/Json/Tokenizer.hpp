#pragma once

#include "Json/Keywords.hpp"
#include "Json/Token.hpp"

namespace json::internals
{
enum class TokenizerPosition
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
    /**
     * Inspect a letter
     * @returns true if further action is needed
     */
    template<typename Iter, typename Callback>
    bool _inspectCharacter(CharT letter, Iter &iter, Callback &callback);

    /**
     * Inspect existing token
     * @returns true if further action is needed
     */
    template<typename Iter, typename Callback>
    bool _inspectExistingToken(Iter &iter, Callback &callback);

    void _reset();

    Token<CharT> _token;
    internals::TokenizerPosition _position;
};
} // namespace json

// Implementation

namespace json
{
template<typename CharT>
Tokenizer<CharT>::Tokenizer()
    : _position(internals::TokenizerPosition::undefined)
{
}

template<typename CharT>
template<typename Iter, typename Callback>
void Tokenizer<CharT>::tokenize(Iter begin, Iter end, const Callback &callback)
{
    using std::cout;
    using std::endl;

    using namespace internals;

    while (begin != end)
    {
        CharT letter = *begin;

        if (_inspectCharacter(*begin, begin, callback))
        {
            _inspectExistingToken(begin, callback);
        }

        ++begin;
    }
}

template<typename CharT>
template<typename Iter, typename Callback>
bool Tokenizer<CharT>::_inspectCharacter(
    CharT letter, Iter &iter, Callback &callback)
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
        if (_position == TokenizerPosition::singeLineComment)
        {
            callback(_token);
            _reset();
        }

        return false;
    }
    case Keywords<CharT>::space:
    {
        if (_position == TokenizerPosition::singeLineComment
            || _position == TokenizerPosition::multiLineComment
            || _position == TokenizerPosition::string)
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
        // if (_position == TokenizerPosition::string)
        // {
        //     _position = TokenizerPosition::undefined;
        // }
        // else
        // {
        //     _position = TokenizerPosition::string;
        // }
        switch (_position)
        {
        case TokenizerPosition::string:
            _position = TokenizerPosition::undefined;
            break;
        default:
            _position = TokenizerPosition::string;
            break;
        }

        return false;
    }
    case Keywords<CharT>::doubleQuote:
    {
        if (_position == TokenizerPosition::string)
        {
            _position = TokenizerPosition::undefined;
        }
        else
        {
            _position = TokenizerPosition::string;
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
template<typename Iter, typename Callback>
bool Tokenizer<CharT>::_inspectExistingToken(Iter &iter, Callback &callback)
{
    using namespace internals;

    if (_token.data == Keywords<CharT>::singleLineComment)
    {
        _token.reset();
        _token.type = TokenType::comment;
        _position = TokenizerPosition::singeLineComment;
        ++iter;
    }

    return false;
}

template<typename CharT>
void Tokenizer<CharT>::_reset()
{
    using namespace internals;

    _token.reset();
    _position = TokenizerPosition::undefined;
}
} // namespace json