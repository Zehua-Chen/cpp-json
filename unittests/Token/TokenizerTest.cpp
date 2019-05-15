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
using Tokens = vector<Token<char>>;

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
    Tokens tokens = tokenize(json);
    Tokens expected{ { TType::beginObject },    { TType::endObject },
                     { TType::beginArray },     { TType::endArray },
                     { TType::valueSeparator }, { TType::keyValueSeparator } };

    EXPECT_EQ(tokens, expected);
}

TEST(TokenizerTest, String)
{
    // Just string
    {
        string_view json = "\"abc\"";

        Tokens tokens = tokenize(json);
        Tokens expected{
            { "abc" },
        };

        EXPECT_EQ(tokens, expected);
    }

    // String with single quotes
    {
        string_view json = "\"'lo\\\"ng'\"";

        Tokens tokens = tokenize(json);
        Tokens expected{ { "'lo\"ng'" } };

        EXPECT_EQ(tokens, expected);
    }

    // String with escape characters
    {
        string_view json = "\"\\ba\\fb\\nc\\rd\\t\"";

        Tokens tokens = tokenize(json);
        Tokens expected{ { "\ba\fb\nc\rd\t" } };

        EXPECT_EQ(tokens, expected);
    }

    // String with solidus
    {
        string_view json = "\"\\\\ /\"";

        Tokens tokens = tokenize(json);
        Tokens expected{ { "\\ /" } };

        EXPECT_EQ(tokens, expected);
    }

    // String with unicode
    // A: 0x41
    // y: 0x79

    // utf8 string with hexes
    {
        string_view json = "\"\\u4179\"";

        Tokens tokens = tokenize(json);
        Tokens expected{ { "Ay" } };

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
        Tokens tokens = tokenize(json);
        Tokens expected{ { 123.0 } };

        ASSERT_EQ(tokens.size(), expected.size());
        EXPECT_FLOAT_EQ(tokens[0].number(), expected[0].number());
    }

    // negative integer
    {
        string_view json = "-123";
        Tokens tokens = tokenize(json);
        Tokens expected{ { -123.0 } };

        ASSERT_EQ(tokens.size(), expected.size());
        EXPECT_FLOAT_EQ(tokens[0].number(), expected[0].number());
    }

    // float point number
    {
        string_view json = "-123.123";
        Tokens tokens = tokenize(json);
        Tokens expected{ { -123.123 } };

        ASSERT_EQ(tokens.size(), expected.size());
        EXPECT_FLOAT_EQ(tokens[0].number(), expected[0].number());
    }

    // float point number with scale
    {
        string_view json = "-123.1e-2";
        Tokens tokens = tokenize(json);
        Tokens expected{ { -123.1e-2 } };

        ASSERT_EQ(tokens.size(), expected.size());
        EXPECT_FLOAT_EQ(tokens[0].number(), expected[0].number());
    }

    // two numbers
    {
        string_view json = "-123e0,-123e-1 123e2";

        Tokens tokens = tokenize(json);
        Tokens expected{
            { -123.0e0 }, { TType::valueSeparator }, { -123.0e-1 }, { 123.0e2 }
        };

        ASSERT_EQ(tokens.size(), expected.size());

        for (size_t i = 0; i < tokens.size(); ++i)
        {
            const auto &actual = tokens[i];
            const auto &e = expected[i];

            if (actual.type == TType::number)
            {
                EXPECT_FLOAT_EQ(actual.number(), e.number());
            }
        }
    }
}

TEST(TokenizerTest, Bool)
{
    string_view json = "true false";

    Tokens tokens = tokenize(json);
    Tokens expected{
        { TType::boolean, true },
        { TType::boolean, false }
    };

    EXPECT_EQ(tokens, expected);
}

TEST(TokenizerTest, Null)
{
    string_view json = "null null";

    Tokens tokens = tokenize(json);
    Tokens expected{
        { TType::null },
        { TType::null }
    };

    EXPECT_EQ(tokens, expected);
}