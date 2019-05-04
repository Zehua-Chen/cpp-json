#include "gtest/gtest.h"
#include "json/Utils/Convert.hpp"

using namespace json::utils::convert;

TEST(ConverTest, ToHex)
{
    // utf8
    {   
        int8_t value = integer::fromHex<int8_t>('0');
        EXPECT_EQ(value, 0);
        
        value = integer::fromHex<int8_t>('8');
        EXPECT_EQ(value, 8);
        
        value = integer::fromHex<int8_t>('f');
        EXPECT_EQ(value, 15);
        
        value = integer::fromHex<int8_t>('F');
        EXPECT_EQ(value, 15);
    }
    
    // utf16
    {
        auto value = integer::fromHex<int8_t>(u'0');
        EXPECT_EQ(value, 0);
        
        value = integer::fromHex<int8_t>(u'8');
        EXPECT_EQ(value, 8);
        
        value = integer::fromHex<int8_t>(u'f');
        EXPECT_EQ(value, 15);
        
        value = integer::fromHex<int8_t>(u'F');
        EXPECT_EQ(value, 15);
    }
    
    // utf32
    {
        auto value = integer::fromHex<int8_t>(U'0');
        EXPECT_EQ(value, 0);
        
        value = integer::fromHex<int8_t>(U'8');
        EXPECT_EQ(value, 8);
        
        value = integer::fromHex<int8_t>(U'f');
        EXPECT_EQ(value, 15);
        
        value = integer::fromHex<int8_t>(U'F');
        EXPECT_EQ(value, 15);
    }
}