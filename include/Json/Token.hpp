//
//  Token.hpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

namespace json::token
{
enum class TokenType
{
    beginObject,
    endObject,
    beginArray,
    endArray,
    key,
    value,
    comment,
    undefined,
};

template<typename CharT>
struct Token
{
    Token();
    Token(
        TokenType type,
        std::basic_string<CharT> data = std::basic_string<CharT>{});

    Token(const Token<CharT> &other);
    Token(Token<CharT> &&other);

    void append(CharT letter);
    void reset();

    bool operator==(const Token<CharT> &other) const;
    bool operator!=(const Token<CharT> &other) const;

    TokenType type;
    std::basic_string<CharT> data;
};

template<typename CharT>
std::basic_ostream<CharT> &
operator<<(std::basic_ostream<CharT> &out, const Token<CharT> &token);

} // namespace json

// Implementation

namespace json::token
{
template<typename CharT>
std::basic_ostream<CharT> &
operator<<(std::basic_ostream<CharT> &out, const Token<CharT> &token)
{
    using std::string_view;

    const auto print = [&](string_view text) {
        for (const auto letter : text)
        {
            out << out.widen(letter);
        }
    };

    switch (token.type)
    {
    case TokenType::key:
        print("key=");
        out << token.data;
        break;
    case TokenType::value:
        print("value=");
        out << token.data;
        break;
    case TokenType::beginObject:
        print("beginObject");
        break;
    case TokenType::endObject:
        print("endObject");
        break;
    case TokenType::beginArray:
        print("beginArray");
        break;
    case TokenType::endArray:
        print("endArray");
        break;
    case TokenType::comment:
        print("comment");
        break;
    case TokenType::undefined:
        print("?");
        break;
    }

    return out;
}

template<typename CharT>
Token<CharT>::Token()
{
}

template<typename CharT>
Token<CharT>::Token(TokenType type, std::basic_string<CharT> data)
    : type(type)
    , data(data)
{
}

template<typename CharT>
Token<CharT>::Token(const Token<CharT> &other)
    : type(other.type)
    , data(other.data)
{
}

template<typename CharT>
Token<CharT>::Token(Token<CharT> &&other)
    : type(other.type)
    , data(std::move(other.data))
{
}

template<typename CharT>
void Token<CharT>::append(CharT letter)
{
    data.push_back(letter);
}

template<typename CharT>
void Token<CharT>::reset()
{
    data.clear();
    type = TokenType::undefined;
}

template<typename CharT>
bool Token<CharT>::operator==(const Token<CharT> &other) const
{
    if (other.type != type)
    {
        return false;
    }

    if (other.data != data)
    {
        return false;
    }

    return true;
}

template<typename CharT>
bool Token<CharT>::operator!=(const Token<CharT> &other) const
{
    if (other.type != type)
    {
        return true;
    }

    if (other.data != data)
    {
        return true;
    }

    return false;
}
} // namespace json
