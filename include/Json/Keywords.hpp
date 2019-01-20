#pragma once

#include <string_view>

namespace json
{
template<typename CharT>
struct Keywords
{
    using Letter = CharT;
    using String = std::basic_string<CharT>;

    // {
    constexpr static Letter beginObject = Letter{};
    // }
    constexpr static Letter endObject = Letter{};
    // [
    constexpr static Letter beginArray = Letter{};
    // ]
    constexpr static Letter endArray = Letter{};
    // //
    constexpr static String singleLineComment = String{};
    // /*
    constexpr static String beginMultilineComment = String{};
    // */
    constexpr static String endMultilineComment = String{};
    // ,
    constexpr static Letter keyValuesSeparator = Letter{};
    // ,
    constexpr static Letter propertiesSeparator = Letter{};
    // \n
    constexpr static Letter endline = Letter{};
    // \r
    constexpr static Letter carriageReturn = Letter{};
    //
    constexpr static Letter space = Letter{};
    // \t
    constexpr static Letter tab = Letter{};
    // '
    constexpr static Letter singleQuote = Letter{};
    // '
    constexpr static Letter doubleQuote = Letter{};
};

template<>
struct Keywords<char>
{
    using Letter = char;
    using String = std::basic_string_view<char>;

    // {
    constexpr static Letter beginObject = '{';
    // }
    constexpr static Letter endObject = '}';
    // [
    constexpr static Letter beginArray = '[';
    // ]
    constexpr static Letter endArray = ']';
    // //
    constexpr static String singleLineComment = "//";
    // /*
    constexpr static String beginMultilineComment = "/*";
    // */
    constexpr static String endMultilineComment = "*/";
    // ,
    constexpr static Letter keyValuesSeparator = ':';
    // ,
    constexpr static Letter propertiesSeparator = ',';
    // \n
    constexpr static Letter endline = '\n';
    // \r
    constexpr static Letter carriageReturn = '\r';
    //
    constexpr static Letter space = ' ';
    //
    constexpr static Letter tab = '\t';
    // '
    constexpr static Letter singleQuote = '\'';
    // '
    constexpr static Letter doubleQuote = '\"';
};
} // namespace json