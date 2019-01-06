#pragma once

#include <string_view>

namespace json
{
template<typename CharT>
struct Keywords
{
    // {
    constexpr static CharT beginObject = CharT{};
    // }
    constexpr static CharT endObject = CharT{};
    // [
    constexpr static CharT beginArray = CharT{};
    // ]
    constexpr static CharT endArray = CharT{};
    // //
    constexpr static std::basic_string_view<CharT> beginComment
        = std::basic_string_view<CharT>{};
    // ,
    constexpr static CharT keyValuesSeparator = CharT{};
    // ,
    constexpr static CharT propertiesSeparator = CharT{};
    // \n
    constexpr static CharT endline = CharT{};
    //
    constexpr static CharT space = CharT{};
    // \t
    constexpr static CharT tab = CharT{};
    // '
    constexpr static CharT singleQuote = CharT{};
    // '
    constexpr static CharT doubleQuote = CharT{};
};

template<>
struct Keywords<char>
{
    // {
    constexpr static char beginObject = '{';
    // }
    constexpr static char endObject = '}';
    // [
    constexpr static char beginArray = '[';
    // ]
    constexpr static char endArray = ']';
    // //
    constexpr static std::basic_string_view<char> beginComment = "//";
    // ,
    constexpr static char keyValuesSeparator = ':';
    // ,
    constexpr static char propertiesSeparator = ',';
    // \n
    constexpr static char endline = '\n';
    //
    constexpr static char space = ' ';
    //
    constexpr static char tab = '\t';
    // '
    constexpr static char singleQuote = '\'';
    // '
    constexpr static char doubleQuote = '\"';
};
} // namespace json