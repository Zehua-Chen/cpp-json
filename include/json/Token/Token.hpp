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
        boolean,
        null,
        comment,
        uninitialized,
        endOfString,
    };

    using StringData = std::basic_string<CharT>;
    using NumberData = double;
    using BooleanData = bool;

    using Data = std::variant<StringData, NumberData, BooleanData>;

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

    /**
     * Get a reference to the boolean data;
     * @returns a reference to the boolean data
     */
    BooleanData &boolean();

    /**
     * Become a string token
     * @param buffer the buffer to give to the token
     */
    void formString(StringData &&buffer);

    /**
     * Become a comment token
     * @param buffer the buffer to give to the token
     */
    void formComment(StringData &&buffer);

    /**
     * Become a number token
     * @param number the number to give to the token
     */
    void formNumber(NumberData number);

    /**
     * Become a bolean
     * @param boolean the value to give to the token
     */
    void formBoolean(BooleanData boolean);
    
    /**
     * Determine if a token is the end of string
     * @returns if the token is end of stirng
     */
    bool isEndOfString();

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
inline std::ostream &operator<<(std::ostream &out, const Token<char> &token)
{
    using std::get;

    switch (token.type)
    {
    case Token<char>::Type::string:
        out << "string=";
        out << get<0>(token.data);
        break;
    case Token<char>::Type::number:
        out << "number";
        out << get<1>(token.data);
        break;
    case Token<char>::Type::boolean:
        out << "boolean";
        break;
    case Token<char>::Type::null:
        out << "null";
        break;
    case Token<char>::Type::beginObject:
        out << "beginObject";
        break;
    case Token<char>::Type::endObject:
        out << "endObject";
        break;
    case Token<char>::Type::beginArray:
        out << "beginArray";
        break;
    case Token<char>::Type::endArray:
        out << "endArray";
        break;
    case Token<char>::Type::comment:
        out << "comment";
        out << get<0>(token.data);
        break;
    case Token<char>::Type::uninitialized:
        out << "?";
        break;
    case Token<char>::Type::endOfString:
        out << "eos";
        break;
    }

    return out;
}

/**
 * Create a token with type initialized to undefined.
 */
template<typename CharT>
Token<CharT>::Token()
    : type(Type::uninitialized)
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
 * Become a string token
 * @param buffer the buffer to give to the token
 */
template<typename CharT>
void Token<CharT>::formString(StringData &&buffer)
{
    type = Type::string;
    data.template emplace<0>(std::move(buffer));
}

/**
 * Become a comment token
 * @param buffer the buffer to give to the token
 */
template<typename CharT>
void Token<CharT>::formComment(StringData &&buffer)
{
    type = Type::comment;
    data.template emplace<0>(std::move(buffer));
}

/**
 * Become a number token
 * @param number the buffer to give to the token
 */
template<typename CharT>
void Token<CharT>::formNumber(NumberData number)
{
    type = Type::number;
    data.template emplace<1>(number);
}

/**
 * Become a bolean
 * @param boolean the value to give to the token
 */
template<typename CharT>
void Token<CharT>::formBoolean(BooleanData boolean)
{
    type = Type::boolean;
    data.template emplace<2>(boolean);
}

template<typename CharT>
bool Token<CharT>::isEndOfString()
{
    return type == Type::endOfString;
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
 * Get a reference to the number data;
 * @returns a reference to the number data
 */
template<typename CharT>
typename Token<CharT>::BooleanData &Token<CharT>::boolean()
{
    return std::get<2>(data);
}

/**
 * See if two tokens are equal
 * @param other another token
 */
template<typename CharT>
bool Token<CharT>::operator==(const Token<CharT> &other) const
{
    return (other.type == type) && (other.data == data);
}

/**
 * See if two tokens are not equal
 * @param other another token
 */
template<typename CharT>
bool Token<CharT>::operator!=(const Token<CharT> &other) const
{
    return (other.type != type) || (other.data != data);
}
} // namespace json::token
