//
//  TokenizerTest.cpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#include "Json/Tokenizer.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::istreambuf_iterator;
using std::string;
using std::stringstream;
using std::vector;

using namespace json;
using namespace json::token;

TEST(TokenizerTest, Simple)
{
    // Simple object
    {
        string json = "{ 'name': \"a\" }";

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{
            { TokenType::beginObject },
            { TokenType::key, "name" },
            { TokenType::value, "a" },
            { TokenType::endObject },
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
            { TokenType::beginArray }, { TokenType::value, "a" },
            { TokenType::value, "b" }, { TokenType::value, "c" },
            { TokenType::endArray },
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
        vector<Token<char>> expectedTokens{ { TokenType::value, "text" } };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(json.begin(), json.end(), recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }
}

TEST(TokenizerTest, Nested)
{
    // Nested object
    {
        stringstream ss;
        ss << "{"
           << "  'name': 'philosophor',"
           << "  'a': {"
           << "    'age': '12',"
           << "    'b': {"
           << "      'gender': 'male',"
           << "      'c': {"
           << "        'd': {},"
           << "        'alive': 'true'"
           << "      }"
           << "    }"
           << "  }"
           << "}";

        istreambuf_iterator<char> begin{ ss };
        istreambuf_iterator<char> end{};

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{
            // level 1
            { TokenType::beginObject },
            { TokenType::key, "name" },
            { TokenType::value, "philosophor" },
            { TokenType::key, "a" },
            // level 2
            { TokenType::beginObject },
            { TokenType::key, "age" },
            { TokenType::value, "12" },
            { TokenType::key, "b" },
            // level 3
            { TokenType::beginObject },
            { TokenType::key, "gender" },
            { TokenType::value, "male" },
            { TokenType::key, "c" },
            // level 4
            { TokenType::beginObject },
            { TokenType::key, "d" },
            // level 5
            { TokenType::beginObject },
            { TokenType::endObject },
            // end level 5
            { TokenType::key, "alive" },
            { TokenType::value, "true" },
            // end level 4
            { TokenType::endObject },
            // end level 3
            { TokenType::endObject },
            // end level 2
            { TokenType::endObject },
            // end level 1
            { TokenType::endObject },
        };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(begin, end, recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }

    // Nested array
    {
        stringstream ss;
        ss << "["
           << "  ["
           << "    ['a', 'b'],"
           << "    ['a', 'b']"
           << "  ],"
           << "  ["
           << "    ['a', 'b'],"
           << "    ['a', 'b']"
           << "  ],"
           << "]";

        istreambuf_iterator<char> begin{ ss };
        istreambuf_iterator<char> end{};

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{
            // root
            { TokenType::beginArray },
            // Nested 1-1
            { TokenType::beginArray },
            // Netsed 2-1
            { TokenType::beginArray },
            { TokenType::value, "a" },
            { TokenType::value, "b" },
            // End Nested 2-1
            { TokenType::endArray },
            // Netsed 2-2
            { TokenType::beginArray },
            { TokenType::value, "a" },
            { TokenType::value, "b" },
            { TokenType::endArray },
            // End Nested 2-2
            { TokenType::endArray },
            // Nested 1-2
            { TokenType::beginArray },
            // Netsed 2-1
            { TokenType::beginArray },
            { TokenType::value, "a" },
            { TokenType::value, "b" },
            // End Nested 2-1
            { TokenType::endArray },
            // Netsed 2-2
            { TokenType::beginArray },
            { TokenType::value, "a" },
            { TokenType::value, "b" },
            { TokenType::endArray },
            // End Nested 2-2
            { TokenType::endArray },
            // End root
            { TokenType::endArray },
        };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(begin, end, recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }
}

TEST(TokenizerTest, Complex)
{
    {
        stringstream ss;
        ss << "{\n"
           << "  'a': 'a value', 'b': 'b value',\n"
           << "  'collection': ['a', 'b'],\n"
           << "  'nested': { 'collection': ['a', 'b'] }\n"
           << "}";

        istreambuf_iterator<char> begin{ ss };
        istreambuf_iterator<char> end{};

        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{
            { TokenType::beginObject },
            { TokenType::key, "a" },
            { TokenType::value, "a value" },
            { TokenType::key, "b" },
            { TokenType::value, "b value" },
            { TokenType::key, "collection" },
            { TokenType::beginArray },
            { TokenType::value, "a" },
            { TokenType::value, "b" },
            { TokenType::endArray },
            { TokenType::key, "nested" },
            { TokenType::beginObject },
            { TokenType::key, "collection" },
            { TokenType::beginArray },
            { TokenType::value, "a" },
            { TokenType::value, "b" },
            { TokenType::endArray },
            { TokenType::endObject },
            { TokenType::endObject }
        };

        Tokenizer<char> tokenizer;

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(begin, end, recorder);
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

        istreambuf_iterator<char> begin{ ss };
        istreambuf_iterator<char> end{};

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{
            { TokenType::beginObject },
            { TokenType::comment, "I am a philosophor" },
            { TokenType::key, "name" },
            { TokenType::value, "a" },
            { TokenType::endObject },
        };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(begin, end, recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }

    // Object with comment using Windows endline
    {
        stringstream ss;
        ss << "{\r\n"
           << "  // I am a philosophor\r\n"
           << "  'name': 'a'\r\n"
           << "}";

        istreambuf_iterator<char> begin{ ss };
        istreambuf_iterator<char> end{};

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{
            { TokenType::beginObject },
            { TokenType::comment, "I am a philosophor" },
            { TokenType::key, "name" },
            { TokenType::value, "a" },
            { TokenType::endObject },
        };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(begin, end, recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }
}

TEST(TokenizerTest, MultiLineComment)
{
    // Object with comment using unix endline
    {
        stringstream ss;
        ss << "{\n"
           << "/*"
           << "I am a philosophor\n"
           << "Surprize!\n"
           << "*/\n"
           << "'name': 'a'\n"
           << "}";

        istreambuf_iterator<char> begin{ ss };
        istreambuf_iterator<char> end{};

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{
            { TokenType::beginObject },
            { TokenType::comment, "I am a philosophor\nSurprize!\n" },
            { TokenType::key, "name" },
            { TokenType::value, "a" },
            { TokenType::endObject },
        };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(begin, end, recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }
}