#include "gtest/gtest.h"
#include "json/json.hpp"
#include <string_view>
#include <fstream>
#include <iterator>

using std::string_view;
using namespace json;

TEST(ParserTest, Primitive)
{
    string_view json = "123";
    Value value = json::parse(json.begin(), json.end());

    ASSERT_EQ(value.type(), Value::Type::number);
    EXPECT_FLOAT_EQ(value.number(), 123.0);
}

TEST(ParserTest, Object)
{
    string_view json
        = "{ \"a\": \"b\", \"c\": \"d\", \"e\": { \"f\": \"g\" } }";
    Value value = json::parse(json.begin(), json.end());

    ASSERT_EQ(value.type(), Value::Type::object);

    ASSERT_TRUE(value.contains("a"));
    EXPECT_EQ(value["a"].string(), "b");

    ASSERT_TRUE(value.contains("c"));
    EXPECT_EQ(value["c"].string(), "d");

    ASSERT_TRUE(value.contains("e"));
    const Value &nested = value["e"];
    
    ASSERT_TRUE(nested.contains("f"));
    EXPECT_EQ(nested["f"].string(), "g");
}

TEST(ParserTest, Array) 
{
    string_view json
        = "[true, [123, 223], false]";
    Value value = json::parse(json.begin(), json.end());

    ASSERT_EQ(value.type(), Value::Type::array);
    ASSERT_EQ(value.size(), size_t{ 3 });

    EXPECT_EQ(value[0].boolean(), true);
    EXPECT_EQ(value[2].boolean(), false);

    Value &nested = value[1];

    ASSERT_EQ(nested.type(), Value::Type::array);
    EXPECT_FLOAT_EQ(nested[0].number(), 123.0);
    EXPECT_FLOAT_EQ(nested[1].number(), 223.0);
}

TEST(ParserTest, File) 
{
    std::ifstream file{ PATH_TESTFILE };

    ASSERT_TRUE(file.is_open());

    std::istreambuf_iterator<char> begin{ file };
    std::istreambuf_iterator<char> end;

    Value value = json::parse(begin, end);
    
    ASSERT_EQ(value.type(), Value::Type::array);

    const Value &last = value[value.size() - 1];
    ASSERT_EQ(last.type(), Value::Type::object);

    ASSERT_TRUE(last.contains("Attack"));
    EXPECT_FLOAT_EQ(last["Attack"].number(), 5);

    ASSERT_TRUE(last.contains("Type"));
    EXPECT_EQ(last["Type"].string(), "Monster");

    ASSERT_TRUE(last.contains("Death Effects"));
    const Value &deathEffect = last["Death Effects"];

    ASSERT_TRUE(deathEffect.contains("Rock"));
    EXPECT_FLOAT_EQ(deathEffect["Rock"].number(), 3);
}