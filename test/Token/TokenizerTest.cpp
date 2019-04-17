#include "json/Token/Tokenizer.hpp"
#include "gtest/gtest.h"
#include <string_view>
#include <vector>

using std::string_view;
using std::vector;

using json::token::Token;
using json::token::Tokenizer;

using TType = Token<char>::Type;

static vector<Token<char>> tokenize(string_view js)
{
    vector<Token<char>> output;
    Tokenizer<char, string_view::iterator> tokenizer{ js.begin(), js.end() };

    while (tokenizer)
    {
        tokenizer.extract();
        output.push_back(tokenizer.token());
    }

    return output;
}

TEST(TokenizerTest, NonContextual)
{
    string_view json = "{}[],:";
    vector<Token<char>> tokens = tokenize(json);
    vector<Token<char>> expected{
        { TType::beginObject },    { TType::endObject },
        { TType::beginArray },     { TType::endArray },
        { TType::valueSeparator }, { TType::keyValueSeparator }
    };

    EXPECT_EQ(tokens, expected);
}