//
//  Tokenizer.hpp
//  CPP
//
//  Created by Zehua Chen on 4/15/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#pragma once

#include "json/Value/BasicValue.hpp"

namespace json::parser
{
template<typename CharT, typename TokenizerT, typename ComposerT>
class Parser
{
public:
    Parser();

    template<typename IterT>
    BasicValue<CharT> parse(IterT begin, IterT end);

private:
    TokenizerT _tokenizer;
    ComposerT _composer;
};
} // namespace json::parser

// Implementations

namespace json::parser
{
template<typename CharT, typename TokenizerT, typename ComposerT>
template<typename IterT>
BasicValue<CharT>
Parser<CharT, TokenizerT, ComposerT>::parse(IterT begin, IterT end)
{
    while (begin != end)
    {
        _tokenizer.take(begin);
    }
}
} // namespace json::parser