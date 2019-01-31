//
//  Type.hpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#pragma once

namespace json
{
/**
 * Type of the value
 */
enum class Type
{
    // Value is object
    object,
    // Value is array
    array,
    // Value is primitive
    primitive,
};
} // namespace json
