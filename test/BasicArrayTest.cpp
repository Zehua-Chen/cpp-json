#include "Json/Json.hpp"
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