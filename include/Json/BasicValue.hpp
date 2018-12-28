#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <iostream>

namespace json
{
enum class Type
{
    object,
    array,
    primitive,
    null,
};

template<typename CharT>
class BasicValue
{
public:
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
    
    const BasicValue<CharT> &get(const Key &key) const;
    BasicValue<CharT> &get(const Key &key);
    const BasicValue<CharT> &operator[](const Key &key) const;

    // Array modifiers
    void append(const BasicValue &value);
    void erase(const size_t index);

    const BasicValue<CharT> &operator[](size_t index) const;
    BasicValue<CharT> &operator[](size_t index);

    // Primitive Modifiers
    void boolean(bool state);
    void string(const std::basic_string<CharT> &text);
    void number(float number);
    void nullify();

    bool boolean() const;
    const std::basic_string<CharT> &string() const;
    float number() const;
    bool isNull() const;
    
    std::basic_string<CharT> &string();

private:
    Type _type;
    std::variant<ObjectData, ArrayData, PrimitiveData> _data;
};

template<typename CharT = char>
BasicValue<CharT> makeObject();

template<typename CharT = char>
BasicValue<CharT> makeArray();

template<typename CharT = char>
BasicValue<CharT> makePrimitive();

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
Type BasicValue<CharT>::type() const
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

template<typename CharT>
const BasicValue<CharT> &BasicValue<CharT>::get(const Key &key) const
{
    ObjectData &data = std::get<ObjectData>(_data);
    return data.find(key)->second;
}

template<typename CharT>
BasicValue<CharT> &BasicValue<CharT>::get(const Key &key)
{
    ObjectData &data = std::get<ObjectData>(_data);
    return data.find(key)->second;
}

template<typename CharT>
void BasicValue<CharT>::string(const std::basic_string<CharT> &text)
{
    PrimitiveData &data = std::get<PrimitiveData>(_data);
    data = text;
}

template<typename CharT>
const std::basic_string<CharT> &BasicValue<CharT>::string() const
{
    return std::get<PrimitiveData>(_data);
}

template<typename CharT>
std::basic_string<CharT> &BasicValue<CharT>::string()
{
    return std::get<PrimitiveData>(_data);
}

template<typename CharT>
BasicValue<CharT> makeObject()
{
    return { Type::object };
}

template<typename CharT>
BasicValue<CharT> makeArray()
{
    return { Type::array };
}

template<typename CharT>
BasicValue<CharT> makePrimitive()
{
    return { Type::primitive };
}

template<typename CharT>
BasicValue<CharT> makeNull()
{
    return { Type::null };
}

} // namespace json