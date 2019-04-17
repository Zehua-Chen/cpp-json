#include "gtest/gtest.h"
#include "json/Token/Tokenizer.hpp"
#include <string_view>

using std::string_view;
using json::token::Tokenizer;
using json::token::Token;

TEST(TokenizerTest, NonContextual)
{
    string_view json = "{}[],:";
    
    auto begin = json.begin();
    auto end = json.end();
    
    Tokenizer<char> tokenizer;
    
    while (!tokenizer.token().isEndOfString())
    {
        tokenizer.take(begin, end);
    }
}