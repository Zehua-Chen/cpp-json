#include "gtest/gtest.h"
#include "json/utils/convert.h"

using namespace json::utils::convert;

TEST(ConvertTest, FromHex)
{
    // utf8
    {
        int8_t value = number::fromHex<int8_t>('0');
        EXPECT_EQ(value, 0);

        value = number::fromHex<int8_t>('8');
        EXPECT_EQ(value, 8);

        value = number::fromHex<int8_t>('f');
        EXPECT_EQ(value, 15);

        value = number::fromHex<int8_t>('F');
        EXPECT_EQ(value, 15);
    }

    // utf16
    {
        auto value = number::fromHex<int8_t>(u'0');
        EXPECT_EQ(value, 0);

        value = number::fromHex<int8_t>(u'8');
        EXPECT_EQ(value, 8);

        value = number::fromHex<int8_t>(u'f');
        EXPECT_EQ(value, 15);

        value = number::fromHex<int8_t>(u'F');
        EXPECT_EQ(value, 15);
    }

    // utf32
    {
        auto value = number::fromHex<int8_t>(U'0');
        EXPECT_EQ(value, 0);

        value = number::fromHex<int8_t>(U'8');
        EXPECT_EQ(value, 8);

        value = number::fromHex<int8_t>(U'f');
        EXPECT_EQ(value, 15);

        value = number::fromHex<int8_t>(U'F');
        EXPECT_EQ(value, 15);
    }
}

TEST(ConverTest, FromDec)
{
    // utf8
    {
        auto value = number::fromDec<int>('3');
        EXPECT_EQ(value, 3);
    }

    // utf16
    {
        auto value = number::fromDec<int>(u'3');
        EXPECT_EQ(value, 3);
    }

    // utf32
    {
        auto value = number::fromDec<int>(U'3');
        EXPECT_EQ(value, 3);
    }
}