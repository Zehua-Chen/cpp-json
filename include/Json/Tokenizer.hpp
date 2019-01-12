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
    void tokenize(Iter begin, Iter end, Callback callback);

private:
    Token<CharT> _token;
    internals::TokenizerPosition _position;
};
} // namespace json

namespace json
{
template<typename CharT>
Tokenizer<CharT>::Tokenizer()
    : _position(internals::TokenizerPosition::undefined)
{
}

template<typename CharT>
template<typename Iter, typename Callback>
void Tokenizer<CharT>::tokenize(Iter begin, Iter end, Callback callback)
{
    using std::cout;
    using std::endl;
    using namespace internals;

    const auto reset = [&]() {
        _token.reset();
        _position = TokenizerPosition::undefined;
    };

    while (begin != end)
    {
        CharT letter = *begin;
        
        if (letter == Keywords<CharT>::carriageReturn)
        {
            ++begin;
            continue;
        }

        // Condition that will invoke the callback
        // \n
        if (letter == Keywords<CharT>::endline)
        {
            if (_position == TokenizerPosition::singeLineComment)
            {
                callback(_token);
                reset();
            }
        }
        // {
        else if (letter == Keywords<CharT>::beginObject)
        {
            _token.type = TokenType::beginObject;
            callback(_token);
            reset();
        }
        // }
        else if (letter == Keywords<CharT>::endObject)
        {
            if (!_token.data.empty())
            {
                _token.type = TokenType::value;
                callback(_token);
                reset();
            }

            _token.type = TokenType::endObject;
            callback(_token);
            reset();
        }
        // [
        else if (letter == Keywords<CharT>::beginArray)
        {
            _token.type = TokenType::beginArray;
            callback(_token);
            reset();
        }
        // ]
        else if (letter == Keywords<CharT>::endArray)
        {
            if (!_token.data.empty())
            {
                _token.type = TokenType::value;
                callback(_token);
                reset();
            }

            _token.type = TokenType::endArray;
            callback(_token);
            reset();
        }
        // :
        else if (letter == Keywords<CharT>::keyValuesSeparator)
        {
            _token.type = TokenType::key;
            callback(_token);
            reset();
        }
        else if (letter == Keywords<CharT>::propertiesSeparator)
        {
            _token.type = TokenType::value;
            callback(_token);
            reset();
        }
        // "//"
        else if (_token.data == Keywords<CharT>::beginComment)
        {
            _token.reset();
            _token.type = TokenType::comment;
            _position = TokenizerPosition::singeLineComment;
        }
        // Condition that will add to the buffer
        // Ignore space unless recording string items
        else if (letter == Keywords<CharT>::space)
        {
            if (_position == TokenizerPosition::singeLineComment
                || _position == TokenizerPosition::multiLineComment
                || _position == TokenizerPosition::string)
            {
                _token.append(letter);
            }
        }
        else if (
            letter != Keywords<CharT>::tab
            && letter != Keywords<CharT>::singleQuote
            && letter != Keywords<CharT>::doubleQuote)
        {
            _token.append(letter);
        }
        // Strings
        else if (
            letter == Keywords<CharT>::singleQuote
            || letter == Keywords<CharT>::doubleQuote)
        {
            if (_position == TokenizerPosition::string)
            {
                _position = TokenizerPosition::undefined;
            }
            else
            {
                _position = TokenizerPosition::string;
            }
        }

        ++begin;
    }
}
} // namespace json