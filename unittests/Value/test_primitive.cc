//
//  BasicPrimitiveTest.cpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "json/value/basic_value.h"

using std::string;
using std::string_view;

using std::cout;
using std::endl;

using json::BasicValue;
using Value = json::BasicValue<char>;

TEST(PrimitiveTest, String) {
  // Initialize using literal
  {
    Value value;
    value.set_string("a");

    EXPECT_EQ(value.type(), Value::Type::kString);
    EXPECT_EQ(value.string(), "a");
  }

  // Initialize using STL string
  {
    string str = "a";
    Value value;
    value.set_string(str);

    EXPECT_EQ(value.type(), Value::Type::kString);
    EXPECT_EQ(value.string(), "a");
  }
}

TEST(PrimitiveTest, Number) {
  Value value;
  value.set_number(22);

  EXPECT_EQ(value.type(), Value::Type::kNumber);
  EXPECT_FLOAT_EQ(value.number(), 22);
}

TEST(PrimitiveTest, Bool) {
  Value value;
  value.set_boolean(true);

  EXPECT_EQ(value.type(), Value::Type::kBoolean);
  EXPECT_EQ(value.boolean(), true);
}