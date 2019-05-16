//
//  BasicObjectTest.cpp
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

using Value = BasicValue<char>;
using VType = Value::Type;

TEST(BasicObjectTest, Construction)
{
    Value object{ VType::object };
    EXPECT_EQ(object.type(), BasicValue<char>::Type::object);
    EXPECT_TRUE(object.isObject());
}

TEST(BasicObjectTest, CopyConstruction)
{
    Value object{ VType::object };
    Value name{ "jackson" };
    
    object["name"] = name;
    
    BasicValue<char> copy = object;
    
    EXPECT_EQ(object["name"].string(), copy["name"].string());
}

TEST(BasicObjectTest, MoveConstruction)
{
    Value object{ VType::object };
    Value name{ "jackson" };

    object["name"] = name;
    
    BasicValue<char> moved = std::move(object);
    
    ASSERT_EQ(moved.type(), VType::object);
    EXPECT_EQ(moved["name"].string(), "jackson");
    EXPECT_EQ(object.size(), size_t(0));
}

TEST(BasicObjectTest, CopyAssignment)
{
    Value object{ VType::object };
    Value name{ "jackson" };
    
    object["name"] = name;
    
    Value copy{ VType::object };
    copy["random data"] = Value{};
    
    copy = object;
    
    ASSERT_EQ(object.type(), VType::object);
    ASSERT_EQ(copy.type(), VType::object);
    ASSERT_EQ(copy["name"].type(), VType::string);
    ASSERT_EQ(object["name"].type(), VType::string);

    EXPECT_EQ(object["name"].string(), copy["name"].string());
}

TEST(BasicObjectTest, MoveAssignment)
{
    Value object{ VType::object };
    Value name{ "jackson" };
    
    name.string("jackson");
    object["name"] = name;
    
    Value moved{ VType::object };
    moved["random data"] = Value{};
    moved = std::move(object);
    
    EXPECT_EQ(moved["name"].string(), "jackson");
    EXPECT_EQ(object.size(), size_t(0));
}

TEST(BasicObjectTest, ReadWriteWithMethods)
{
    Value object{ VType::object };
    Value name{ "jackson" };

    object.set("name", name);

    ASSERT_TRUE(object.size() == 1);
    ASSERT_TRUE(object.contains("name"));

    EXPECT_EQ(object.get("name").string(), name.string());
}

TEST(BasicObjectTest, ReadWriteWithSubscripts)
{
    Value object{ VType::object };
    Value name{ "jackson" };

    object["name"] = name;

    ASSERT_TRUE(object.size() == 1);
    ASSERT_TRUE(object.contains("name"));

    EXPECT_EQ(object.get("name").string(), name.string());

    Value newName{ "peter" };
    object["name"] = newName;

    ASSERT_TRUE(object.size() == 1);
    ASSERT_TRUE(object.contains("name"));

    EXPECT_EQ(object.get("name").string(), newName.string());
}

TEST(BasicObjectTest, Erase)
{
    Value object{ VType::object };
    Value name{ "jackson" };
    
    object["name"] = name;
    
    ASSERT_TRUE(object.contains("name"));
    
    object.erase("name");
    
    ASSERT_FALSE(object.contains("name"));
}