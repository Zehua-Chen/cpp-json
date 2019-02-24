#include "gtest/gtest.h"
#include "Json/Json.hpp"
#include <string>
#include <sstream>

using std::u16string_view;
using std::u32string_view;
using std::wstring_view;

using namespace json;

TEST(EncodingTest, UTF16)
{
    u16string_view text = u"{ 'key': 'value', 'items': [ 'a' ] }";
    U16Value value = parse<char16_t>(text.begin(), text.end());
    
    ASSERT_EQ(value.type(), U16Value::Type::object);
    
    ASSERT_TRUE(value.contains(u"key"));
    EXPECT_EQ(value[u"key"].string(), u"value");
    
    ASSERT_TRUE(value.contains(u"items"));
    U16Value &array = value[u"items"];
    ASSERT_EQ(array.type(), U16Value::Type::array);
    ASSERT_EQ(array.size(), size_t{ 1 });
    EXPECT_EQ(array[0].string(), u"a");
}

TEST(EncodingTest, UTF32)
{
    u32string_view text = U"{ 'key': 'value', 'items': [ 'a' ] }";
    U32Value value = parse<char32_t>(text.begin(), text.end());
    
    ASSERT_EQ(value.type(), U32Value::Type::object);
    
    ASSERT_TRUE(value.contains(U"key"));
    EXPECT_EQ(value[U"key"].string(), U"value");
    
    ASSERT_TRUE(value.contains(U"items"));
    U32Value &array = value[U"items"];
    ASSERT_EQ(array.type(), U32Value::Type::array);
    ASSERT_EQ(array.size(), size_t{ 1 });
    EXPECT_EQ(array[0].string(), U"a");
}

TEST(EncodingTest, WChar)
{
    wstring_view text = L"{ 'key': 'value', 'items': [ 'a' ] }";
    WValue value = parse<wchar_t>(text.begin(), text.end());
    
    ASSERT_EQ(value.type(), WValue::Type::object);
    
    ASSERT_TRUE(value.contains(L"key"));
    EXPECT_EQ(value[L"key"].string(), L"value");
    
    ASSERT_TRUE(value.contains(L"items"));
    WValue &array = value[L"items"];
    ASSERT_EQ(array.type(), WValue::Type::array);
    ASSERT_EQ(array.size(), size_t{ 1 });
    EXPECT_EQ(array[0].string(), L"a");
}