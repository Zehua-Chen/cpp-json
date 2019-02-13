//
//  BasicValue.hpp
//  CPP
//
//  Created by Zehua Chen on 1/31/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#pragma once

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
    void takeToken(const json::token::Token<CharT> &token);

    BasicValue<CharT> &root();
    const BasicValue<CharT> &root() const;

private:
    /**
     * _V stands for Value, "_" prefix denotes private type
     */
    using _V = BasicValue<CharT>;

    /**
     * _VType stands for Value::Type, "_" prefix denotes private type
     */
    using _VType = typename BasicValue<CharT>::Type;

    /**
     * The name given to values, "_" prefix denotes private type
     */
    using _Name = std::basic_string<CharT>;

    struct _Scope
    {
        BasicValue<CharT> value;
        std::basic_string<CharT> name;

        _Scope(_VType type, std::basic_string<CharT> &&name);
        _Scope(BasicValue<CharT> &&value, std::basic_string<CharT> &&name);

        _Scope(_Scope &&other);

        bool isAnonymous() const;
    };

    void _finishTopItem();
    void _assignItemToTopOfScope(_V &&value, _Name *name = nullptr);

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
    takeToken(token);
}

template<typename CharT>
void Assembler<CharT>::takeToken(const json::token::Token<CharT> &token)
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
            _finishTopItem();
        }

        break;
    }
    case TType::endArray:
    {
        // Cannot touch the last one, needs it to return the root
        if (_stack.size() > size_t{ 1 })
        {
            _finishTopItem();
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
            BasicValue<CharT> primtive = makePrimitive<CharT>();
            primtive.string(token.data);

            _stack.emplace(std::move(primtive), std::move(_key));
        }
        else
        {
            BasicValue<CharT> primitive = makePrimitive<CharT>();
            primitive.string(token.data);
            
            _assignItemToTopOfScope(std::move(primitive), &_key);
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
json::BasicValue<CharT> &Assembler<CharT>::root()
{
    return _stack.top().value;
}

template<typename CharT>
const json::BasicValue<CharT> &Assembler<CharT>::root() const
{
    return _stack.top().value;
}

template<typename CharT>
void Assembler<CharT>::_finishTopItem()
{
    _Scope current = std::move(_stack.top());
    _stack.pop();

    _assignItemToTopOfScope(std::move(current.value), &current.name);
}

template<typename CharT>
void Assembler<CharT>::_assignItemToTopOfScope(_V &&value, _Name *name)
{
    _Scope &top = _stack.top();
    _V &topValue = top.value;
    
    switch (topValue.type())
    {
    case _VType::object:
        topValue[*name] = std::move(value);
        break;
    case _VType::array:
        topValue.append(std::move(value));
        break;
    default:
        break;
    }
}
} // namespace json::assemble