#include "Json/Json.hpp"
#include "gtest/gtest.h"
#include <string>

using std::string;
using std::string_view;

using namespace json;

TEST(BasicArrayTest, Construction)
{
    auto object = makeArray();
    EXPECT_EQ(object.type(), Type::array);
}