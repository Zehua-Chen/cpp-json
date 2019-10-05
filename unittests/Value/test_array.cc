//
//  BasicArrayTest.cpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#include "json/value/basic_value.h"
#include "gtest/gtest.h"
#include <string>

using std::string;
using std::string_view;

using Value = json::BasicValue<char>;
using VType = Value::Type;

TEST(BasicArrayTest, Construction)
{
    Value array{ VType::array };
    EXPECT_EQ(array.type(), VType::array);
    EXPECT_TRUE(array.isArray());
}

TEST(BasicArrayTest, CopyConstruction)
{
    Value array{ VType::array };
    Value element{ "element 1" };

    array.append(element);

    Value copy = array;

    EXPECT_EQ(copy[0].string(), array[0].string());
    EXPECT_EQ(copy.size(), array.size());
}

TEST(BasicArrayTest, MoveConstruction)
{
    Value array{ VType::array };
    Value element{ "element 1" };

    array.append(element);

    Value moved = std::move(array);

    EXPECT_EQ(moved[0].string(), "element 1");
    EXPECT_EQ(array.size(), size_t(0));
}

TEST(BasicArrayTest, CopyAssignment)
{
    Value array{ VType::array };
    Value element{ "element 1" };

    array.append(element);

    Value copy{ VType::array };
    copy.append(Value{});
    copy = array;

    EXPECT_EQ(copy[0].string(), array[0].string());
    EXPECT_EQ(copy.size(), array.size());
}

TEST(BasicArrayTest, MoveAssignment)
{
    Value array{ VType::array };
    Value element{ "element 1" };

    array.append(element);

    Value moved{ VType::array };
    moved.append(Value{});
    moved = std::move(array);

    EXPECT_EQ(moved[0].string(), "element 1");
    EXPECT_EQ(array.size(), size_t(0));
}

TEST(BasicArrayTest, ReadWrite)
{
    Value array{ VType::array };

    Value element1{ "element 1" };
    Value element2{ "element 2" };

    array.append(element1);
    array.append(element2);

    ASSERT_TRUE(array.size() == 2);
    EXPECT_EQ(array[0].string(), element1.string());
    EXPECT_EQ(array[1].string(), element2.string());

    Value newElement{ "new" };
    array[1] = newElement;

    EXPECT_EQ(array[1].string(), newElement.string());
}

TEST(BasicArrayTest, Erase)
{
    Value array{ VType::array };

    Value element1{ "element 1" };
    Value element2{ "element 2" };

    array.append(element1);
    array.append(element2);

    ASSERT_EQ(array.size(), size_t{ 2 });

    array.erase(1);

    ASSERT_EQ(array.size(), size_t{ 1 });
    EXPECT_EQ(array[0].string(), "element 1");
}