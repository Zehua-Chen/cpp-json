#include "json/Token/Tokenizer.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <string_view>
#include <vector>

using std::basic_string_view;
using std::cout;
using std::endl;
using std::string_view;
using std::vector;

using json::token::Token;
using json::token::Tokenizer;

using TType = Token<char>::Type;

template<typename CharT>
static vector<Token<CharT>> tokenize(basic_string_view<CharT> js)
{
    vector<Token<CharT>> output;
    Tokenizer<CharT, typename basic_string_view<CharT>::iterator> tokenizer{
        js.begin(), js.end()
    };

    while (tokenizer)
    {
        tokenizer.extract();
        output.push_back(tokenizer.token());
    }

    return output;
}

TEST(TokenizerTest, NonContextual)
{
    string_view json = "{ }\r[]\t,\n:";
    vector<Token<char>> tokens = tokenize(json);
    vector<Token<char>> expected{
        { TType::beginObject },    { TType::endObject },
        { TType::beginArray },     { TType::endArray },
        { TType::valueSeparator }, { TType::keyValueSeparator }
    };

    EXPECT_EQ(tokens, expected);
}

TEST(TokenizerTest, String)
{
    // Just string
    {
        string_view json = "\"abc\"";

        vector<Token<char>> tokens = tokenize(json);
        vector<Token<char>> expected{
            { "abc" },
        };

        EXPECT_EQ(tokens, expected);
    }

    // String with single quotes
    {
        string_view json = "\"'lo\\\"ng'\"";

        vector<Token<char>> tokens = tokenize(json);
        vector<Token<char>> expected{ { "'lo\"ng'" } };

        EXPECT_EQ(tokens, expected);
    }

    // String with escape characters
    {
        string_view json = "\"\\ba\\fb\\nc\\rd\\t\"";

        vector<Token<char>> tokens = tokenize(json);
        vector<Token<char>> expected{ { "\ba\fb\nc\rd\t" } };

        EXPECT_EQ(tokens, expected);
    }

    // String with solidus
    {
        string_view json = "\"\\\\ /\"";

        vector<Token<char>> tokens = tokenize(json);
        vector<Token<char>> expected{ { "\\ /" } };

        EXPECT_EQ(tokens, expected);
    }

    // String with unicode
    // A: 0x41
    // y: 0x79

    // utf8 string with hexes
    {
        string_view json = "\"\\u4179\"";

        vector<Token<char>> tokens = tokenize(json);
        vector<Token<char>> expected{ { "Ay" } };

        EXPECT_EQ(tokens, expected);
    }

    // utf16 string with hexes
    {
        basic_string_view<char16_t> json = u"\"\\u1189\"";

        vector<Token<char16_t>> tokens = tokenize(json);
        vector<Token<char16_t>> expected{ { u"ᆉ" } };

        EXPECT_EQ(tokens, expected);
    }

    // utf32 string with hexes
    {
        basic_string_view<char32_t> json = U"\"\\u1189\"";

        vector<Token<char32_t>> tokens = tokenize(json);
        vector<Token<char32_t>> expected{ { U"ᆉ" } };

        EXPECT_EQ(tokens, expected);
    }
}

TEST(TokenizerTest, Number)
{
    // positive integer
    {
        string_view json = "123";
        vector<Token<char>> tokens = tokenize(json);
        vector<Token<char>> expected{ { 123.0 } };

        ASSERT_GT(tokens.size(), size_t{ 0 });
        EXPECT_FLOAT_EQ(tokens[0].number(), expected[0].number());
    }

    // negative integer
    {
        string_view json = "-123";
        vector<Token<char>> tokens = tokenize(json);
        vector<Token<char>> expected{ { -123.0 } };

        ASSERT_GT(tokens.size(), size_t{ 0 });
        EXPECT_FLOAT_EQ(tokens[0].number(), expected[0].number());
    }
}