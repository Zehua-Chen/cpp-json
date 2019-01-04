#pragma once

#include <string_view>

namespace json
{
template<typename CharT>
struct Keywords
{
    // {
    constexpr static std::basic_string_view<CharT> beginObject
        = std::basic_string_view<CharT>{};
    // }
    constexpr static std::basic_string_view<CharT> endObject
        = std::basic_string_view<CharT>{};
    // [
    constexpr static std::basic_string_view<CharT> beginArray
        = std::basic_string_view<CharT>{};
    // ]
    constexpr static std::basic_string_view<CharT> endArray
        = std::basic_string_view<CharT>{};
    // //
    constexpr static std::basic_string_view<CharT> beginComment
        = std::basic_string_view<CharT>{};
};

template<>
struct Keywords<char>
{
    // {
    constexpr static std::basic_string_view<char> beginObject = "{";
    // }
    constexpr static std::basic_string_view<char> endObject = "}";
    // [
    constexpr static std::basic_string_view<char> beginArray = "[";
    // ]
    constexpr static std::basic_string_view<char> endArray = "]";
    // //
    constexpr static std::basic_string_view<char> beginComment = "//";
};

template<>
struct Keywords<wchar_t>
{
    // {
    constexpr static std::basic_string_view<wchar_t> beginObject = L"{";
    // }
    constexpr static std::basic_string_view<wchar_t> endObject = L"}";
    // [
    constexpr static std::basic_string_view<wchar_t> beginArray = L"[";
    // ]
    constexpr static std::basic_string_view<wchar_t> endArray = L"]";
    // //
    constexpr static std::basic_string_view<wchar_t> beginComment = L"//";
};
} // namespace json