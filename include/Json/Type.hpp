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
