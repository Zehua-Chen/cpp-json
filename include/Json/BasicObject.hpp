#pragma once 

#include "Json/BasicValue.hpp"
#include <string>
#include <memory>
#include <unordered_map>

namespace json
{
template<class CharT>
class BasicObject: public BasicValue<CharT>
{
public:
    using Key = typename BasicValue<CharT>::Key;
    using Value = typename BasicValue<CharT>::Value;
    
    ~BasicObject();
    
    virtual Value * operator[](const Key &key) override;
    virtual Value * get(const Key &key) override;
    virtual void set(const Key &key, Value *value) override;

private:
    using _ValuePointer = std::unique_ptr<BasicValue<CharT>>;
    using _Map = std::unordered_map<Key, _ValuePointer>;
    
    _Map _map;
};
}


namespace json 
{
template<class CharT>
BasicObject<CharT>::~BasicObject()
{
}
    
template<class CharT>
typename BasicObject<CharT>::Value * BasicObject<CharT>::operator[](
    const Key &key) 
{ 
    auto found = _map.find(key);
    
    return found == _map.end() ? nullptr : found->second.get();
}

template<class CharT>
typename BasicObject<CharT>::Value * BasicObject<CharT>::get(
    const Key &key) 
{ 
    auto found = _map.find(key);
    
    return found == _map.end() ? nullptr : found->second.get();
}

template<class CharT>
void BasicObject<CharT>::set(const Key &key, Value *value) 
{
    _map[key].reset(value);
}
}
