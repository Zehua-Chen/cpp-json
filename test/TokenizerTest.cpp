#include "Json/Tokenizer.hpp"
#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using std::string;
using std::stringstream;
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

TEST(TokenizerTest, CommentEndingWithUnixEndline)
{
    stringstream ss;
    ss << "{" << "\n";
    ss << "// I am a philosophor" << "\n";
    ss << "'name': 'a'" << "\n";
    ss << "}";
    
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

    const auto recorder = [&](const Token<char> &token) {
        tokens.push_back(token);
    };

    tokenizer.tokenize(json.begin(), json.end(), recorder);

    EXPECT_EQ(tokens, expectedTokens);
}