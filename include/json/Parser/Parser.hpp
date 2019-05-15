//
//  Tokenizer.hpp
//  CPP
//
//  Created by Zehua Chen on 4/15/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#pragma once

namespace json
{
template<typename CharT>
class Parser
{
public: 
    template<typename IterT>
    void parse(IterT begin, IterT end);
};
}

namespace json
{
template<typename CharT>
template<typename IterT>
void Parser<CharT>::parse(IterT begin, IterT end)
{

}
}