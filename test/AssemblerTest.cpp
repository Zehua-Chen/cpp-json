#include "gtest/gtest.h"
#include "Json/Assembler.hpp"
#include <vector>

using std::vector;
using std::cout;
using std::endl;

using namespace std::string_literals;

using namespace json;
using namespace json::token;
using namespace json::assemble;

// Alias TType stands for Token Type and is used to reduce column width
using TType = Token<char>::Type;
using VType = BasicValue<char>::Type;

TEST(AssemblerTest, Simple)
{
    // Simple object
    {
        vector<Token<char>> tokens{
            { TType::beginObject },
            { TType::key, "name" },
            { TType::value, "billy" },
            { TType::key, "age" },
            { TType::value, "12" },
            { TType::endObject },
        };
        
        Assembler<char> assembler;
        
        for (const auto &token: tokens)
        {
            assembler(token);
        }
        
        auto object = assembler.root();
        
        ASSERT_EQ(VType::object, object.type());
        
        ASSERT_TRUE(object.contains("name"));
        EXPECT_EQ(object.get("name").string(), "billy"s);
        
        ASSERT_TRUE(object.contains("age"));
        EXPECT_EQ(object.get("age").string(), "12"s);
    }
}