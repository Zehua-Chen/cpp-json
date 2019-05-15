//
//  BasicPrimitiveTest.cpp
//  CPP
//
//  Created by Zehua Chen on 2/14/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#include "json/Value/BasicValue.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <string>

using std::string;
using std::string_view;

using std::cout;
using std::endl;

using namespace json;

TEST(BasicValueTest, TypeChanging)
{
    BasicValue<char> value = makeObject();
    BasicValue<char> name = makePrimitive();
    name.string("Philosophor");
    
    value["name"] = name;
    
    ASSERT_EQ(value.type(), BasicValue<char>::Type::object);
    
    value.type(BasicValue<char>::Type::primitive);
    value.string("some text");
    
    ASSERT_EQ(value.type(), BasicValue<char>::Type::primitive);
    EXPECT_EQ(value.string(), "some text");
}