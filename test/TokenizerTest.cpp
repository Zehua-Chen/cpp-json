#include "Json/Tokenizer.hpp"
#include "gtest/gtest.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

using namespace json;

namespace tokenizer_test
{
    template<typename CharT>
    struct TokenCopy
    {
        std::basic_string<CharT> value;
        TokenType type;
        
        TokenCopy(const std::basic_string<CharT> &value, TokenType type)
            : value(value)
            , type(type)
        {
        }
        
        TokenCopy(const Token<CharT> &token)
            : value(token.view)
            , type(token.type)
        {
        }
        
        bool operator==(const TokenCopy<CharT> &other) const
        {
            if (other.type != type)
            {
                return false;
            }
            
            if (other.value != value)
            {
                return false;
            }
            
            return true;
        }
    };
}

using namespace tokenizer_test;

TEST(TokenizerTest, Simple)
{
    string json = "{ 'name': 'a' }";
    
    Tokenizer<char> tokenizer;
    vector<TokenCopy<char>> tokens;
    vector<TokenCopy<char>> expectedTokens{
        { "", TokenType::beginObject },
        { "name", TokenType::key },
        { "a", TokenType::value },
        { "", TokenType::endObject },
    };
    
    const auto recorder = [&](const Token<char> &token){
        tokens.emplace_back(token);
    };
    
    tokenizer.tokenize(json.begin(), json.end(), recorder);
    
    EXPECT_EQ(tokens, expectedTokens);
}