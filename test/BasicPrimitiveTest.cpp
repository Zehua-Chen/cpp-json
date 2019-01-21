//
//  BasicPrimitiveTest.cpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#include "Json/BasicValue.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <string>

using std::string;
using std::string_view;

using std::cout;
using std::endl;

using namespace json;

TEST(BasicPrimitiveTest, Construction)
{
    auto primitive = makePrimitive();
    EXPECT_EQ(primitive.type(), Type::primitive);
    EXPECT_TRUE(primitive.isPrimitive());
}

TEST(BasicPrimitiveTest, ReadWrite)
{
    auto primitive = makePrimitive();
    primitive.string("aaa");

    ASSERT_TRUE(primitive.size() == 3);
    EXPECT_EQ(primitive.string(), "aaa");

    auto &value = primitive.string();
    value = "a";

    ASSERT_TRUE(primitive.size() == 1);
    EXPECT_EQ(primitive.string(), "a");
}