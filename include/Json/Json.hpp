#pragma once

#include "Json/BasicValue.hpp"
#include "Json/BasicObject.hpp"
#include "Json/Parse.hpp"

namespace json 
{
    using JsonValue = BasicValue<char>;
    using JsonObject = BasicObject<char>;
}