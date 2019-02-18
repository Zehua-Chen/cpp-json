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
            { Token<char>::Type::beginObject },
            { Token<char>::Type::key, "name" },
            { Token<char>::Type::value, "a" },
            { Token<char>::Type::endObject },
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
            { Token<char>::Type::beginArray },
            { Token<char>::Type::value, "a" },
            { Token<char>::Type::value, "b" },
            { Token<char>::Type::value, "c" },
            { Token<char>::Type::endArray },
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

        vector<Token<char>> expectedTokens{};
        expectedTokens.emplace_back(Token<char>::Type::value, "text");

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(json.begin(), json.end(), recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }

    // Single Number
    {
        string json = "12.33";

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;

        vector<Token<char>> expectedTokens{};
        expectedTokens.emplace_back(Token<char>::Type::value, "12.33");

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(json.begin(), json.end(), recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }
    
    // Single Bools
    {
        string json = "false";

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;

        vector<Token<char>> expectedTokens{};
        expectedTokens.emplace_back(Token<char>::Type::value, "false");

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
           << "    'age': 12,"
           << "    'b': {"
           << "      'gender': 'male',"
           << "      'c': {"
           << "        'd': {},"
           << "        'alive': true"
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
            { Token<char>::Type::beginObject },
            { Token<char>::Type::key, "name" },
            { Token<char>::Type::value, "philosophor" },
            { Token<char>::Type::key, "a" },
            // level 2
            { Token<char>::Type::beginObject },
            { Token<char>::Type::key, "age" },
            { Token<char>::Type::value, "12" },
            { Token<char>::Type::key, "b" },
            // level 3
            { Token<char>::Type::beginObject },
            { Token<char>::Type::key, "gender" },
            { Token<char>::Type::value, "male" },
            { Token<char>::Type::key, "c" },
            // level 4
            { Token<char>::Type::beginObject },
            { Token<char>::Type::key, "d" },
            // level 5
            { Token<char>::Type::beginObject },
            { Token<char>::Type::endObject },
            // end level 5
            { Token<char>::Type::key, "alive" },
            { Token<char>::Type::value, "true" },
            // end level 4
            { Token<char>::Type::endObject },
            // end level 3
            { Token<char>::Type::endObject },
            // end level 2
            { Token<char>::Type::endObject },
            // end level 1
            { Token<char>::Type::endObject },
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
            { Token<char>::Type::beginArray },
            // Nested 1-1
            { Token<char>::Type::beginArray },
            // Netsed 2-1
            { Token<char>::Type::beginArray },
            { Token<char>::Type::value, "a" },
            { Token<char>::Type::value, "b" },
            // End Nested 2-1
            { Token<char>::Type::endArray },
            // Netsed 2-2
            { Token<char>::Type::beginArray },
            { Token<char>::Type::value, "a" },
            { Token<char>::Type::value, "b" },
            { Token<char>::Type::endArray },
            // End Nested 2-2
            { Token<char>::Type::endArray },
            // Nested 1-2
            { Token<char>::Type::beginArray },
            // Netsed 2-1
            { Token<char>::Type::beginArray },
            { Token<char>::Type::value, "a" },
            { Token<char>::Type::value, "b" },
            // End Nested 2-1
            { Token<char>::Type::endArray },
            // Netsed 2-2
            { Token<char>::Type::beginArray },
            { Token<char>::Type::value, "a" },
            { Token<char>::Type::value, "b" },
            { Token<char>::Type::endArray },
            // End Nested 2-2
            { Token<char>::Type::endArray },
            // End root
            { Token<char>::Type::endArray },
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
            { Token<char>::Type::beginObject },
            { Token<char>::Type::key, "a" },
            { Token<char>::Type::value, "a value" },
            { Token<char>::Type::key, "b" },
            { Token<char>::Type::value, "b value" },
            { Token<char>::Type::key, "collection" },
            { Token<char>::Type::beginArray },
            { Token<char>::Type::value, "a" },
            { Token<char>::Type::value, "b" },
            { Token<char>::Type::endArray },
            { Token<char>::Type::key, "nested" },
            { Token<char>::Type::beginObject },
            { Token<char>::Type::key, "collection" },
            { Token<char>::Type::beginArray },
            { Token<char>::Type::value, "a" },
            { Token<char>::Type::value, "b" },
            { Token<char>::Type::endArray },
            { Token<char>::Type::endObject },
            { Token<char>::Type::endObject }
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
           << "  //I am a philosophor\n"
           << "  'name': 'a'\n"
           << "}";

        istreambuf_iterator<char> begin{ ss };
        istreambuf_iterator<char> end{};

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{
            { Token<char>::Type::beginObject },
            { Token<char>::Type::comment, "I am a philosophor" },
            { Token<char>::Type::key, "name" },
            { Token<char>::Type::value, "a" },
            { Token<char>::Type::endObject },
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
           << "  //I am a philosophor\r\n"
           << "  'name': 'a'\r\n"
           << "}";

        istreambuf_iterator<char> begin{ ss };
        istreambuf_iterator<char> end{};

        Tokenizer<char> tokenizer;
        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{
            { Token<char>::Type::beginObject },
            { Token<char>::Type::comment, "I am a philosophor" },
            { Token<char>::Type::key, "name" },
            { Token<char>::Type::value, "a" },
            { Token<char>::Type::endObject },
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
            { Token<char>::Type::beginObject },
            { Token<char>::Type::comment, "I am a philosophor\nSurprize!\n" },
            { Token<char>::Type::key, "name" },
            { Token<char>::Type::value, "a" },
            { Token<char>::Type::endObject },
        };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(begin, end, recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }
}

TEST(TokenizerTest, Context)
{
    {
        string json = "\"some value = {}[]:,\n\"";
        Tokenizer<char> tokenizer;

        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{ { Token<char>::Type::value,
                                              "some value = {}[]:,\n" } };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(json.begin(), json.end(), recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }

    {
        string json = "\"the 'philosophor'\"";
        Tokenizer<char> tokenizer;

        vector<Token<char>> tokens;
        vector<Token<char>> expectedTokens{ { Token<char>::Type::value,
                                              "the 'philosophor'" } };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(json.begin(), json.end(), recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }
}