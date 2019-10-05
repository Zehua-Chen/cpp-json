#include "gtest/gtest.h"
#include "json/value/basic_key.h"
#include <unordered_map>

using std::hash;
using std::string;
using std::unordered_map;
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

        EXPECT_EQ(std::get<std::string_view>(keyView.data()).data(), addr);
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

TEST(KeyTest, Equality)
{
    Key keyView{ "abc" };
    Key key{ "abc"s };

    EXPECT_EQ(keyView, key);
}

TEST(KeyTest, UnorderedMap)
{
    unordered_map<Key, int> map;

    map["a"] = 1;
    map["b"s] = 2;

    EXPECT_EQ(map["a"], 1);
    EXPECT_EQ(map["a"s], 1);

    EXPECT_EQ(map["b"], 2);
    EXPECT_EQ(map["b"s], 2);
}
