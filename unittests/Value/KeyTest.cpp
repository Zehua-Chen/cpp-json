#include "gtest/gtest.h"
#include "json/Value/BasicKey.hpp"

using std::string;
using std::hash;
using namespace std::string_literals;

using Key = json::BasicKey<char>;

TEST(KeyTest, Construction)
{
    // See if the correct constructor is called
    {
        Key keyView{ "abc" };

        EXPECT_EQ(keyView.index(), 1);
        EXPECT_FALSE(keyView.hasOwnership());

        Key key{ "abc"s };

        EXPECT_EQ(key.index(), 0);
        EXPECT_TRUE(key.hasOwnership());
    }
    
    // See if the string_view's address matches
    {
        const char *addr = "bbb";
        Key keyView{ addr };

        EXPECT_EQ(std::get<std::string_view>(keyView).data(), addr);
    }
}

TEST(KeyTest, Hash)
{
    hash<Key> keyHasher;

    Key keyView{ "abc" };
    Key key{ "abc"s };

    EXPECT_EQ(keyHasher(keyView), keyHasher(key));

    string str = "abc";
    hash<string> stringHasher;

    EXPECT_EQ(stringHasher(str), keyHasher(key));
}