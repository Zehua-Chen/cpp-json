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
#include <fstream>
#include <iterator>

using std::cout;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;

using std::string;

using namespace json;

class ParseTest: public testing::Test 
{
public:

    BasicValue<char> root;
    
    void SetUp() override
    {
        ifstream file{ "../test/resources/1.jsonc" };
        
        istreambuf_iterator<char> begin{ file };
        istreambuf_iterator<char> end;
        
        root = parse(begin, end);
    }
};

TEST_F(ParseTest, RootTest)
{
    EXPECT_EQ(root.type(), BasicValue<char>::Type::array);
    EXPECT_EQ(root.size(), size_t{ 20 });
}

TEST_F(ParseTest, FirstElement)
{
    const BasicValue<char> &last = root[0];
    
    ASSERT_TRUE(last.contains("Dead"));
    EXPECT_EQ(last["Dead"].string(), "false");
    
    ASSERT_TRUE(last.contains("Destination"));
    EXPECT_EQ(last["Destination"].string(), "0");
    
    ASSERT_TRUE(last.contains("Type"));
    EXPECT_EQ(last["Type"].string(), "Player");
    
    // Nested Item: Death Efects
    ASSERT_TRUE(last.contains("Death Effects"));
    const BasicValue<char> &deathEffect = last["Death Effects"];
    
    ASSERT_TRUE(deathEffect.contains("Speed"));
    EXPECT_EQ(deathEffect["Speed"].string(), "0");
}

TEST_F(ParseTest, LastElement)
{
    const BasicValue<char> &last = root[19];
    
    ASSERT_TRUE(last.contains("Attack"));
    EXPECT_EQ(last["Attack"].string(), "5");
    
    ASSERT_TRUE(last.contains("Base Health"));
    EXPECT_EQ(last["Base Health"].string(), "30");
    
    ASSERT_TRUE(last.contains("Type"));
    EXPECT_EQ(last["Type"].string(), "Monster");
    
    // Nested Item: Death Efects
    ASSERT_TRUE(last.contains("Death Effects"));
    const BasicValue<char> &deathEffect = last["Death Effects"];
    
    ASSERT_TRUE(deathEffect.contains("Speed"));
    EXPECT_EQ(deathEffect["Speed"].string(), "0");
}