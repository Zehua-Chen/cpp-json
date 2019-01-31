//
//  TokenTest.cpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#include "Json/Token.hpp"
#include "gtest/gtest.h"
#include <string>

using std::string;
using namespace std::string_literals;

using namespace json::token;

TEST(TokenTest, CopyConstructor)
{
    Token<char> token1{ TokenType::value, "something" };
    Token<char> token2 = token1;

    EXPECT_EQ(token2.data, token1.data);
}

TEST(TokenTest, MoveConstructor)
{
    Token<char> token1{ TokenType::value, "something" };
    Token<char> token2 = std::move(token1);

    EXPECT_TRUE(token1.data.empty());
    EXPECT_EQ(token2.data, "something"s);
}

TEST(TokenTest, Same)
{
    // Begin Object
    {
        Token<char> token1 = { TokenType::beginObject };
        Token<char> token2 = { TokenType::beginObject };

        EXPECT_EQ(token1, token2);
    }

    // Begin Array
    {
        Token<char> token1 = { TokenType::beginArray };
        Token<char> token2 = { TokenType::beginArray };

        EXPECT_EQ(token1, token2);
    }

    // Keys
    {
        Token<char> token1 = { TokenType::key };
        Token<char> token2 = { TokenType::key };

        EXPECT_EQ(token1, token2);
    }

    // Values
    {
        Token<char> token1 = { TokenType::value };
        Token<char> token2 = { TokenType::value };

        EXPECT_EQ(token1, token2);
    }
}

TEST(TokenTest, Different)
{
    // Object
    {
        Token<char> token1 = { TokenType::beginObject };
        Token<char> token2 = { TokenType::endObject };

        EXPECT_NE(token1, token2);
    }

    // Array
    {
        Token<char> token1 = { TokenType::beginArray };
        Token<char> token2 = { TokenType::endArray };

        EXPECT_NE(token1, token2);
    }

    // Keys and Value
    {
        Token<char> token1 = { TokenType::key };
        Token<char> token2 = { TokenType::value };

        EXPECT_NE(token1, token2);
    }

    // Different Buffer - Key
    {
        Token<char> token1 = { TokenType::key, "a" };
        Token<char> token2 = { TokenType::key, "b" };

        EXPECT_NE(token1, token2);
    }

    // Different Buffer - Value
    {
        Token<char> token1 = { TokenType::value, "a" };
        Token<char> token2 = { TokenType::value, "b" };

        EXPECT_NE(token1, token2);
    }
}