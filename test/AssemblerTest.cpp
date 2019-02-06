//
//  BasicArrayTest.cpp
//  CPP
//
//  Created by Zehua Chen on 1/31/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#include "gtest/gtest.h"
#include "Json/Assembler.hpp"
#include <vector>
#include <utility>

using std::vector;
using std::cout;
using std::endl;
using std::move;

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
            assembler.takeToken(token);
        }
        
        auto object = move(assembler.root());
        
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
        
        BasicValue<char> array = move(assembler.root());
        
        ASSERT_EQ(array.type(), VType::array);
        ASSERT_EQ(array.size(), size_t{ 2 });
        
        EXPECT_EQ(array[0].string(), "billy");
        EXPECT_EQ(array[1].string(), "12");
    }
    
    // Simple primitive
    {
        Token<char> token{ TType::value, "value" };
        Assembler<char> assembler;
        
        assembler.takeToken(token);
        
        BasicValue<char> primitive = move(assembler.root());
        
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
            assembler.takeToken(token);
        }
        
        // Test root
        BasicValue<char> object = move(assembler.root());
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
    
    // Nested array
    {
        vector<Token<char>> tokens {
            // root array
            { TType::beginArray },
            // nested array 1
            { TType::beginArray },
            { TType::value, "a" },
            { TType::value, "b" },
            { TType::value, "c" },
            { TType::endArray },
            // nested array 1
            { TType::beginArray },
            // item 1
            { TType::value, "d" },
            // item 2
            { TType::beginObject },
            { TType::key, "data" },
            { TType::value, "12" },
            { TType::endObject },
            // item 3
            { TType::value, "e" },
            { TType::endArray },
            // end root array
            { TType::endArray },
        };
        
        Assembler<char> assembler;
        
        for (const auto &token: tokens)
        {
            assembler.takeToken(token);
        }
        
        BasicValue<char> root = move(assembler.root());
        ASSERT_EQ(root.type(), VType::array);
        ASSERT_EQ(root.size(), size_t{ 2 });
        
        BasicValue<char> &nestedArray1 = root[0];
        ASSERT_EQ(nestedArray1.type(), VType::array);
        ASSERT_EQ(nestedArray1.size(), size_t{ 3 });
        
        EXPECT_EQ(nestedArray1[0].string(), "a");
        EXPECT_EQ(nestedArray1[1].string(), "b");
        EXPECT_EQ(nestedArray1[2].string(), "c");
        
        BasicValue<char> &nestedArray2 = root[1];
        ASSERT_EQ(nestedArray2.type(), VType::array);
        ASSERT_EQ(nestedArray2.size(), size_t{ 3 });
        
        EXPECT_EQ(nestedArray2[0].string(), "d");
        EXPECT_EQ(nestedArray2[2].string(), "e");
        
        BasicValue<char> &object = nestedArray2[1];
        ASSERT_EQ(object.type(), VType::object);
        ASSERT_TRUE(object.contains("data"));
        EXPECT_EQ(object["data"].string(), "12");
    }
}