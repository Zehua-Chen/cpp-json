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
inline constexpr CharT leftCurlyBrace{};

template<typename CharT>
inline constexpr CharT rightCurlyBrace{};

template<typename CharT>
inline constexpr CharT leftSquareBracket{};

template<typename CharT>
inline constexpr CharT rightSquareBracket{};

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
inline constexpr CharT A{};

template<typename CharT>
inline constexpr CharT a{};

template<typename CharT>
inline constexpr CharT Z{};

template<typename CharT>
inline constexpr CharT z{};

template<typename CharT>
inline constexpr CharT zero{};

template<typename CharT>
inline constexpr CharT nine{};

template<typename CharT>
inline constexpr CharT dot{};

// utf8 Specialization

template<>
inline constexpr char leftCurlyBrace<char>{ '{' };

template<>
inline constexpr char rightCurlyBrace<char>{ '}' };

template<>
inline constexpr char leftSquareBracket<char>{ '[' };

template<>
inline constexpr char rightSquareBracket<char>{ ']' };

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
inline constexpr char A<char>{ 'A' };

template<>
inline constexpr char a<char>{ 'a' };

template<>
inline constexpr char Z<char>{ 'Z' };

template<>
inline constexpr char z<char>{ 'z' };

template<>
inline constexpr char zero<char>{ '0' };

template<>
inline constexpr char nine<char>{ '9' };

template<>
inline constexpr char dot<char>{ '.' };

// utf16 Specialization

template<>
inline constexpr char16_t leftCurlyBrace<char16_t>{ u'{' };

template<>
inline constexpr char16_t rightCurlyBrace<char16_t>{ u'}' };

template<>
inline constexpr char16_t leftSquareBracket<char16_t>{ u'[' };

template<>
inline constexpr char16_t rightSquareBracket<char16_t>{ u']' };

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
inline constexpr char16_t A<char16_t>{ u'A' };

template<>
inline constexpr char16_t a<char16_t>{ u'a' };

template<>
inline constexpr char16_t Z<char16_t>{ u'Z' };

template<>
inline constexpr char16_t z<char16_t>{ u'z' };

template<>
inline constexpr char16_t zero<char16_t>{ u'0' };

template<>
inline constexpr char16_t nine<char16_t>{ u'9' };

template<>
inline constexpr char16_t dot<char16_t>{ u'.' };

// utf32 Specialization

template<>
inline constexpr char32_t leftCurlyBrace<char32_t>{ U'{' };

template<>
inline constexpr char32_t rightCurlyBrace<char32_t>{ U'}' };

template<>
inline constexpr char32_t leftSquareBracket<char32_t>{ U'[' };

template<>
inline constexpr char32_t rightSquareBracket<char32_t>{ U']' };

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
inline constexpr char32_t A<char32_t>{ U'A' };

template<>
inline constexpr char32_t a<char32_t>{ U'a' };

template<>
inline constexpr char32_t Z<char32_t>{ U'Z' };

template<>
inline constexpr char32_t z<char32_t>{ U'z' };

template<>
inline constexpr char32_t zero<char32_t>{ U'0' };

template<>
inline constexpr char32_t nine<char32_t>{ U'9' };

template<>
inline constexpr char32_t dot<char32_t>{ U'.' };

// wchar_t Specialization

template<>
inline constexpr wchar_t leftCurlyBrace<wchar_t>{ L'{' };

template<>
inline constexpr wchar_t rightCurlyBrace<wchar_t>{ L'}' };

template<>
inline constexpr wchar_t leftSquareBracket<wchar_t>{ L'[' };

template<>
inline constexpr wchar_t rightSquareBracket<wchar_t>{ L']' };

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
inline constexpr wchar_t A<wchar_t>{ L'A' };

template<>
inline constexpr wchar_t a<wchar_t>{ L'a' };

template<>
inline constexpr wchar_t Z<wchar_t>{ L'Z' };

template<>
inline constexpr wchar_t z<wchar_t>{ L'z' };

template<>
inline constexpr wchar_t zero<wchar_t>{ L'0' };

template<>
inline constexpr wchar_t nine<wchar_t>{ L'9' };

template<>
inline constexpr wchar_t dot<wchar_t>{ L'.' };

} // namespace json::keywords