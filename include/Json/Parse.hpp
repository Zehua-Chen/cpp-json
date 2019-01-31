//
//  Parse.hpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#pragma once

#include "Json/BasicValue.hpp"

namespace json
{
template<typename Iter, typename CharT = char>
BasicValue<CharT> parse(Iter begin, Iter end);
} // namespace json