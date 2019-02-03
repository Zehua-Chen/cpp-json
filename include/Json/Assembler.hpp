//
//  BasicValue.hpp
//  CPP
//
//  Created by Zehua Chen on 1/31/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#include "Json/BasicValue.hpp"
#include "Json/Token.hpp"
#include <iostream>
#include <stack>
#include <utility>

// Delcarations

namespace json::assemble
{
template<typename CharT>
class Assembler
{
public:
    void operator()(const json::token::Token<CharT> &token);
    BasicValue<CharT> &&root();

private:
    /**
     * _VType stands for Value Type, "_" prefix denotes private type
     */
    using _VType = typename BasicValue<CharT>::Type;

    struct _Scope
    {
        BasicValue<CharT> value;
        std::basic_string<CharT> name;

        _Scope(_VType type, std::basic_string<CharT> &&name);
        _Scope(BasicValue<CharT> &&value, std::basic_string<CharT> &&name);
        
        _Scope(_Scope &&other);

        bool isAnonymous() const;
    };

    std::basic_string<CharT> _key;
    std::stack<_Scope> _stack;
};
} // namespace json::assemble

// Implementations

namespace json::assemble
{
template<typename CharT>
Assembler<CharT>::_Scope::_Scope(_VType type, std::basic_string<CharT> &&name)
    : value(type)
    , name(std::move(name))
{
}

template<typename CharT>
Assembler<CharT>::_Scope::_Scope(
    BasicValue<CharT> &&value, std::basic_string<CharT> &&name)
    : value(std::move(value))
    , name(std::move(name))
{
}


template<typename CharT>
Assembler<CharT>::_Scope::_Scope(_Scope &&other)
    : value(std::move(other.value))
    , name(std::move(other.name))
{
}

template<typename CharT>
bool Assembler<CharT>::_Scope::isAnonymous() const
{
    return name.empty();
}

// Assembler<CharT> Implementation

template<typename CharT>
void Assembler<CharT>::operator()(const json::token::Token<CharT> &token)
{
    using namespace json::token;

    // TType stands for Token Type
    using TType = typename Token<CharT>::Type;

    switch (token.type)
    {
    case TType::beginObject:
        _stack.emplace(_VType::object, std::move(_key));
        break;
    case TType::beginArray:
        _stack.emplace(_VType::array, std::move(_key));
        break;
    case TType::endObject:
    {
        // Cannot touch the last one, needs it to return the root
        if (_stack.size() > size_t{ 1 })
        {
            _Scope current = std::move(_stack.top());
            _stack.pop();
            
            _Scope &next = _stack.top();
            BasicValue<CharT> &nextValue = next.value;
            
            switch (nextValue.type())
            {
            case _VType::object:
                nextValue[current.name] = std::move(current.value);
                break;
            case _VType::array:
                nextValue.append(std::move(current.value));
                break;
            default:
                break;
            }
        }

        break;
    }
    case TType::endArray:
    {
        // Cannot touch the last one, needs it to return the root
        if (_stack.size() > size_t{ 1 })
        {
        }

        break;
    }
    case TType::key:
        _key = token.data;
        break;
    case TType::value:
    {
        // If stack is empty, and we have an array, we have a single primitive
        if (_stack.empty())
        {
            BasicValue<char> primtive = makePrimitive();
            primtive.string(token.data);
            
            _stack.emplace(std::move(primtive), std::move(_key));
        }
        else
        {
            _Scope &top = _stack.top();
            BasicValue<CharT> &topValue = top.value;

            switch (topValue.type())
            {
            case _VType::object:
            {
                // Assign values to corresponding pairs
                BasicValue<CharT> value = makePrimitive();
                value.string(token.data);

                topValue[_key] = std::move(value);
                _key.clear();
                break;
            }
            case _VType::array:
            {
                BasicValue<CharT> value = makePrimitive();
                value.string(token.data);

                topValue.append(std::move(value));
                break;
            }
            default:
                break;
            }
        }

        break;
    }
    case TType::comment:
        break;
    default:
        break;
    }
}

template<typename CharT>
json::BasicValue<CharT> &&Assembler<CharT>::root()
{
    return std::move(_stack.top().value);
}
} // namespace json::assemble