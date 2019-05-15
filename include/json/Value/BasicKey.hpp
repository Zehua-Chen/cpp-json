#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <variant>

namespace json
{
template<typename CharT>
class BasicKey
    : public std::
          variant<std::basic_string<CharT>, std::basic_string_view<CharT>>
{
public:
    BasicKey(const std::basic_string<CharT> &key);
    BasicKey(const CharT *key);

    bool hasOwnership() const;
   
};
} // namespace json

namespace json
{
template<typename CharT>
BasicKey<CharT>::BasicKey(const std::basic_string<CharT> &key)
{
    this->template emplace<0>(key);
}

template<typename CharT>
BasicKey<CharT>::BasicKey(const CharT *key)
{
    this->template emplace<1>(key);
}


template<typename CharT>
bool BasicKey<CharT>::hasOwnership() const
{
    return this->index() == 0;
}
}

// Hashing

namespace std
{
template<typename CharT>
struct hash<json::BasicKey<CharT>>
{
size_t operator()(const json::BasicKey<CharT> &key)
{
    switch (key.index())
    {
    case 0:
    {
        hash<basic_string<CharT>> hasher;
        return hasher(get<basic_string<CharT>>(key));
    }
    case 1:
    {
        hash<basic_string_view<CharT>> hasher;
        return hasher(get<basic_string_view<CharT>>(key));
    }
    default:
        return 0;
    }
}
};
}