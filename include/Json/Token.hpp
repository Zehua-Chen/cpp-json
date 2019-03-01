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
#include <variant>

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
        string,
        number,
        booleanTrue,
        booleanFalse,
        null,
        comment,
        undefined,
    };
    
    using StringData = std::basic_string<CharT>;
    using NumberData = float;
    
    using Data = std::variant<StringData, NumberData>;

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
     * Create a token of a specific type
     * @type type to create the token with.
     */
    Token(const Type &type);
    
    /**
     * Create a string token
     * @str the string to create the token with
     */
    Token(const std::basic_string<CharT> &str);
    
    /**
     * Create a number token
     * @num the number to create the token with
     */
    Token(const float &num);
    
    /**
     * Get a reference to the string data if the type is string or comment
     * @returns a reference to the string data
     */
    StringData &string();
    
    /**
     * Get a reference to the number data;
     * @returns a reference to the number data
     */
    NumberData &number();
    
    void formString();
    void formComment();
    void formNumber();

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

    /**
     * Type of the token
     */
    Type type;
    
    /**
     * Data of the token
     * Undefined unless type is string, comment or number
     */
    Data data;
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
    using std::get;
    using std::string_view;

    const auto print = [&](string_view text) {
        for (const auto letter : text)
        {
            out << out.widen(letter);
        }
    };

    switch (token.type)
    {
    case Token<CharT>::Type::string:
        print("string=");
        out << get<0>(token.data);
        break;
    case Token<CharT>::Type::number:
        print("number=");
        out << get<1>(token.data);
        break;
    case Token<CharT>::Type::booleanTrue:
        print("number=true");
        break;
    case Token<CharT>::Type::booleanFalse:
        print("number=false");
        break;
    case Token<CharT>::Type::null:
        print("null");
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
        out << get<0>(token.data);
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
{
    switch (type)
    {
    case Type::string:
    case Type::comment:
        data.template emplace<0>(std::move(std::get<0>(other.data)));
        break;
    case Type::number:
        data = other.data;
        break;
    default:
        break;
    }
}

/**
 * Create a token of a specific type
 * @type type to create the token with.
 */
template<typename CharT>
Token<CharT>::Token(const Type &type)
    : type(type)
    , data()
{
    switch (type)
    {
    case Type::string:
        data.template emplace<0>();
        break;
    case Type::number:
        data.template emplace<1>(0.0f);
        break;
    default:
        break;
    }
}

/**
 * Create a string token
 * @str the string to create the token with
 */
template<typename CharT>
Token<CharT>::Token(const std::basic_string<CharT> &str)
    : type(Type::string)
    , data(str)
{
}

/**
 * Create a number token
 * @num the number to create the token with
 */
template<typename CharT>
Token<CharT>::Token(const float &num)
    : type(Type::number)
    , data(num)
{
    
}

/**
 * Get a reference to the string data if the type is string or comment
 * @returns a reference to the string data
 */
template<typename CharT>
typename Token<CharT>::StringData &Token<CharT>::string()
{
    return std::get<0>(data);
}

/**
 * Get a reference to the number data;
 * @returns a reference to the number data
 */
template<typename CharT>
typename Token<CharT>::NumberData &Token<CharT>::number()
{
    return std::get<1>(data);
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
