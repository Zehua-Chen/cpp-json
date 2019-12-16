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
template <typename CharT>
class BasicValue {
 public:
  /**
   * Type of the value
   */
  enum class Type : char {
    kNull,
    kNumber,
    kBoolean,
    kString,
    kObject,
    kArray,
  };

  /// Aliases

  using Key = BasicKey<CharT>;

  using Null = std::nullptr_t;
  using Number = double;
  using Boolean = bool;
  using String = std::basic_string<CharT>;
  using Object = std::unordered_map<Key, BasicValue<CharT>>;
  using Array = std::vector<BasicValue<CharT>>;

  using Data = std::variant<Null, Number, Boolean, String, Object, Array>;

  /// Constructors, destructors

  BasicValue();

  /**
   * Construct json value of the specified type
   * @param type type of the json value
   */
  BasicValue(Type type);

  BasicValue(const CharT *str);
  explicit BasicValue(const String &str);
  BasicValue(String &&str);

  BasicValue(const double &number);
  BasicValue(const bool &boolean);

  /**
   * Copy constructor
   * @other the other value to copy data from
   */
  BasicValue(const BasicValue<CharT> &other);

  /**
   * Move constructor
   * @other the other value to transfer data from
   */
  BasicValue(BasicValue<CharT> &&other);

  virtual ~BasicValue();

  /// Type accessors

  /**
   * Get the type of object;
   * @returns the type of the object
   */
  Type type() const;

  /**
   * Determines if the value is an object.
   * @returns true if value is object
   */
  bool IsObject() const;

  /**
   * Determines if the value is an array.
   * @returns true if value is array
   */
  bool IsArray() const;

  /**
   * Determines if the value is a primitive.
   * @returns true if value is primitive
   */
  bool IsPrimitive() const;

  /// Type modifiers

  /// Universal Accessor

  /**
   * Get the size of the value
   * @returns type = object: size of unordered map; type = array: size of
   * vector; type = primitive: size of string.
   */
  size_t size() const;

  /// Universal Operators

  /**
   * Copy value from other value
   * @other the value to copy from
   * @returns a reference to this object
   */
  BasicValue<CharT> &operator=(const BasicValue<CharT> &other);

  /**
   * Move value from other value
   * @other the value to move from
   * @returns a reference to this object
   */
  BasicValue<CharT> &operator=(BasicValue<CharT> &&other);

  /// Object modifiers

  /**
   * Set the a value using the key
   * @param value the value
   * @param key the key used to set the value
   * @discussions if a value associated with the key already exits,
   * override the existing value.
   */
  void Set(const Key &key, BasicValue<CharT> &value);

  /**
   * Erase the value with the key
   * @param key the key associated with the value
   */
  void Erase(const Key &key);

  /// Object accessors

  /**
   * Have a reference to the value associated with the key.
   * @param key the key associated with the value
   * @returns a reference to the value
   */
  BasicValue<CharT> &operator[](const Key &key);

  /**
   * Have a reference to the value associated with the key.
   * @param key the key associated with the value
   * @returns a reference to the value
   */
  BasicValue<CharT> &Get(const Key &key);

  /**
   * Have a constant reference to the value associated with the key.
   * @param key the key associated with the value
   * @returns a reference to the value
   */
  const BasicValue<CharT> &operator[](const Key &key) const;

  /**
   * Have a constant reference to the value associated with the key.
   * @param key the key associated with the value
   * @returns a reference to the value
   */
  const BasicValue<CharT> &Get(const Key &key) const;

  /**
   * See if the object contains the key
   * @key the key to lookup
   * @returns true if there is an eleemnt associated with the key in
   * the object.
   */
  bool Contains(const Key &key) const;

  /// Array modifiers

  /**
   * Add new element to the array.
   * @value the value to add
   */
  void Append(const BasicValue<CharT> &value);

  /**
   * Add new element to the array.
   * @value the value to add
   */
  void Append(BasicValue<CharT> &&value);

  /**
   * Erase element in the array, will push elements to remain ordering.
   * @index the index to erase at.
   */
  void Erase(const size_t index);

  /// Array accessors

  /**
   * Retreive a constant reference to an element at the index
   * @index the index of the element
   * @returns a constant reference to the json object
   */
  const BasicValue<CharT> &operator[](size_t index) const;

