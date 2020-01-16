//
//  BasicValue.hpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#pragma once

#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
#include "json/value/basic_key.h"

namespace json {
/**
 * @brief A json value
 */
template <typename CharT>
class BasicValue {
 public:
  /**
   * @brief Type of the value
   */
  enum class Type : char {
    /**
     * @brief null
     */
    kNull,
    /**
     * @brief number
     */
    kNumber,
    /**
     * @brief boolean
     */
    kBoolean,
    /**
     * @brief string
     */
    kString,
    /**
     * @brief object
     */
    kObject,
    /**
     * @brief array
     */
    kArray,
  };

  /**
   * @brief Key used to index properties in a json object
   */
  using Key = BasicKey<CharT>;

  /**
   * @brief Type used to store a null
   */
  using Null = std::nullptr_t;

  /**
   * @brief Type used to store a number
   */
  using Number = double;

  /**
   * @brief Type used to store a boolean
   */
  using Boolean = bool;

  /**
   * @brief Type used to store a string
   */
  using String = std::basic_string<CharT>;

  /**
   * @brief Type used to store an object
   */
  using Object = std::unordered_map<Key, BasicValue<CharT>>;

  /**
   * @brief Type used to store an array
   */
  using Array = std::vector<BasicValue<CharT>>;

  /**
   * @brief Type used to store the data of the json value
   */
  using Data = std::variant<Null, Number, Boolean, String, Object, Array>;

  /**
   * @brief Create a null value
   */
  BasicValue();

  /**
   * @brief Construct json value of the specified type
   * @param type type of the json value
   */
  BasicValue(Type type);

  /**
   * @brief Construct a string value
   * @param str a string pointer
   */
  BasicValue(const CharT *str);

  /**
   * @brief Construct a string value
   * @param str a reference to a string object
   */
  explicit BasicValue(const String &str);

  /**
   * @brief Construct a string value
   * @param str a r-reference to a string object
   */
  BasicValue(String &&str);

  /**
   * @brief Construct a number value
   * @param number the number the json value would hold
   */
  BasicValue(const double &number);

  /**
   * @brief Construct a boolean value
   * @param boolean the number the json value would hold
   */
  BasicValue(const bool &boolean);

  /**
   * Copy constructor
   * @param other the other value to copy data from
   */
  BasicValue(const BasicValue<CharT> &other);

  /**
   * Move constructor
   * @other the other value to transfer data from
   */
  BasicValue(BasicValue<CharT> &&other);

  virtual ~BasicValue();

  /**
   * @brief Get the type of object;
   * @returns the type of the object
   */
  Type type() const;

  /**
   * @brief Determines if the value is an object.
   * @returns true if value is object
   */
  bool IsObject() const;

  /**
   * @brief Determines if the value is an array.
   * @returns true if value is array
   */
  bool IsArray() const;

  /**
   * @brief Determines if the value is a primitive.
   * @returns true if value is primitive
   */
  bool IsPrimitive() const;

  /**
   * @brief Get the size of the value
   * @returns type = object: size of unordered map; type = array: size of
   * vector; type = primitive: size of string.
   */
  size_t size() const;

  /**
   * @brief Copy value from other value
   * @param other the value to copy from
   * @returns a reference to this object
   */
  BasicValue<CharT> &operator=(const BasicValue<CharT> &other);

  /**
   * Move value from other value
   * @param other the value to move from
   * @returns a reference to this object
   */
  BasicValue<CharT> &operator=(BasicValue<CharT> &&other);

  /// Object modifiers

  /**
   * @brief Set the a value using the key
   *
   * if a value associated with the key already exits,
   * override the existing value.
   *
   * @param value the value
   * @param key the key used to set the value
   */
  void Set(const Key &key, BasicValue<CharT> &value);

  /**
   * @brief Erase the value with the key
   * @param key the key associated with the value
   */
  void Erase(const Key &key);

  /**
   * @brief Have a reference to the value associated with the key.
   * @param key the key associated with the value
   * @returns a reference to the value
   */
  BasicValue<CharT> &operator[](const Key &key);

  /**
   * @brief Have a reference to the value associated with the key.
   * @param key the key associated with the value
   * @returns a reference to the value
   */
  BasicValue<CharT> &Get(const Key &key);

  /**
   * @brief Have a constant reference to the value associated with the key.
   * @param key the key associated with the value
   * @returns a reference to the value
   */
  const BasicValue<CharT> &operator[](const Key &key) const;

  /**
   * @brief Have a constant reference to the value associated with the key.
   * @param key the key associated with the value
   * @returns a reference to the value
   */
  const BasicValue<CharT> &Get(const Key &key) const;

  /**
   * @brief See if the object contains the key
   * @param key the key to lookup
   * @returns true if there is an element associated with the key in
   * the object.
   */
  bool Contains(const Key &key) const;

