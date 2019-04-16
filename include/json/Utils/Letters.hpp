//
//  Letters.hpp
//  CPP
//
//  Created by Zehua Chen on 4/15/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#pragma once

namespace json::utils
{
template<typename CharT>
inline constexpr CharT leftCurleyBrace{ '{' };

template<typename CharT>
inline constexpr CharT rightCurleyBrace{ '{' };

template<typename CharT>
inline constexpr CharT leftSquareBracket{ '[' };

template<typename CharT>
inline constexpr CharT rightSquareBracket{ ']' };

template<typename CharT>
inline constexpr CharT comma{ ',' };

template<typename CharT>
inline constexpr CharT colon{ ':' };

template<typename CharT>
inline constexpr CharT carriageReturn{ '\r' };

template<typename CharT>
inline constexpr CharT endline{ '\n' };

template<typename CharT>
inline constexpr CharT endline{ ' ' };
} // namespace json::utils