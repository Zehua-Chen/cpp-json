#include "gtest/gtest.h"
#include "Json/Json.hpp"
#include <string>

using std::string;
using std::string_view;

using JsonObject = json::Object;
using JsonPrimitive = json::Primitive;

TEST(BasicObjectTest, ReadWrite)
{
    JsonObject object;
    
    object.set("name", new JsonPrimitive("Philosophor"));
    
    // Test conversion using get
    {
        JsonPrimitive *name = static_cast<JsonPrimitive *>(object.get("name"));
        auto &nameStr = name->value();
        EXPECT_EQ(nameStr, "Philosophor");
    }
    
    // Test conversion using subscript
    {
        {
            JsonPrimitive *name = static_cast<JsonPrimitive *>(object["name"]);
            auto &nameStr = name->value();
            EXPECT_EQ(nameStr, "Philosophor");
        }
        
        {
            string property = "name";
            
            JsonPrimitive *name 
                = static_cast<JsonPrimitive *>(object[property]);
            auto &nameStr = name->value();
            EXPECT_EQ(nameStr, "Philosophor");
        }
    }
}

TEST(BasicObjectTest, BooleanConversion)
{
    JsonObject object;
    EXPECT_FALSE(static_cast<bool>(object));
    
    if (object)
    {
        FAIL();
    }
    
    object.set("name", new JsonPrimitive("Philosophor"));
    EXPECT_TRUE(static_cast<bool>(object));
    
    if (!object)
    {
        FAIL();
    }
}