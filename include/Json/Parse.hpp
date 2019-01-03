#pragma once

#include "Json/BasicValue.hpp"

namespace json
{
template<typename Iter, typename CharT = char>
BasicValue<CharT> parse(Iter begin, Iter end);
} // namespace json