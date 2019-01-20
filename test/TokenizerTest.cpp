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
    // Simple object
    {
        string json = "{ 'name': \"a\" }";

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{
            { "", TokenType::beginObject },
            { "name", TokenType::key },
            { "a", TokenType::value },
            { "", TokenType::endObject },
        };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(json.begin(), json.end(), recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }

    // Simple array
    {
        string json = "[ 'a', 'b', 'c' ]";

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{
            { "", TokenType::beginArray }, { "a", TokenType::value },
            { "b", TokenType::value },     { "c", TokenType::value },
            { "", TokenType::endArray },
        };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(json.begin(), json.end(), recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }

    // Single string
    {
        string json = "'text'";

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{ { "text", TokenType::value } };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(json.begin(), json.end(), recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }
}

TEST(TokenizerTest, SingleLineComment)
{
    // Object with comment using unix endline
    {
        stringstream ss;
        ss << "{\n"
           << "  // I am a philosophor\n"
           << "  'name': 'a'\n"
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

    // Object with comment using Windows endline
    {
        stringstream ss;
        ss << "{\r\n"
           << "  // I am a philosophor\r\n"
           << "  'name': 'a'\r\n"
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
}

TEST(TokenizerTest, MultiLineComment)
{
    // Object with comment using unix endline
    {
        stringstream ss;
        ss << "{\n"
           << "  /*"
           << "   * I am a philosophor\n"
           << "   * Surprize!\n"
           << "   */\n"
           << "  'name': 'a'\n"
           << "}";

        string json = ss.str();

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{
            { "", TokenType::beginObject },
            { "I am a philosophor\nSurprize", TokenType::comment },
            { "name", TokenType::key },
            { "a", TokenType::value },
            { "", TokenType::endObject },
        };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(json.begin(), json.end(), recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }
}