#pragma once

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
    enum class Type
    {
        object,
        array,
        primitive
    };

    using Key = std::basic_string<CharT>;
    using ObjectData = std::unordered_map<Key, BasicValue<CharT>>;
    using ArrayData = std::vector<BasicValue<CharT>>;
    using PrimitiveData = std::optional<std::basic_string<CharT>>;

    BasicValue(Type type = Type::object);
    virtual ~BasicValue();

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
    void string(std::basic_string<CharT> &text);
    void number(float number);
    void nullify();
    
    bool boolean() const;
    std::basic_string<CharT> string() const;
    float number();
    bool isNull() const;
private:
    Type _type;
    std::variant<ObjectData, ArrayData, PrimitiveData> _data;
};

Value makeObject();
Value makeArray();
Value makePrimitive();
Value makeNull();

} // namespace json