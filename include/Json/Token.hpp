#pragma once

#include <string>

namespace json
{
enum class TokenType
{
    beginObject,
    endObject,
    beginArray,
    endArray,
    key,
    value,
};

template<typename CharT>
struct Token
{
    Token(std::basic_string_view<CharT> view, TokenType type);
    bool operator==(const Token<CharT> &other) const;
    bool operator!=(const Token<CharT> &other) const;
    
    std::basic_string_view<CharT> view;
    TokenType type;
};
} // namespace json

// Implementation
namespace json
{
template<typename CharT>
Token<CharT>::Token(std::basic_string_view<CharT> view, TokenType type)
    : view(view)
    , type(type)
{
}

template<typename CharT>
bool Token<CharT>::operator==(const Token<CharT> &other) const
{
    if (other.type != type)
    {
        return false;
    }
    
    if (other.view != view)
    {
        return false;
    }
    
    return true;
}

template<typename CharT>
bool Token<CharT>::operator!=(const Token<CharT> &other) const
{
    if (other.type != type)
    {
        return true;
    }
    
    if (other.view != view)
    {
        return true;
    }
    
    return false;
}
}