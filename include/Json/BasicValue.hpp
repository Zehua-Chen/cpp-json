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

    BasicValue(Type type);
    virtual ~BasicValue();

    // Type modifiers
    Type type() const;
    void type(Type type);

    bool isObject() const;
    bool isArray() const;
    bool isPrimitive() const;

    void becomeObject();
    void becomeArray();
    void becomePrimitive();
    void becomeEmpty();

    // Object modifiers
    void set(const Key &key, BasicValue<CharT> &value);
    const BasicValue<CharT> &get(const Key &key) const;
    BasicValue<CharT> &get(const Key &key);
    void erase(const Key &key);

    const BasicValue<CharT> &operator[](const Key &key) const;
    BasicValue<CharT> &operator[](const Key &key);

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
    float number();
    bool isNull() const;

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

template<typename CharT>
BasicValue<CharT>::~BasicValue()
{
}

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
Type BasicValue<CharT>::type() const
{
    return _type;
}

template<typename CharT>
void BasicValue<CharT>::set(const Key &key, BasicValue<CharT> &value)
{
    ObjectData &data = std::get<ObjectData>(_data);
    data.insert_or_assign(key, value);
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

} // namespace json