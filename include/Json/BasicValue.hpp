#pragma once

#include <memory>
#include <string>

namespace json
{
template<class CharT>
class BasicValue
{
public:
    using Key = std::basic_string<CharT>;
    using Value = BasicValue<CharT>;

    virtual ~BasicValue();

    virtual Value *operator[](const Key &key) = 0;
    virtual Value *operator[](const CharT *key) = 0;

    virtual Value *get(const Key &key) = 0;
    virtual void set(const Key &key, Value *value) = 0;
};
} // namespace json

// Implementation
namespace json
{
template<class CharT>
inline BasicValue<CharT>::~BasicValue()
{
}
} // namespace json