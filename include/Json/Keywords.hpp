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
inline const std::basic_string_view<char> singleLineComment<char>{ "//" };

template<>
inline const std::basic_string_view<char> beginMultiLineComment<char>{ "/*" };

template<>
inline const std::basic_string_view<char> endMultiLineComment<char>{ "*/" };
} // namespace json::keywords