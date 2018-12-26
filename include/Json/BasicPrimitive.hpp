#pragma once

#include "Json/BasicValue.hpp"
#include <string>
#include <string_view>

namespace json
{
template<class CharT>
class BasicPrimitive: public BasicValue<CharT>
{  
public:
    using Key = typename BasicValue<CharT>::Key;
    using Value = typename BasicValue<CharT>::Value;
    
    explicit BasicPrimitive(const std::basic_string<CharT> &value);
    BasicPrimitive(const std::basic_string_view<CharT> &value);
    
    virtual ~BasicPrimitive() override;
    
    virtual Value * operator[](const Key &key) override;
    virtual Value * get(const Key &key) override;
    virtual void set(const Key &key, Value *value) override;
private:
    std::string _value;
};
}

// Implementation
namespace json
{
template<class CharT>
BasicPrimitive<CharT>::~BasicPrimitive() {}

template<class CharT>
typename BasicPrimitive<CharT>::Value * BasicPrimitive<CharT>::operator[](
    const Key &key)
{
    return nullptr;
}

template<class CharT>
typename BasicPrimitive<CharT>::Value * BasicPrimitive<CharT>::get(
    const Key &key)
{
}

template<class CharT>
void BasicPrimitive<CharT>::set(const Key &key, Value *value)
{
}
}