#include "Json/Tokenizer.hpp"
#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

using namespace json;

TEST(TokenizerTest, Simple)
{
    string json = "{ 'name': 'a' }";

    Tokenizer<char> tokenizer;
    vector<Token<char>> tokens;
    vector<Token<char>> expectedTokens{
        { "", TokenType::beginObject },
        { "name", TokenType::key },
        { "a", TokenType::value },
        { "", TokenType::endObject },
    };

    const auto recorder = [&](const Token<char> &token) {
        tokens.push_back(token);
    };

    tokenizer.tokenize(json.begin(), json.end(), recorder);

    EXPECT_EQ(tokens, expectedTokens);
}