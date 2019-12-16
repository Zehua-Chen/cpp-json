#include <fstream>
#include <iterator>
#include <string_view>
#include "gtest/gtest.h"
#include "json/json.h"

using std::string_view;
using namespace json;

TEST(ParserTest, Primitive) {
  string_view json = "123";
  Value value = json::parse(json);

  ASSERT_EQ(value.type(), Value::Type::kNumber);
  EXPECT_FLOAT_EQ(value.number(), 123.0);
}

TEST(ParserTest, Object) {
  string_view json = "{ \"a\": \"b\", \"c\": \"d\", \"e\": { \"f\": \"g\" } }";
  Value value = json::parse(json);

  ASSERT_EQ(value.type(), Value::Type::kObject);

  ASSERT_TRUE(value.Contains("a"));
  EXPECT_EQ(value["a"].string(), "b");

  ASSERT_TRUE(value.Contains("c"));
  EXPECT_EQ(value["c"].string(), "d");

  ASSERT_TRUE(value.Contains("e"));
  const Value &nested = value["e"];

  ASSERT_TRUE(nested.Contains("f"));
  EXPECT_EQ(nested["f"].string(), "g");
}

TEST(ParserTest, Array) {
  string_view json = "[true, [123, 223], false]";
  Value value = json::parse(json);

  ASSERT_EQ(value.type(), Value::Type::kArray);
  ASSERT_EQ(value.size(), size_t{3});

  EXPECT_EQ(value[0].boolean(), true);
  EXPECT_EQ(value[2].boolean(), false);

  Value &nested = value[1];

  ASSERT_EQ(nested.type(), Value::Type::kArray);
  EXPECT_FLOAT_EQ(nested[0].number(), 123.0);
  EXPECT_FLOAT_EQ(nested[1].number(), 223.0);
}

TEST(ParserTest, File) {
  std::ifstream file{"../unittests/resources/1.jsonc"};

  ASSERT_TRUE(file.is_open());

  Value value = json::parse(file);

  ASSERT_EQ(value.type(), Value::Type::kArray);

  const Value &last = value[value.size() - 1];
  ASSERT_EQ(last.type(), Value::Type::kObject);

  ASSERT_TRUE(last.Contains("Attack"));
  EXPECT_FLOAT_EQ(last["Attack"].number(), 5);

  ASSERT_TRUE(last.Contains("Type"));
  EXPECT_EQ(last["Type"].string(), "Monster");

  ASSERT_TRUE(last.Contains("Death Effects"));
  const Value &deathEffect = last["Death Effects"];

  ASSERT_TRUE(deathEffect.Contains("Rock"));
  EXPECT_FLOAT_EQ(deathEffect["Rock"].number(), 3);
}