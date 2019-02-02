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
    
    // Simple array
    {
        vector<Token<char>> tokens{
            { TType::beginArray },
            { TType::value, "billy" },
            { TType::value, "12" },
            { TType::endArray },
        };
        
        Assembler<char> assembler;
        
        for (const auto &token: tokens)
        {
            assembler(token);
        }
        
        BasicValue<char> array = assembler.root();
        
        ASSERT_EQ(array.type(), VType::array);
        ASSERT_EQ(array.size(), size_t{ 2 });
        
        EXPECT_EQ(array[0].string(), "billy");
        EXPECT_EQ(array[1].string(), "12");
    }
    
    // Simple primitive
    {
        Token<char> token{ TType::value, "value" };
        Assembler<char> assembler;
        
        assembler(token);
        
        BasicValue<char> primitive = assembler.root();
        
        ASSERT_EQ(primitive.type(), VType::primitive);
        EXPECT_EQ(primitive.string(), "value");
    }
}

TEST(AssemblerTest, Nested)
{
    // Nested object
    {
        vector<Token<char>> tokens{
            // root object
            { TType::beginObject },
            { TType::key, "person" },
            // person object
            { TType::beginObject },
            { TType::key, "name" },
            // name object 
            { TType::beginObject },
            // first name = Billy
            { TType::key, "firstName" },
            { TType::value, "Billy" },
            // last name = Herrington
            { TType::key, "lastName" },
            { TType::value, "Herrington" },
            { TType::endObject },
            // end name object
            // age = 12
            { TType::key, "age" },
            { TType::value, "12" },
            // gender = male
            { TType::key, "gender" },
            { TType::value, "male" },
            { TType::endObject },
            // end person object
            { TType::endObject },
            // end root object
        };
        
        Assembler<char> assembler;
        
        for (const auto &token: tokens)
        {
            assembler(token);
        }
        
        // Test root
        BasicValue<char> object = assembler.root();
        ASSERT_EQ(object.type(), VType::object);
        
        // Test person
        ASSERT_TRUE(object.contains("person"));
        BasicValue<char> &person = object["person"];
        ASSERT_EQ(person.type(), VType::object);

        ASSERT_TRUE(person.contains("age"));
        ASSERT_TRUE(person.contains("gender"));
        EXPECT_EQ(person["age"].string(), "12");
        EXPECT_EQ(person["gender"].string(), "male");
        
        // Test name
        ASSERT_TRUE(person.contains("name"));
        const BasicValue<char> &name = person["name"];
        ASSERT_EQ(name.type(), VType::object);
        
        ASSERT_TRUE(name.contains("firstName"));
        ASSERT_TRUE(name.contains("lastName"));
        EXPECT_EQ(name["firstName"].string(), "Billy");
        EXPECT_EQ(name["lastName"].string(), "Herrington");
    }
}