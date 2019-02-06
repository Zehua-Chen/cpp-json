//
//  BasicArrayTest.cpp
//  CPP
//
//  Created by Zehua Chen on 2/6/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#include "gtest/gtest.h"
#include "Json/Parse.hpp"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

using namespace json;

class ParseTest: public testing::Test 
{
    void SetUp() override
    {
        string text = "{}";
        auto object = parse(text.begin(), text.end());
    }
};

TEST_F(ParseTest, Contains)
{
    
}