  /**
   * @brief Add new element to the array.
   * @param value the value to add
   */
  void Append(const BasicValue<CharT> &value);

  /**
   * @brief Add new element to the array.
   * @param value the value to add
   */
  void Append(BasicValue<CharT> &&value);

  /**
   * @brief Erase element in the array, will push elements to remain ordering.
   * @param index the index to erase at.
   */
  void Erase(const size_t index);

  /**
   * @brief Retreive a constant reference to an element at the index
   * @param index the index of the element
   * @returns a constant reference to the json object
   */
  const BasicValue<CharT> &operator[](size_t index) const;

  /**
   * @brief Retreive a reference to an element at the index
   * @param index the index of the element
   * @returns a reference to the json object
   */
  BasicValue<CharT> &operator[](size_t index);

  /**
   * @brief Set the value of the primitive to a string value.
   * @param str the string to use
   */
  void set_string(const CharT *str);

  /**
   * @brief Set the value of the primitive to a string value.
   * @param str the value to set
   */
  void set_string(const std::basic_string<CharT> &str);

  /**
   * @brief Set the value of the primitive to a string value.
   * @param text the value to move from
   */
  void set_string(std::basic_string<CharT> &&text);

  /**
   * @brief Retrieve a constant reference to the string represented by the
   * primitive
   * @returns a constant reference to the string.
   */
  const String &string() const;

  /**
   * @brief Retrieve a reference to the string represented by the primitive
   * @returns a reference to the string.
   */
  String &string();

  /**
   * @brief Set the value of the primitive to a number value
   * @param number the value to use
   */
  void set_number(const Number &number);

  /**
   * @brief Retrieve a reference to the number represented by the
   * primitive
   * @returns a constant reference to the number.
   */
  Number &number();

  /**
   * @brief Retrieve a constant reference to the number represented by the
   * primitive
   * @returns a constant reference to the number.
   */
  const Number &number() const;

  /**
   * @brief Set the value of the primitive to a boolean
   * @param boolean the value to use
   */
  void set_boolean(const Boolean &boolean);

  /**
   * @brief Retrieve a reference to the boolean represented by the
   * primitive
   * @returns a constant reference to the boolean.
   */
  Boolean &boolean();

  /**
   * @brief Retrieve a constant reference to the boolean represented by the
   * primitive
   * @returns a constant reference to the boolean.
   */
  const Boolean &boolean() const;

 private:
  Data data_;
};

/**
 * @brief Make an empty object.
 * @returns an empty object.
 */
template <typename CharT = char>
BasicValue<CharT> MakeObject();

/**
 * @brief Make an empty array.
 * @returns an empty array.
 */
template <typename CharT = char>
BasicValue<CharT> MakeArray();

}  // namespace json

// Implementations

