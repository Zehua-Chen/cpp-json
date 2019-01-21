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
            { "", TokenType::beginObject },
            { "name", TokenType::key },
            { "philosophor", TokenType::value },
            { "a", TokenType::key },
            // level 2
            { "", TokenType::beginObject },
            { "age", TokenType::key },
            { "12", TokenType::value },
            { "b", TokenType::key },
            // level 3
            { "", TokenType::beginObject },
            { "gender", TokenType::key },
            { "male", TokenType::value },
            { "c", TokenType::key },
            // level 4
            { "", TokenType::beginObject },
            { "d", TokenType::key },
            // level 5
            { "", TokenType::beginObject },
            { "", TokenType::endObject },
            // end level 5
            { "alive", TokenType::key },
            { "true", TokenType::value },
            // end level 4
            { "", TokenType::endObject },
            // end level 3
            { "", TokenType::endObject },
            // end level 2
            { "", TokenType::endObject },
            // end level 1
            { "", TokenType::endObject },
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
            { "", TokenType::beginArray },
            // Nested 1-1
            { "", TokenType::beginArray},
            // Netsed 2-1
            { "", TokenType::beginArray },
            { "a", TokenType::value },
            { "b", TokenType::value },
            // End Nested 2-1
            { "", TokenType::endArray },
            // Netsed 2-2
            { "", TokenType::beginArray },
            { "a", TokenType::value },
            { "b", TokenType::value },
            { "", TokenType::endArray },
            // End Nested 2-2
            { "", TokenType::endArray },
            // Nested 1-2
            { "", TokenType::beginArray},
            // Netsed 2-1
            { "", TokenType::beginArray },
            { "a", TokenType::value },
            { "b", TokenType::value },
            // End Nested 2-1
            { "", TokenType::endArray },
            // Netsed 2-2
            { "", TokenType::beginArray },
            { "a", TokenType::value },
            { "b", TokenType::value },
            { "", TokenType::endArray },
            // End Nested 2-2
            { "", TokenType::endArray },
            // End root
            { "", TokenType::endArray },
        };

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
            { "", TokenType::beginObject },
            { "I am a philosophor", TokenType::comment },
            { "name", TokenType::key },
            { "a", TokenType::value },
            { "", TokenType::endObject },
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
            { "", TokenType::beginObject },
            { "I am a philosophor", TokenType::comment },
            { "name", TokenType::key },
            { "a", TokenType::value },
            { "", TokenType::endObject },
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
            { "", TokenType::beginObject },
            { "I am a philosophor\nSurprize!\n", TokenType::comment },
            { "name", TokenType::key },
            { "a", TokenType::value },
            { "", TokenType::endObject },
        };

        const auto recorder
            = [&](const Token<char> &token) { tokens.push_back(token); };

        tokenizer.tokenize(begin, end, recorder);

        EXPECT_EQ(tokens, expectedTokens);
    }
}