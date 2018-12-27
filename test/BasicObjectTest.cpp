#include "Json/Json.hpp"
#include "gtest/gtest.h"
#include <string>
#include <iostream>

using std::string;
using std::string_view;

using std::cout;
using std::endl;

using namespace json;

TEST(BasicObjectTest, ReadWrite)
{
    Value value = makeObject();
}