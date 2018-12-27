#pragma once

#include "Json/BasicObject.hpp"
#include "Json/BasicPrimitive.hpp"
#include "Json/BasicValue.hpp"
#include "Json/Parse.hpp"

namespace json
{
using Value = BasicValue<char>;
using Object = BasicObject<char>;
using Primitive = BasicPrimitive<char>;
} // namespace json