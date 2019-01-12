#include "Json/Tokenizer.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;

using namespace json;

TEST(TokenizerTest, Simple)
{
    vector<Token<char>> expectedTokens{
        { "", TokenType::beginObject },
        { "name", TokenType::key },
        { "a", TokenType::value },
        { "", TokenType::endObject },
    };

    {
        string json = "{ 'name': 'a' }";

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(json.begin(), json.end(), recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }

    {
        string json = "{ \"name\": \"a\" }";

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(json.begin(), json.end(), recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }
}

TEST(TokenizerTest, CommentEndingWithUnixEndline)
{
    stringstream ss;
    ss << "{"
       << "\n"
       << "// I am a philosophor"
       << "\n"
       << "'name': 'a'"
       << "\n"
       << "}";

    string json = ss.str();

    Tokenizer<char> tokenizer;
    vector<Token<char>> tokens;
    vector<Token<char>> expectedTokens{
        { "", TokenType::beginObject },
        { "I am a philosophor", TokenType::comment },
        { "name", TokenType::key },
        { "a", TokenType::value },
        { "", TokenType::endObject },
    };

    const auto recorder
        = [&](const Token<char> &token) { tokens.push_back(token); };

    tokenizer.tokenize(json.begin(), json.end(), recorder);

    EXPECT_EQ(tokens, expectedTokens);
}