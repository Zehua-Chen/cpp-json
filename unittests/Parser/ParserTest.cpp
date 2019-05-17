#include "gtest/gtest.h"
#include "json/json.hpp"
#include <string_view>

using std::string_view;
using namespace json;

TEST(ComposerTest, Object)
{
    string_view json
        = "{ \"a\": \"b\", \"c\": \"d\", \"e\": { \"f\": \"g\" } }";
    Value value = json::parse(json.begin(), json.end());

    ASSERT_TRUE(value.contains("a"));
    EXPECT_EQ(value["a"].string(), "b");

    ASSERT_TRUE(value.contains("c"));
    EXPECT_EQ(value["c"].string(), "d");
}

TEST(ComposerTest, Array) {}

TEST(ComposerTest, Mixed) {}