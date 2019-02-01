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
    EXPECT_EQ(array.type(), BasicValue<char>::Type::array);
    EXPECT_TRUE(array.isArray());
}

TEST(BasicArrayTest, CopyConstruction)
{
    auto array = makeArray();
    auto element = makePrimitive();
    element.string("element 1");
    
    array.append(element);
    
    BasicValue<char> copy = array;
    
    EXPECT_EQ(copy[0].string(), array[0].string());
    EXPECT_EQ(copy.size(), array.size());
}

TEST(BasicArrayTest, MoveConstruction)
{
    auto array = makeArray();
    auto element = makePrimitive();
    element.string("element 1");
    
    array.append(element);
    
    BasicValue<char> moved = std::move(array);
    
    EXPECT_EQ(moved[0].string(), "element 1");
    EXPECT_EQ(array.size(), size_t(0));
}

TEST(BasicArrayTest, CopyAssignment)
{
    auto array = makeArray();
    auto element = makePrimitive();
    element.string("element 1");
    
    array.append(element);
    
    auto copy = makeArray();
    copy.append(makePrimitive());
    copy = array;
    
    EXPECT_EQ(copy[0].string(), array[0].string());
    EXPECT_EQ(copy.size(), array.size());
}

TEST(BasicArrayTest, MoveAssignment)
{
    auto array = makeArray();
    auto element = makePrimitive();
    element.string("element 1");
    
    array.append(element);
    
    auto moved = makeArray();
    moved.append(makePrimitive());
    moved = std::move(array);
    
    EXPECT_EQ(moved[0].string(), "element 1");
    EXPECT_EQ(array.size(), size_t(0));
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
