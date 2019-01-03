#pragma once

#include "Json/Token.hpp"
#include <iostream>

namespace json
{
template<typename CharT>
class Tokenizer
{
public:
    Tokenizer();
    
    template<typename Iter, typename Callback>
    void tokenize(Iter begin, Iter end, Callback callback);
private:
    Token<CharT> _token;
};
} // namespace json

namespace json
{
template<typename CharT>
Tokenizer<CharT>::Tokenizer()
{
    
}

template<typename CharT>
template<typename Iter, typename Callback>
void Tokenizer<CharT>::tokenize(Iter begin, Iter end, Callback callback)
{
    
    while (begin != end)
    {
        CharT letter = *begin;
        _token.append(letter);
        
        callback(_token);
        _token.reset();
        ++begin;
    }
}
} // namespace json