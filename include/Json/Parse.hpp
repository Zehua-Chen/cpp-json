//
//  Parse.hpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#pragma once

#include "Json/Assembler.hpp"
#include "Json/BasicValue.hpp"
#include "Json/Tokenizer.hpp"
#include <utility>

// Declarations

namespace json
{
/**
 * Parse a json value stored in a string in the range [begin, end)
 * @param begin inclusive begin
 * @param end exclusive end
 */
template<typename CharT = char, typename Iter>
BasicValue<CharT> parse(Iter begin, Iter end);
} // namespace json

// Implementations

namespace json
{
/**
 * Parse a json value stored in a string in the range [begin, end)
 * @param begin inclusive begin
 * @param end exclusive end
 */
template<typename CharT, typename Iter>
BasicValue<CharT> parse(Iter begin, Iter end)
{
    using json::assemble::Assembler;
    using json::token::Tokenizer;

    Tokenizer<CharT, Assembler<CharT>> tokenizer;

    tokenizer.tokenize(begin, end);

    BasicValue<CharT> root = std::move(tokenizer.output().root());

    return root;
}
} // namespace json