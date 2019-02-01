//
//  BasicValue.hpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace json
{
template<typename CharT>
class BasicValue
{
public:
    /**
     * Type of the value
     */
    enum class Type
    {
        // Value is object
        object,
        // Value is array
        array,
        // Value is primitive
        primitive,
    };

    /// Aliases

    using Key = std::basic_string<CharT>;
    using ObjectData = std::unordered_map<Key, BasicValue<CharT>>;
    using ArrayData = std::vector<BasicValue<CharT>>;
    using PrimitiveData = std::basic_string<CharT>;

    /// Constructors, destructors

    /**
     * Construct json value of the specifiec type
     * @param type type of the json value
     */
    BasicValue(Type type = Type::object);
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
    bool isObject() const;

    /**
     * Determines if the value is an array.
     * @returns true if value is array
     */
    bool isArray() const;

    /**
     * Determines if the value is a primitive.
     * @returns true if value is primitive
     */
    bool isPrimitive() const;

    /// Type modifiers

    // TODO: Implement
    /**
     * Set the type of object;
     * @param type new type of the value
     */
    void type(Type type);

    /// Universal Accessor

    /**
     * Get the size of the value
     * @returns type = object: size of unordered map; type = array: size of
     * vector; type = primitive: size of string.
     */
    size_t size() const;

    /// Object modifiers

    /**
     * Set the a value using the key
     * @param value the value
     * @param key the key used to set the value
     * @discussions if a value associated with the key already exits,
     * override the existing value.
     */
    void set(const Key &key, BasicValue<CharT> &value);

    // TODO: Implement
    /**
     * Erase the value with the key
     * @param key the key associated with the value
     */
    void erase(const Key &key);

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
    BasicValue<CharT> &get(const Key &key);

    // TODO: Implement
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
    const BasicValue<CharT> &get(const Key &key) const;
    
    /**
     * See if the object contains the key
     * @key the key to lookup
     */
    bool contains(const Key &key) const;

    /// Array modifiers

    /**
     * Add new element to the array.
     * @value the value to add
     */
    void append(const BasicValue &value);

    // TODO: Implement
    /**
     * Erase element in the array, will push elements to remain ordering.
     * @index the index to erase at.
     */
    void erase(const size_t index);

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

    /// Primitive Modifiers

    // TODO: Implement
    /**
     * Set the value of the primitive to a boolean value.
     * @value the value to set
     */
    void boolean(bool value);

    /**
     * Set the value of the primitive to a string value.
     * @value the value to set
     */
    void string(const std::basic_string<CharT> &text);

    // TODO: Implement
    /**
     * Set the value of the primitive to a number value.
     * @value the value to set
     */
    void number(float number);

    // TODO: Implement
    /**
     * Set the value of the primitive to null value.
     * @value the value to set
     */
    void nullify();

    /// Primitive accessors

    // TODO: Implement
    /**
     * Retrieve the boolean value represented by the primitive
     * @returns true or false
     */
    bool boolean() const;

    /**
     * Retrieve a constant reference to the string represented by the primitive
     * @returns a constant reference to the string.
     */
    const std::basic_string<CharT> &string() const;

    // TODO: Implement
    /**
     * Retrieve the number represented by the primitive
     * @returns the number that the primitive represents
     */
    float number() const;

    // TODO: Implement
    /**
     * Determines if the primitve is null.
     * @returns if the primitive is null.
     */
    bool isNull() const;

    /**
     * Retrieve a reference to the string represented by the primitive
     * @returns a reference to the string.
     */
    std::basic_string<CharT> &string();

private:
    Type _type;
    std::variant<ObjectData, ArrayData, PrimitiveData> _data;
};

/// Factory Methods

/**
 * Make an empty object.
 * @returns an empty object.
 */
template<typename CharT = char>
BasicValue<CharT> makeObject();

/**
 * Make an empty array.
 * @returns an empty array.
 */
template<typename CharT = char>
BasicValue<CharT> makeArray();

/**
 * Make an empty primitive.
 * @returns an empty primitive.
 */
template<typename CharT = char>
BasicValue<CharT> makePrimitive();

/**
 * Make a null primitive.
 * @returns a null primitive.
 */
template<typename CharT = char>
BasicValue<CharT> makeNull();

} // namespace json

// Implementation

