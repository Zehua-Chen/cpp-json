#pragma once 

#include "json/Token/Token.hpp"
#include "json/Token/Tokenizer.hpp"

namespace json::parse
{
template<typename CharT>
class Composer
{
public:
    void take(const token::Token<CharT> &token);
};
} // namespace json

namespace json::parse
{
template<typename CharT>
void Composer<CharT>::take(const token::Token<CharT> &token)
{
    
}
} // namespace json
