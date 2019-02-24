//
//  BasicObjectTest.cpp
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

TEST(BasicObjectTest, Construction)
{
    auto object = makeObject();
    EXPECT_EQ(object.type(), BasicValue<char>::Type::object);
    EXPECT_TRUE(object.isObject());
}

TEST(BasicObjectTest, CopyConstruction)
{
    auto object = makeObject();
    auto name = makePrimitive();
    
    name.string("jackson");
    object["name"] = name;
    
    BasicValue<char> copy = object;
    
    EXPECT_EQ(object["name"].string(), copy["name"].string());
}

TEST(BasicObjectTest, MoveConstruction)
{
    auto object = makeObject();
    auto name = makePrimitive();
    
    name.string("jackson");
    object["name"] = name;
    
    BasicValue<char> moved = std::move(object);
    
    EXPECT_EQ(moved["name"].string(), "jackson");
    EXPECT_EQ(object.size(), size_t(0));
}

TEST(BasicObjectTest, CopyAssignment)
{
    auto object = makeObject();
    auto name = makePrimitive();
    
    name.string("jackson");
    object["name"] = name;
    
    auto copy = makeObject();
    copy["random data"] = makePrimitive();
    
    copy = object;
    
    EXPECT_EQ(object["name"].string(), copy["name"].string());
}

TEST(BasicObjectTest, MoveAssignment)
{
    auto object = makeObject();
    auto name = makePrimitive();
    
    name.string("jackson");
    object["name"] = name;
    
    auto moved = makeObject();
    moved["random data"] = makePrimitive();
    moved = std::move(object);
    
    EXPECT_EQ(moved["name"].string(), "jackson");
    EXPECT_EQ(object.size(), size_t(0));
}

TEST(BasicObjectTest, ReadWriteWithMethods)
{
    auto object = makeObject();
    auto name = makePrimitive();

    name.string("jackson");
    object.set("name", name);

    ASSERT_TRUE(object.size() == 1);
    ASSERT_TRUE(object.contains("name"));

    EXPECT_EQ(object.get("name").string(), name.string());
}

TEST(BasicObjectTest, ReadWriteWithSubscripts)
{
    auto object = makeObject();
    auto name = makePrimitive();

    name.string("jackson");
    object["name"] = name;

    ASSERT_TRUE(object.size() == 1);
    ASSERT_TRUE(object.contains("name"));

    EXPECT_EQ(object.get("name").string(), name.string());

    auto newName = makePrimitive();
    name.string("peter");
    object["name"] = newName;

    ASSERT_TRUE(object.size() == 1);
    ASSERT_TRUE(object.contains("name"));

    EXPECT_EQ(object.get("name").string(), newName.string());
}

TEST(BasicObjectTest, Erase)
{
    auto object = makeObject();
    
    auto name = makePrimitive();
    name.string("jackson");
    
    object["name"] = name;
    
    ASSERT_TRUE(object.contains("name"));
    
    object.erase("name");
    
    ASSERT_FALSE(object.contains("name"));
}