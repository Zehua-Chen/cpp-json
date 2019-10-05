//
//  BasicPrimitiveTest.cpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#include "json/value/basic_value.h"
#include "gtest/gtest.h"
#include <iostream>
#include <string>

using std::string;
using std::string_view;

using std::cout;
using std::endl;

using json::BasicValue;
using Value = json::BasicValue<char>;

TEST(PrimitiveTest, String)
{
    // Initialize using literal
    {
        Value value;
        value.string("a");

        EXPECT_EQ(value.type(), Value::Type::string);
        EXPECT_EQ(value.string(), "a");
    }

    // Initialize using STL string
    {
        string str = "a";
        Value value;
        value.string(str);

        EXPECT_EQ(value.type(), Value::Type::string);
        EXPECT_EQ(value.string(), "a");
    }
}

TEST(PrimitiveTest, Number)
{
    Value value;
    value.number(22);

    EXPECT_EQ(value.type(), Value::Type::number);
    EXPECT_FLOAT_EQ(value.number(), 22);
}

TEST(PrimitiveTest, Bool)
{
    Value value;
    value.boolean(true);

    EXPECT_EQ(value.type(), Value::Type::boolean);
    EXPECT_EQ(value.boolean(), true);
}