namespace json {
template <typename CharT>
BasicValue<CharT>::BasicValue() {}

template <typename CharT>
BasicValue<CharT>::BasicValue(Type type) {
  using std::forward;

  switch (type) {
    case Type::kNull:
      data_.template emplace<Null>();
      break;
    case Type::kNumber:
      data_.template emplace<Number>();
      break;
    case Type::kBoolean:
      data_.template emplace<Boolean>();
      break;
    case Type::kString:
      data_.template emplace<String>();
      break;
    case Type::kObject:
      data_.template emplace<Object>();
      break;
    case Type::kArray:
      data_.template emplace<Array>();
      break;
    default:
      break;
  }
}

template <typename CharT>
BasicValue<CharT>::BasicValue(const CharT *str) {
  data_.template emplace<String>(str);
}

template <typename CharT>
BasicValue<CharT>::BasicValue(const String &str) {
  data_.template emplace<String>(str);
}

template <typename CharT>
BasicValue<CharT>::BasicValue(String &&str) {
  data_.template emplace<String>(std::move(str));
}

template <typename CharT>
BasicValue<CharT>::BasicValue(const double &number) {
  data_.template emplace<Number>(number);
}

template <typename CharT>
BasicValue<CharT>::BasicValue(const bool &boolean) {
  data_.template emplace<Boolean>(boolean);
}

template <typename CharT>
BasicValue<CharT>::BasicValue(const BasicValue<CharT> &other)
    : data_(other.data_) {}

template <typename CharT>
BasicValue<CharT>::BasicValue(BasicValue<CharT> &&other)
    : data_(std::move(other.data_)) {}

template <typename CharT>
BasicValue<CharT>::~BasicValue() {}

template <typename CharT>
typename BasicValue<CharT>::Type BasicValue<CharT>::type() const {
  return static_cast<Type>(data_.index());
}

template <typename CharT>
bool BasicValue<CharT>::IsObject() const {
  return type() == Type::kObject;
}

template <typename CharT>
bool BasicValue<CharT>::IsArray() const {
  return type() == Type::kArray;
}

template <typename CharT>
bool BasicValue<CharT>::IsPrimitive() const {
  return false;
}

template <typename CharT>
size_t BasicValue<CharT>::size() const {
  using std::get;

  switch (data_.index()) {
    case 3:
      return get<String>(data_).size();
    case 4:
      return get<Object>(data_).size();
    case 5:
      return get<Array>(data_).size();
    default:
      return 0;
  }
}

template <typename CharT>
BasicValue<CharT> &BasicValue<CharT>::operator=(
    const BasicValue<CharT> &other) {
  if (&other != this) {
    data_ = other.data_;
  }

  return *this;
}

template <typename CharT>
BasicValue<CharT> &BasicValue<CharT>::operator=(BasicValue<CharT> &&other) {
  if (&other != this) {
    data_ = std::move(other.data_);
  }

  return *this;
}

template <typename CharT>
void BasicValue<CharT>::Set(const Key &key, BasicValue<CharT> &value) {
  Object &data = std::get<Object>(data_);
  data.insert_or_assign(key, value);
}

template <typename CharT>
void BasicValue<CharT>::Erase(const Key &key) {
  Object &data = std::get<Object>(data_);
  data.erase(key);
}

template <typename CharT>
BasicValue<CharT> &BasicValue<CharT>::operator[](const Key &key) {
  Object &data = std::get<Object>(data_);
  return data[key];
}

template <typename CharT>
BasicValue<CharT> &BasicValue<CharT>::Get(const Key &key) {
  Object &data = std::get<Object>(data_);
  return data.find(key)->second;
}

template <typename CharT>
const BasicValue<CharT> &BasicValue<CharT>::operator[](const Key &key) const {
  const Object &data = std::get<Object>(data_);
  return data.find(key)->second;
}

template <typename CharT>
const BasicValue<CharT> &BasicValue<CharT>::Get(const Key &key) const {
  Object &data = std::get<Object>(data_);
  return data.find(key)->second;
}

template <typename CharT>
bool BasicValue<CharT>::Contains(const Key &key) const {
  const Object &data = std::get<Object>(data_);
  auto found = data.find(key);

  return found != data.end();
}

template <typename CharT>
void BasicValue<CharT>::Append(const BasicValue<CharT> &value) {
  Array &data = std::get<Array>(data_);
  data.push_back(value);
}

template <typename CharT>
void BasicValue<CharT>::Append(BasicValue<CharT> &&value) {
  Array &data = std::get<Array>(data_);
  data.push_back(std::move(value));
}

template <typename CharT>
void BasicValue<CharT>::Erase(const size_t index) {
  Array &data = std::get<Array>(data_);
  auto position = data.begin();
  std::advance(position, index);

  data.erase(position);
}

template <typename CharT>
const BasicValue<CharT> &BasicValue<CharT>::operator[](size_t index) const {
  Array &data = std::get<Array>(data_);
  return data[index];
}

template <typename CharT>
BasicValue<CharT> &BasicValue<CharT>::operator[](size_t index) {
  Array &data = std::get<Array>(data_);
  return data[index];
}

template <typename CharT>
void BasicValue<CharT>::set_string(const CharT *str) {
  data_.template emplace<String>(str);
}

template <typename CharT>
void BasicValue<CharT>::set_string(const String &str) {
  data_.template emplace<String>(str);
}

template <typename CharT>
void BasicValue<CharT>::set_string(std::basic_string<CharT> &&str) {
  data_.template emplace<String>(std::move(str));
}

template <typename CharT>
const typename BasicValue<CharT>::String &BasicValue<CharT>::string() const {
  return std::get<String>(data_);
}

template <typename CharT>
typename BasicValue<CharT>::String &BasicValue<CharT>::string() {
  return std::get<String>(data_);
}

template <typename CharT>
void BasicValue<CharT>::set_number(const Number &number) {
  data_.template emplace<Number>(number);
}

template <typename CharT>
typename BasicValue<CharT>::Number &BasicValue<CharT>::number() {
  return std::get<Number>(data_);
}

template <typename CharT>
const typename BasicValue<CharT>::Number &BasicValue<CharT>::number() const {
  return std::get<Number>(data_);
}

template <typename CharT>
void BasicValue<CharT>::set_boolean(const Boolean &boolean) {
  data_.template emplace<Boolean>(boolean);
}

template <typename CharT>
typename BasicValue<CharT>::Boolean &BasicValue<CharT>::boolean() {
  return std::get<Boolean>(data_);
}

template <typename CharT>
const typename BasicValue<CharT>::Boolean &BasicValue<CharT>::boolean() const {
  return std::get<Boolean>(data_);
}

template <typename CharT>
BasicValue<CharT> MakeObject() {
  return {BasicValue<CharT>::Type::kObject};
}

template <typename CharT>
BasicValue<CharT> MakeArray() {
  return {BasicValue<CharT>::Type::kArray};
}
}  // namespace json