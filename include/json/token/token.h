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

namespace json::token {
template <typename CharT>
struct Token {
  enum class Type {
    kBeginObject,
    kEndObject,
    kBeginArray,
    kEndArray,
    kString,
    kNumber,
    kBoolean,
    kNull,
    kComment,
    kValueSeparator,
    kKeyValueSeparator,
    kUninitialized,
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

  template <typename DataT>
  Token(const Type &type, DataT &&data);

  /**
   * Create a string token
   * @str the string to create the token with
   */
  Token(const std::basic_string<CharT> &str);

  /**
   * Create a number token
   * @num the number to create the token with
   */
  Token(const NumberData &num);

  /**
   * Get a reference to the string data if the type is string or comment
   * @returns a reference to the string data
   */
  const StringData &string() const;

  /**
   * Get a reference to the number data;
   * @returns a reference to the number data
   */
  const NumberData &number() const;

  /**
   * Get a reference to the boolean data;
   * @returns a reference to the boolean data
   */
  const BooleanData &boolean() const;

  /**
   * Become a string token
   * @param buffer the buffer to give to the token
   */
  void FormString(StringData &&buffer);

  /**
   * Become a comment token
   * @param buffer the buffer to give to the token
   */
  void FormComment(StringData &&buffer);

  /**
   * Become a number token
   * @param number the number to give to the token
   */
  void FormNumber(NumberData number);

  /**
   * Become a bolean
   * @param boolean the value to give to the token
   */
  void FormBoolean(BooleanData boolean);

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
template <typename CharT>
std::basic_ostream<CharT> &operator<<(std::basic_ostream<CharT> &out,
                                      const Token<CharT> &token);

}  // namespace json::token

// Implementation

namespace json::token {
/**
 * Print token to out stream
 * @param out the out stream
 * @param token the token to print
 */
inline std::ostream &operator<<(std::ostream &out, const Token<char> &token) {
  using std::get;
  using TType = Token<char>::Type;

  switch (token.type) {
    case TType::kString:
      out << "string=";
      out << get<0>(token.data);
      break;
    case TType::kNumber:
      out << "number";
      out << get<1>(token.data);
      break;
    case TType::kBoolean:
      out << "boolean";
      out << std::boolalpha << "=" << token.boolean();
      break;
    case TType::kNull:
      out << "null";
      break;
    case TType::kBeginObject:
      out << "beginObject";
      break;
    case TType::kEndObject:
      out << "endObject";
      break;
    case TType::kBeginArray:
      out << "beginArray";
      break;
    case TType::kEndArray:
      out << "endArray";
      break;
    case TType::kComment:
      out << "comment";
      out << get<0>(token.data);
      break;
    case TType::kUninitialized:
      out << "?";
      break;
    case TType::kValueSeparator:
      out << "','";
      break;
    case TType::kKeyValueSeparator:
      out << "':'";
      break;
  }

  return out;
}

/**
 * Create a token with type initialized to undefined.
 */
template <typename CharT>
Token<CharT>::Token() : type(Type::kUninitialized) {}

/**
 * Create a copy of another token
 * @param other another token
 */
template <typename CharT>
Token<CharT>::Token(const Token<CharT> &other)
    : type(other.type), data(other.data) {}

/**
 * Transfer the resources of anther token
 * @param other another token
 */
template <typename CharT>
Token<CharT>::Token(Token<CharT> &&other) : type(other.type) {
  switch (type) {
    case Type::kString:
    case Type::kComment:
      data.template emplace<0>(std::move(std::get<0>(other.data)));
      break;
    case Type::kNumber:
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
template <typename CharT>
Token<CharT>::Token(const Type &type) : type(type), data() {
  switch (type) {
    case Type::kString:
      data.template emplace<0>();
      break;
    case Type::kNumber:
      data.template emplace<1>(0.0f);
      break;
    default:
      break;
  }
}

template <typename CharT>
template <typename DataT>
Token<CharT>::Token(const Type &type, DataT &&data)
    : type(type), data(std::forward<DataT>(data)) {}

/**
 * Create a string token
 * @str the string to create the token with
 */
template <typename CharT>
Token<CharT>::Token(const std::basic_string<CharT> &str)
    : type(Type::kString), data(str) {}

/**
 * Create a number token
 * @num the number to create the token with
 */
template <typename CharT>
Token<CharT>::Token(const NumberData &num) : type(Type::kNumber), data(num) {}

/**
 * Become a string token
 * @param buffer the buffer to give to the token
 */
template <typename CharT>
void Token<CharT>::FormString(StringData &&buffer) {
  type = Type::string;
  data.template emplace<0>(std::move(buffer));
}

/**
 * Become a comment token
 * @param buffer the buffer to give to the token
 */
template <typename CharT>
void Token<CharT>::FormComment(StringData &&buffer) {
  type = Type::comment;
  data.template emplace<0>(std::move(buffer));
}

/**
 * Become a number token
 * @param number the buffer to give to the token
 */
template <typename CharT>
void Token<CharT>::FormNumber(NumberData number) {
  type = Type::kNumber;
  data.template emplace<1>(number);
}

/**
 * Become a bolean
 * @param boolean the value to give to the token
 */
template <typename CharT>
void Token<CharT>::FormBoolean(BooleanData boolean) {
  type = Type::kBoolean;
  data.template emplace<2>(boolean);
}

/**
 * Get a reference to the string data if the type is string or comment
 * @returns a reference to the string data
 */
template <typename CharT>
const typename Token<CharT>::StringData &Token<CharT>::string() const {
  return std::get<0>(data);
}

/**
 * Get a reference to the number data;
 * @returns a reference to the number data
 */
template <typename CharT>
const typename Token<CharT>::NumberData &Token<CharT>::number() const {
  return std::get<1>(data);
}

/**
 * Get a reference to the number data;
 * @returns a reference to the number data
 */
template <typename CharT>
const typename Token<CharT>::BooleanData &Token<CharT>::boolean() const {
  return std::get<2>(data);
}

/**
 * See if two tokens are equal
 * @param other another token
 */
template <typename CharT>
bool Token<CharT>::operator==(const Token<CharT> &other) const {
  switch (type) {
    case Type::kString:
    case Type::kBoolean:
    case Type::kNumber:
      return (other.type == type) && (other.data == data);
    default:
      return type == other.type;
  }
}

/**
 * See if two tokens are not equal
 * @param other another token
 */
template <typename CharT>
bool Token<CharT>::operator!=(const Token<CharT> &other) const {
  return (other.type != type) || (other.data != data);
}
}  // namespace json::token
