//
//  Keywords.hpp
//  CPP
//
//  Created by Zehua Chen on 1/4/19.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#pragma once

#include <string_view>

// Keywords declarations

namespace json::keywords
{
template<typename CharT>
inline constexpr CharT beginObject{};

template<typename CharT>
inline constexpr CharT endObject{};

template<typename CharT>
inline constexpr CharT beginArray{};

template<typename CharT>
inline constexpr CharT endArray{};

template<typename CharT>
inline constexpr CharT comma{};

template<typename CharT>
inline constexpr CharT colon;

template<typename CharT>
inline constexpr CharT endline{};

template<typename CharT>
inline constexpr CharT carriageReturn{};

template<typename CharT>
inline constexpr CharT space{};

template<typename CharT>
inline constexpr CharT tab{};

template<typename CharT>
inline constexpr CharT singleQuote{};

template<typename CharT>
inline constexpr CharT doubleQuote{};

template<typename CharT>
inline constexpr CharT backSlash{};

template<typename CharT>
inline constexpr CharT star{};

template<typename CharT>
inline const std::basic_string_view<CharT> singleLineComment{};

template<typename CharT>
inline const std::basic_string_view<CharT> beginMultiLineComment{};

template<typename CharT>
inline const std::basic_string_view<CharT> endMultiLineComment{};

// utf8 Specialization

template<>
inline constexpr char beginObject<char>{ '{' };

template<>
inline constexpr char endObject<char>{ '}' };

template<>
inline constexpr char beginArray<char>{ '[' };

template<>
inline constexpr char endArray<char>{ ']' };

template<>
inline constexpr char comma<char>{ ',' };

template<>
inline constexpr char colon<char>{ ':' };

template<>
inline constexpr char endline<char>{ '\n' };

template<>
inline constexpr char carriageReturn<char>{ '\r' };

template<>
inline constexpr char space<char>{ ' ' };

template<>
inline constexpr char tab<char>{ '\t' };

template<>
inline constexpr char singleQuote<char>{ '\'' };

template<>
inline constexpr char doubleQuote<char>{ '\"' };

template<>
inline constexpr char backSlash<char>{ '/' };

template<>
inline constexpr char star<char>{ '*' };

template<>
inline const std::basic_string_view<char> singleLineComment<char>{ "//" };

template<>
inline const std::basic_string_view<char> beginMultiLineComment<char>{ "/*" };

template<>
inline const std::basic_string_view<char> endMultiLineComment<char>{ "*/" };

// utf16 Specialization

template<>
inline constexpr char16_t beginObject<char16_t>{ u'{' };

template<>
inline constexpr char16_t endObject<char16_t>{ u'}' };

template<>
inline constexpr char16_t beginArray<char16_t>{ u'[' };

template<>
inline constexpr char16_t endArray<char16_t>{ u']' };

template<>
inline constexpr char16_t comma<char16_t>{ u',' };

template<>
inline constexpr char16_t colon<char16_t>{ u':' };

template<>
inline constexpr char16_t endline<char16_t>{ u'\n' };

template<>
inline constexpr char16_t carriageReturn<char16_t>{ u'\r' };

template<>
inline constexpr char16_t space<char16_t>{ u' ' };

template<>
inline constexpr char16_t tab<char16_t>{ u'\t' };

template<>
inline constexpr char16_t singleQuote<char16_t>{ u'\'' };

template<>
inline constexpr char16_t doubleQuote<char16_t>{ u'\"' };

template<>
inline constexpr char16_t backSlash<char16_t>{ u'/' };

template<>
inline constexpr char16_t star<char16_t>{ u'*' };

template<>
inline const std::basic_string_view<char16_t> singleLineComment<char16_t>{
    u"//"
};

template<>
inline const std::basic_string_view<char16_t> beginMultiLineComment<char16_t>{
    u"/*"
};

template<>
inline const std::basic_string_view<char16_t> endMultiLineComment<char16_t>{
    u"*/"
};

// utf32 Specialization

template<>
inline constexpr char32_t beginObject<char32_t>{ U'{' };

template<>
inline constexpr char32_t endObject<char32_t>{ U'}' };

template<>
inline constexpr char32_t beginArray<char32_t>{ U'[' };

template<>
inline constexpr char32_t endArray<char32_t>{ U']' };

template<>
inline constexpr char32_t comma<char32_t>{ U',' };

template<>
inline constexpr char32_t colon<char32_t>{ U':' };

template<>
inline constexpr char32_t endline<char32_t>{ U'\n' };

template<>
inline constexpr char32_t carriageReturn<char32_t>{ U'\r' };

template<>
inline constexpr char32_t space<char32_t>{ U' ' };

template<>
inline constexpr char32_t tab<char32_t>{ U'\t' };

template<>
inline constexpr char32_t singleQuote<char32_t>{ U'\'' };

template<>
inline constexpr char32_t doubleQuote<char32_t>{ U'\"' };

template<>
inline constexpr char32_t backSlash<char32_t>{ U'/' };

template<>
inline constexpr char32_t star<char32_t>{ U'*' };

template<>
inline const std::basic_string_view<char32_t> singleLineComment<char32_t>{
    U"//"
};

template<>
inline const std::basic_string_view<char32_t> beginMultiLineComment<char32_t>{
    U"/*"
};

template<>
inline const std::basic_string_view<char32_t> endMultiLineComment<char32_t>{
    U"*/"
};

// wchar_t Specialization

template<>
inline constexpr wchar_t beginObject<wchar_t>{ L'{' };

template<>
inline constexpr wchar_t endObject<wchar_t>{ L'}' };

template<>
inline constexpr wchar_t beginArray<wchar_t>{ L'[' };

template<>
inline constexpr wchar_t endArray<wchar_t>{ L']' };

template<>
inline constexpr wchar_t comma<wchar_t>{ L',' };

template<>
inline constexpr wchar_t colon<wchar_t>{ L':' };

template<>
inline constexpr wchar_t endline<wchar_t>{ L'\n' };

template<>
inline constexpr wchar_t carriageReturn<wchar_t>{ L'\r' };

template<>
inline constexpr wchar_t space<wchar_t>{ L' ' };

template<>
inline constexpr wchar_t tab<wchar_t>{ L'\t' };

template<>
inline constexpr wchar_t singleQuote<wchar_t>{ L'\'' };

template<>
inline constexpr wchar_t doubleQuote<wchar_t>{ L'\"' };

template<>
inline constexpr wchar_t backSlash<wchar_t>{ L'/' };

template<>
inline constexpr wchar_t star<wchar_t>{ L'*' };

template<>
inline const std::basic_string_view<wchar_t> singleLineComment<wchar_t>{
    L"//"
};

template<>
inline const std::basic_string_view<wchar_t> beginMultiLineComment<wchar_t>{
    L"/*"
};

template<>
inline const std::basic_string_view<wchar_t> endMultiLineComment<wchar_t>{
    L"*/"
};
} // namespace json::keywords