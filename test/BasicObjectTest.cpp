#include "Json/BasicValue.hpp"
#include "gtest/gtest.h"
#include <string>
#include <iostream>

using std::string;
using std::string_view;

using std::cout;
using std::endl;

using namespace json;

TEST(BasicObjectTest, Construction)
{
    auto object = makeObject();
    EXPECT_EQ(object.type(), Type::object);
    EXPECT_TRUE(object.isObject());
}

TEST(BasicObjectTest, ReadWriteWithMethods)
{
    auto object = makeObject();
    auto name = makePrimitive();
    
    name.string("jackson");
    object.set("name", name);
    
    ASSERT_TRUE(object.size() == 1);
    EXPECT_EQ(object.get("name").string(), name.string());
}

TEST(BasicObjectTest, ReadWriteWithSubscripts)
{
    auto object = makeObject();
    auto name = makePrimitive();
    
    name.string("jackson");
    object["name"] = name;
    
    ASSERT_TRUE(object.size() == 1);
    EXPECT_EQ(object.get("name").string(), name.string());
    
    auto newName = makePrimitive();
    name.string("peter");
    object["name"] = newName;
    
    ASSERT_TRUE(object.size() == 1);
    EXPECT_EQ(object.get("name").string(), newName.string());
}