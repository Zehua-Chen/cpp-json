#include "Json/Json.hpp"
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
}

TEST(BasicObjectTest, ReadWrite)
{
    auto object = makeObject();
    auto name = makePrimitive();
    
    name.string("jackson");
    object.set("jackson", name);
    
    EXPECT_EQ(object.get("jackson").string(), name.string());
}