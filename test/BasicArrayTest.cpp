//
//  BasicArrayTest.cpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#include "Json/BasicValue.hpp"
#include "gtest/gtest.h"
#include <string>

using std::string;
using std::string_view;

using namespace json;

TEST(BasicArrayTest, Construction)
{
    auto array = makeArray();
    EXPECT_EQ(array.type(), Type::array);
    EXPECT_TRUE(array.isArray());
}

TEST(BasicArrayTest, ReadWrite)
{
    auto array = makeArray();

    auto element1 = makePrimitive();
    element1.string("element 1");
    auto element2 = makePrimitive();
    element2.string("element 2");

    array.append(element1);
    array.append(element2);

    ASSERT_TRUE(array.size() == 2);
    EXPECT_EQ(array[0].string(), element1.string());
    EXPECT_EQ(array[1].string(), element2.string());

    auto newElement = makePrimitive();
    newElement.string("new");
    array[1] = newElement;

    EXPECT_EQ(array[1].string(), newElement.string());
}