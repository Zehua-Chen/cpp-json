//
//  BasicPrimitiveTest.cpp
//  CPP
//
//  Created by Zehua Chen on 2/14/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#include "json/value/basic_value.h"
#include "gtest/gtest.h"
#include <iostream>
#include <string>

using std::string;
using std::string_view;

using std::cout;
using std::endl;

using json::BasicValue;
using Value = json::BasicValue<char>;

TEST(ValueTest, Null)
{
    Value value;
    EXPECT_EQ(value.type(), Value::Type::null);
}