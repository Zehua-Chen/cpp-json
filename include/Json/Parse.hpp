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
template<typename CharT = char, typename Iter>
BasicValue<CharT> parse(Iter begin, Iter end);
} // namespace json

// Implementations

namespace json
{
template<typename CharT, typename Iter>
BasicValue<CharT> parse(Iter begin, Iter end)
{
    using json::assemble::Assembler;
    using json::token::Tokenizer;

    Tokenizer<CharT> tokenizer;
    Assembler<CharT> assembler;

    tokenizer.tokenize(begin, end, assembler);

    BasicValue<CharT> root = std::move(assembler.root());

    return root;
}
} // namespace json