  /**
   * Retreive a reference to an element at the index
   * @index the index of the element
   * @returns a reference to the json object
   */
  BasicValue<CharT> &operator[](size_t index);

  /// String Modifiers

  /**
   * Set the value of the primitive to a string value.
   * @value the value to set
   */
  void set_string(const CharT *str);

  /**
   * Set the value of the primitive to a string value.
   * @value the value to set
   */
  void set_string(const std::basic_string<CharT> &str);

  /**
   * Set the value of the primitive to a string value.
   * @value the value to move from
   */
  void set_string(std::basic_string<CharT> &&text);

  /// Primitive accessors

  /**
   * Retrieve a constant reference to the string represented by the primitive
   * @returns a constant reference to the string.
   */
  const String &string() const;

  /**
   * Retrieve a reference to the string represented by the primitive
   * @returns a reference to the string.
   */
  String &string();

  void set_number(const Number &number);
  Number &number();
  const Number &number() const;

  void set_boolean(const Boolean &boolean);
  Boolean &boolean();
  const Boolean &boolean() const;

 private:
  Data data_;
};

/// Factory Methods

/**
 * Make an empty object.
 * @returns an empty object.
 */
template <typename CharT = char>
BasicValue<CharT> MakeObject();

/**
 * Make an empty array.
 * @returns an empty array.
 */
template <typename CharT = char>
BasicValue<CharT> MakeArray();

}  // namespace json

// Implementation

