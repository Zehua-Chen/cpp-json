//
//  BasicPrimitiveTest.cpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#include "json/Value/BasicValue.hpp"
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
    Value value;
    value.string("a");

    EXPECT_EQ(value.type(), Value::Type::string);
    EXPECT_EQ(value.string(), "a");
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