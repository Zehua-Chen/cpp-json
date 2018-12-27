#pragma once

#include "Json/BasicValue.hpp"
#include <string>
#include <string_view>
#include <utility>
#include <iostream>

namespace json
{
template<class CharT>
class BasicPrimitive: public BasicValue<CharT>
{  
public:
    using Key = typename BasicValue<CharT>::Key;
    using Value = typename BasicValue<CharT>::Value;
    
    BasicPrimitive(const std::basic_string<CharT> &value);
    BasicPrimitive(std::basic_string<CharT> &&value);
    
    virtual ~BasicPrimitive() override;
    
    virtual Value * operator[](const Key &key) override;
    virtual Value * operator[](const CharT *key) override;
    
    virtual Value * get(const Key &key) override;
    virtual void set(const Key &key, Value *value) override;
    
    std::basic_string<CharT> &value();
    const std::basic_string<CharT> &value() const;
private:
    std::basic_string<CharT> _value;
};
}

// Implementation
namespace json
{
using std::basic_string;
using std::basic_string_view;
using std::forward;
using std::cout;
using std::endl;

template<class CharT>
BasicPrimitive<CharT>::BasicPrimitive(const basic_string<CharT> &value) 
    : _value(value)
{
}

template<class CharT>
BasicPrimitive<CharT>::BasicPrimitive(basic_string<CharT> &&value) 
    : _value(forward<basic_string<CharT>>(value))
{
}

template<class CharT>
BasicPrimitive<CharT>::~BasicPrimitive() 
{
}

template<class CharT>
typename BasicPrimitive<CharT>::Value * BasicPrimitive<CharT>::operator[](
    const Key &key)
{
    return nullptr;
}

template<class CharT>
typename BasicPrimitive<CharT>::Value * BasicPrimitive<CharT>::operator[](
    const CharT *key)
{
    return nullptr;
}

template<class CharT>
typename BasicPrimitive<CharT>::Value * BasicPrimitive<CharT>::get(
    const Key &key)
{
    return nullptr;
}

template<class CharT>
void BasicPrimitive<CharT>::set(const Key &key, Value *value)
{
}

template<class CharT>
std::basic_string<CharT> &BasicPrimitive<CharT>::value() { return _value; }

template<class CharT>
const std::basic_string<CharT> &BasicPrimitive<CharT>::value() const
{
    return _value;
}

}