//
//  Json.hpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#pragma once

#include "Json/Assembler.hpp"
#include "Json/BasicValue.hpp"
#include "Json/Keywords.hpp"
#include "Json/Parse.hpp"
#include "Json/Token.hpp"

namespace json 
{
    using Value = BasicValue<char>;
    using U16Value = BasicValue<char16_t>;
    using U32Value = BasicValue<char32_t>;
    using WValue = BasicValue<wchar_t>;
}