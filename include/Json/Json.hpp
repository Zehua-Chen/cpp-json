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
#include "Json/Tokenizer.hpp"

namespace json 
{
    using Value = BasicValue<char>;
}