namespace json
{
/// Constructor and Destructor

/**
 * Construct json value of the specifiec type
 * @param type type of the json value
 */
template<typename CharT>
BasicValue<CharT>::BasicValue(Type type)
    : _type(type)
{
    // variant will construct ObjectData in its default constructor
    switch (_type)
    {
    case Type::array:
        _data.template emplace<ArrayData>();
        break;
    case Type::primitive:
        _data.template emplace<PrimitiveData>();
        break;
    default:
        break;
    }
}

template<typename CharT>
BasicValue<CharT>::~BasicValue()
{
}

/// Type modifiers

/**
 * Get the type of object.
 * @returns the type of the object
 */
template<typename CharT>
typename BasicValue<CharT>::Type BasicValue<CharT>::type() const
{
    return _type;
}

/**
 * Determines if the value is an object.
 * @returns true if value is object
 */
template<typename CharT>
bool BasicValue<CharT>::isObject() const
{
    return _type == Type::object;
}

/**
 * Determines if the value is an array.
 * @returns true if value is array
 */
template<typename CharT>
bool BasicValue<CharT>::isArray() const
{
    return _type == Type::array;
}

/**
 * Determines if the value is a primitive.
 * @returns true if value is primitive
 */
template<typename CharT>
bool BasicValue<CharT>::isPrimitive() const
{
    return _type == Type::primitive;
}

// Universal accessors

/**
 * Get the size of the value
 * @returns type = object: size of unordered map; type = array: size of
 * vector; type = primitive: size of string.
 */
template<typename CharT>
size_t BasicValue<CharT>::size() const
{
    using std::get;

    switch (_type)
    {
    case Type::object:
    {
        const ObjectData &data = get<ObjectData>(_data);
        return data.size();
    }
    case Type::array:
    {
        const ArrayData &data = get<ArrayData>(_data);
        return data.size();
    }
    case Type::primitive:
    {
        const PrimitiveData &data = get<PrimitiveData>(_data);
        return data.size();
    }
    default:
        return 0;
    }
}

/// Object modifiers

/**
 * Set the a value using the key
 * @param value the value
 * @param key the key used to set the value
 * @discussions if a value associated with the key already exits,
 * override the existing value.
 */
template<typename CharT>
void BasicValue<CharT>::set(const Key &key, BasicValue<CharT> &value)
{
    ObjectData &data = std::get<ObjectData>(_data);
    data.insert_or_assign(key, value);
}

/// Object accessors

/**
 * Have a reference to the value associated with the key.
 * @param key the key associated with the value
 * @returns a reference to the value
 */
template<typename CharT>
BasicValue<CharT> &BasicValue<CharT>::operator[](const Key &key)
{
    ObjectData &data = std::get<ObjectData>(_data);
    return data[key];
}

/**
 * Have a reference to the value associated with the key.
 * @param key the key associated with the value
 * @returns a reference to the value
 */
template<typename CharT>
BasicValue<CharT> &BasicValue<CharT>::get(const Key &key)
{
    ObjectData &data = std::get<ObjectData>(_data);
    return data.find(key)->second;
}

/**
 * Have a constant reference to the value associated with the key.
 * @param key the key associated with the value
 * @returns a reference to the value
 */
template<typename CharT>
const BasicValue<CharT> &BasicValue<CharT>::get(const Key &key) const
{
    ObjectData &data = std::get<ObjectData>(_data);
    return data.find(key)->second;
}

template<typename CharT>
bool BasicValue<CharT>::contains(const Key &key) const
{
    const ObjectData &data = std::get<ObjectData>(_data);
    auto found = data.find(key);
    
    return found != data.end();
}

/// Array modifiers

/**
 * Add new element to the array.
 * @value the value to add
 */
template<typename CharT>
void BasicValue<CharT>::append(const BasicValue &value)
{
    ArrayData &data = std::get<ArrayData>(_data);
    data.push_back(value);
}

/// Array accessors

/**
 * Retreive a constant reference to an element at the index
 * @index the index of the element
 * @returns a constant reference to the json object
 */
template<typename CharT>
const BasicValue<CharT> &BasicValue<CharT>::operator[](size_t index) const
{
    ArrayData &data = std::get<ArrayData>(_data);
    return data[index];
}

/**
 * Retreive a reference to an element at the index
 * @index the index of the element
 * @returns a reference to the json object
 */
template<typename CharT>
BasicValue<CharT> &BasicValue<CharT>::operator[](size_t index)
{
    ArrayData &data = std::get<ArrayData>(_data);
    return data[index];
}

/// Primitive modifiers

/**
 * Set the value of the primitive to a string value.
 * @value the value to set
 */
template<typename CharT>
void BasicValue<CharT>::string(const std::basic_string<CharT> &text)
{
    PrimitiveData &data = std::get<PrimitiveData>(_data);
    data = text;
}

/// Primitive accessors

/**
 * Retrieve a constant reference to the string represented by the primitive
 * @returns a constant reference to the string.
 */
template<typename CharT>
const std::basic_string<CharT> &BasicValue<CharT>::string() const
{
    return std::get<PrimitiveData>(_data);
}

/**
 * Retrieve a reference to the string represented by the primitive
 * @returns a reference to the string.
 */
template<typename CharT>
std::basic_string<CharT> &BasicValue<CharT>::string()
{
    return std::get<PrimitiveData>(_data);
}

/// Factory Methods

/**
 * Make an empty object.
 * @returns an empty object.
 */
template<typename CharT>
BasicValue<CharT> makeObject()
{
    return { BasicValue<CharT>::Type::object };
}

/**
 * Make an empty array.
 * @returns an empty array.
 */
template<typename CharT>
BasicValue<CharT> makeArray()
{
    return { BasicValue<CharT>::Type::array };
}

/**
 * Make an empty primitive.
 * @returns an empty primitive.
 */
template<typename CharT>
BasicValue<CharT> makePrimitive()
{
    return { BasicValue<CharT>::Type::primitive };
}

/**
 * Make a null primitive.
 * @returns a null primitive.
 */
template<typename CharT>
BasicValue<CharT> makeNull()
{
    return { BasicValue<CharT>::Type::primitive };
}

} // namespace json