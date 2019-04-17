//
//  Tokenizer.hpp
//  CPP
//
//  Created by Zehua Chen on 4/15/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#pragma once

#include "json/Token/Token.hpp"

namespace json::token
{
template<typename CharT>
class Tokenizer
{
public:
    /**
     * Take an input iterator to extract letter to process
     * @param iter the input iterator to extract the letter from.
     * @param end the end input iterator
     * @returns returns after a iterator has been found
     */
    template<typename IterT>
    void take(IterT &iter, const IterT &end);

    /**
     * Get a reference to the current token
     * @returns a reference to the current token
     */
    Token<CharT> &token();

private:
    Token<CharT> _token;
};
} // namespace json::token

// Implementations

namespace json::token
{
template<typename CharT>
template<typename IterT>
void Tokenizer<CharT>::take(IterT &iter, const IterT &end)
{
    while (iter != end)
    {
        
    }
    
    _token.type = Token<CharT>::Type::endOfString;
}

template<typename CharT>
Token<CharT> &Tokenizer<CharT>::token()
{
    return _token;
}
} // namespace json::token
