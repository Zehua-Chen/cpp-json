#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <variant>

namespace json
{
template<typename CharT>
class BasicKey
{
public:
    using String = std::basic_string<CharT>;
    using StringView = std::basic_string_view<CharT>;
    using Data = std::variant<String, StringView>;

    BasicKey(const std::basic_string<CharT> &key);
    BasicKey(const CharT *key);

    bool hasOwnership() const;
    bool operator==(const BasicKey<CharT> &other) const;

    int index() const;
    const Data &data() const;

private:
    Data _data;
};
} // namespace json

namespace json
{
template<typename CharT>
BasicKey<CharT>::BasicKey(const std::basic_string<CharT> &key)
{
    _data.template emplace<0>(key);
}

template<typename CharT>
BasicKey<CharT>::BasicKey(const CharT *key)
{
    _data.template emplace<1>(key);
}

template<typename CharT>
bool BasicKey<CharT>::hasOwnership() const
{
    return _data.index() == 0;
}

template<typename CharT>
int BasicKey<CharT>::index() const
{
    return _data.index();
}

template<typename CharT>
const typename BasicKey<CharT>::Data &BasicKey<CharT>::data() const
{
    return _data;
}

template<typename CharT>
bool BasicKey<CharT>::operator==(const BasicKey<CharT> &other) const
{
    using std::get;

    if (_data.index() == 0 && other.index() == 1)
    {
        return get<String>(_data) == get<StringView>(other._data);
    }
    else if (_data.index() == 1 && other.index() == 0)
    {
        return get<StringView>(_data) == get<String>(other._data);
    }

    return _data == other._data;
}
} // namespace json

// Hashing

namespace std
{
template<typename CharT>
struct hash<json::BasicKey<CharT>>
{
    size_t operator()(const json::BasicKey<CharT> &key) const
    {
        switch (key.index())
        {
        case 0:
        {
            hash<basic_string<CharT>> hasher;
            return hasher(get<basic_string<CharT>>(key.data()));
        }
        case 1:
        {
            hash<basic_string_view<CharT>> hasher;
            return hasher(get<basic_string_view<CharT>>(key.data()));
        }
        default:
            return 0;
        }
    }
};
} // namespace std