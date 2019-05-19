#pragma once

#include "json/Value/BasicValue.hpp"
#include "json/Parser/Parser.hpp"

namespace json
{
template<typename CharT = char, typename IterT>
BasicValue<CharT> parse(IterT begin, IterT end);

using Value = BasicValue<char>;
using Key = BasicKey<char>;
}

namespace json
{
template<typename CharT, typename IterT>
BasicValue<CharT> parse(IterT begin, IterT end)
{
    token::Tokenizer<CharT, IterT> tokenizer{ begin, end };
    parser::Parser<CharT> parser;
    
    while(tokenizer)
    {
        tokenizer.extract();
        parser.take(tokenizer.token());
    }

    return parser.root();
}
}