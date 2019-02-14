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
template<typename CharT>
struct Token
{
    enum class Type
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

    /**
     * Create a token with type initialized to undefined.
     */
    Token();

    /**
     * Create a copy of another token
     * @param other another token
     */
    Token(const Token<CharT> &other);

    /**
     * Transfer the resources of anther token
     * @param other another token
     */
    Token(Token<CharT> &&other);

    /**
     * Transfer the resources of anther token
     * @param type type of the token
     * @param data data of the token
     */
    Token(
        Type type, std::basic_string<CharT> data = std::basic_string<CharT>{});

    /**
     * See if two tokens are equal
     * @param other another token
     */
    bool operator==(const Token<CharT> &other) const;

    /**
     * See if two tokens are not equal
     * @param other another token
     */
    bool operator!=(const Token<CharT> &other) const;

    Type type;
    std::basic_string<CharT> data;
};

/**
 * Print token to out stream
 * @param out the out stream
 * @param token the token to print
 */
template<typename CharT>
std::basic_ostream<CharT> &
operator<<(std::basic_ostream<CharT> &out, const Token<CharT> &token);

} // namespace json::token

// Implementation

namespace json::token
{
/**
 * Print token to out stream
 * @param out the out stream
 * @param token the token to print
 */
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
    case Token<CharT>::Type::key:
        print("key=");
        out << token.data;
        break;
    case Token<CharT>::Type::value:
        print("value=");
        out << token.data;
        break;
    case Token<CharT>::Type::beginObject:
        print("beginObject");
        break;
    case Token<CharT>::Type::endObject:
        print("endObject");
        break;
    case Token<CharT>::Type::beginArray:
        print("beginArray");
        break;
    case Token<CharT>::Type::endArray:
        print("endArray");
        break;
    case Token<CharT>::Type::comment:
        print("comment=");
        out << token.data;
        break;
    case Token<CharT>::Type::undefined:
        print("?");
        break;
    }

    return out;
}

/**
 * Create a token with type initialized to undefined.
 */
template<typename CharT>
Token<CharT>::Token()
    : type(Type::undefined)
{
}

/**
 * Create a copy of another token
 * @param other another token
 */
template<typename CharT>
Token<CharT>::Token(const Token<CharT> &other)
    : type(other.type)
    , data(other.data)
{
}

/**
 * Transfer the resources of anther token
 * @param other another token
 */
template<typename CharT>
Token<CharT>::Token(Token<CharT> &&other)
    : type(other.type)
    , data(std::move(other.data))
{
}

/**
 * Transfer the resources of anther token
 * @param type type of the token
 * @param data data of the token
 */
template<typename CharT>
Token<CharT>::Token(Type type, std::basic_string<CharT> data)
    : type(type)
    , data(data)
{
}

/**
 * See if two tokens are equal
 * @param other another token
 */
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

/**
 * See if two tokens are not equal
 * @param other another token
 */
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
} // namespace json::token