namespace json {
/// Constructor and Destructor

template <typename CharT>
BasicValue<CharT>::BasicValue() {}

/**
 * Construct json value of the specifiec type
 * @param type type of the json value
 */
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

/**
 * Copy constructor
 * @other the other value to copy data from
 */
template <typename CharT>
BasicValue<CharT>::BasicValue(const BasicValue<CharT> &other)
    : data_(other.data_) {}

/**
 * Move constructor
 * @other the other value to transfer data from
 */
template <typename CharT>
BasicValue<CharT>::BasicValue(BasicValue<CharT> &&other)
    : data_(std::move(other.data_)) {}

template <typename CharT>
BasicValue<CharT>::~BasicValue() {}

/// Type modifiers

/**
 * Get the type of object.
 * @returns the type of the object
 */
template <typename CharT>
typename BasicValue<CharT>::Type BasicValue<CharT>::type() const {
  return static_cast<Type>(data_.index());
}

/**
 * Determines if the value is an object.
 * @returns true if value is object
 */
template <typename CharT>
bool BasicValue<CharT>::IsObject() const {
  return type() == Type::kObject;
}

/**
 * Determines if the value is an array.
 * @returns true if value is array
 */
template <typename CharT>
bool BasicValue<CharT>::IsArray() const {
  return type() == Type::kArray;
}

/**
 * Determines if the value is a primitive.
 * @returns true if value is primitive
 */
template <typename CharT>
bool BasicValue<CharT>::IsPrimitive() const {
  return false;
}

// Universal accessors

/**
 * Get the size of the value
 * @returns type = object: size of unordered map; type = array: size of
 * vector; type = primitive: size of string.
 */
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

/// Universal Operators

/**
 * Copy value from other value
 * @other the value to copy from
 * @returns a reference to this object
 */
template <typename CharT>
BasicValue<CharT> &BasicValue<CharT>::operator=(
    const BasicValue<CharT> &other) {
  if (&other != this) {
    data_ = other.data_;
  }

  return *this;
}

/**
 * Move value from other value
 * @other the value to move from
 * @returns a reference to this object
 */
template <typename CharT>
BasicValue<CharT> &BasicValue<CharT>::operator=(BasicValue<CharT> &&other) {
  if (&other != this) {
    data_ = std::move(other.data_);
  }

  return *this;
}

/// Object modifiers

/**
 * Set the a value using the key
 * @param value the value
 * @param key the key used to set the value
 * @discussions if a value associated with the key already exits,
 * override the existing value.
 */
template <typename CharT>
void BasicValue<CharT>::Set(const Key &key, BasicValue<CharT> &value) {
  Object &data = std::get<Object>(data_);
  data.insert_or_assign(key, value);
}

/**
 * Erase the value with the key
 * @param key the key associated with the value
 */
template <typename CharT>
void BasicValue<CharT>::Erase(const Key &key) {
  Object &data = std::get<Object>(data_);
  data.erase(key);
}

/// Object accessors

/**
 * Have a reference to the value associated with the key.
 * @param key the key associated with the value
 * @returns a reference to the value
 */
template <typename CharT>
BasicValue<CharT> &BasicValue<CharT>::operator[](const Key &key) {
  Object &data = std::get<Object>(data_);
  return data[key];
}

/**
 * Have a reference to the value associated with the key.
 * @param key the key associated with the value
 * @returns a reference to the value
 */
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

/**
 * Have a constant reference to the value associated with the key.
 * @param key the key associated with the value
 * @returns a reference to the value
 */
template <typename CharT>
const BasicValue<CharT> &BasicValue<CharT>::Get(const Key &key) const {
  Object &data = std::get<Object>(data_);
  return data.find(key)->second;
}

/**
 * See if the object contains the key
 * @key the key to lookup
 * @returns true if there is an eleemnt associated with the key in
 * the object.
 */
template <typename CharT>
bool BasicValue<CharT>::Contains(const Key &key) const {
  const Object &data = std::get<Object>(data_);
  auto found = data.find(key);

  return found != data.end();
}

/// Array modifiers

/**
 * Add new element to the array.
 * @value the value to add
 */
template <typename CharT>
void BasicValue<CharT>::Append(const BasicValue<CharT> &value) {
  Array &data = std::get<Array>(data_);
  data.push_back(value);
}

/**
 * Add new element to the array.
 * @value the value to add
 */
template <typename CharT>
void BasicValue<CharT>::Append(BasicValue<CharT> &&value) {
  Array &data = std::get<Array>(data_);
  data.push_back(std::move(value));
}

/**
 * Erase element in the array, will push elements to remain ordering.
 * @index the index to erase at.
 */
template <typename CharT>
void BasicValue<CharT>::Erase(const size_t index) {
  Array &data = std::get<Array>(data_);
  auto position = data.begin();
  std::advance(position, index);

  data.erase(position);
}

/// Array accessors

/**
 * Retreive a constant reference to an element at the index
 * @index the index of the element
 * @returns a constant reference to the json object
 */
template <typename CharT>
const BasicValue<CharT> &BasicValue<CharT>::operator[](size_t index) const {
  Array &data = std::get<Array>(data_);
  return data[index];
}

/**
 * Retreive a reference to an element at the index
 * @index the index of the element
 * @returns a reference to the json object
 */
template <typename CharT>
BasicValue<CharT> &BasicValue<CharT>::operator[](size_t index) {
  Array &data = std::get<Array>(data_);
  return data[index];
}

/// Primitive modifiers

/**
 * Set the value of the primitive to a string value.
 * @value the value to set
 */
template <typename CharT>
void BasicValue<CharT>::set_string(const CharT *str) {
  data_.template emplace<String>(str);
}

/**
 * Set the value of the primitive to a string value.
 * @value the value to set
 */
template <typename CharT>
void BasicValue<CharT>::set_string(const String &str) {
  data_.template emplace<String>(str);
}

/**
 * Set the value of the primitive to a string value.
 * @value the value to move from
 */
template <typename CharT>
void BasicValue<CharT>::set_string(std::basic_string<CharT> &&str) {
  data_.template emplace<String>(std::move(str));
}

/// Primitive accessors

/**
 * Retrieve a constant reference to the string represented by the primitive
 * @returns a constant reference to the string.
 */
template <typename CharT>
const typename BasicValue<CharT>::String &BasicValue<CharT>::string() const {
  return std::get<String>(data_);
}

/**
 * Retrieve a reference to the string represented by the primitive
 * @returns a reference to the string.
 */
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

/// Factory Methods

/**
 * Make an empty object.
 * @returns an empty object.
 */
template <typename CharT>
BasicValue<CharT> MakeObject() {
  return {BasicValue<CharT>::Type::kObject};
}

/**
 * Make an empty array.
 * @returns an empty array.
 */
template <typename CharT>
BasicValue<CharT> MakeArray() {
  return {BasicValue<CharT>::Type::kArray};
}
}  // namespace json