//
//  Parse.hpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#pragma once

#include "Json/BasicValue.hpp"
#include "Json/Tokenizer.hpp"
#include "Json/Assembler.hpp"

// Declarations

namespace json
{
template<typename Iter, typename CharT = char>
BasicValue<CharT> parse(Iter begin, Iter end);
} // namespace json

// Implementations

namespace json
{
template<typename Iter, typename CharT>
BasicValue<CharT> parse(Iter begin, Iter end)
{
    using json::token::Tokenizer;
    using json::assemble::Assembler;
    
    Tokenizer<CharT> tokenizer;
    Assembler<CharT> assembler;
    
    tokenizer.tokenize(begin, end, assembler);
    
    BasicValue<CharT> root = assembler.root();
    
    return root;
}
} // namespace json