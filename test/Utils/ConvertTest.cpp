#include "gtest/gtest.h"
#include "json/Utils/Convert.hpp"

using json::utils::convert::toHex;

TEST(ConverTest, ToHex)
{
    // utf8
    {
        auto value = toHex('0');
        EXPECT_EQ(value, 0);
        
        value = toHex('8');
        EXPECT_EQ(value, 8);
        
        value = toHex('f');
        EXPECT_EQ(value, 15);
        
        value = toHex('F');
        EXPECT_EQ(value, 15);
    }
    
    // utf16
    {
        auto value = toHex(u'0');
        EXPECT_EQ(value, 0);
        
        value = toHex(u'8');
        EXPECT_EQ(value, 8);
        
        value = toHex(u'f');
        EXPECT_EQ(value, 15);
        
        value = toHex(u'F');
        EXPECT_EQ(value, 15);
    }
    
    // utf32
    {
        auto value = toHex(U'0');
        EXPECT_EQ(value, 0);
        
        value = toHex(U'8');
        EXPECT_EQ(value, 8);
        
        value = toHex(U'f');
        EXPECT_EQ(value, 15);
        
        value = toHex(U'F');
        EXPECT_EQ(value, 15);
    }
}