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
/**
 * Phase 2 of the parsing process, take tokens and assemble tokens into a
 * json value.
 */
template<typename CharT>
class Assembler
{
public:
    /**
     * Take a token and use the token to assemble the json value
     * @param token the token used to assemble.
     */
    void operator()(const json::token::Token<CharT> &token);

    /**
     * Take a token and use the token to assemble the json value
     * @param token the token used to assemble.
     */
    void takeToken(const json::token::Token<CharT> &token);

    /**
     * The root json value
     * @returns a reference to the root of the json value. If the the json
     * tree is empty, append a new json null primitive to the tree and returns
     * it.
     */
    BasicValue<CharT> &root();

    /**
     * The root json value
     * @returns a constant reference to the root of the json value. If the the
     * json tree is empty, append a new json null primitive to the tree and
     * returns it.
     */
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

        _Scope(_VType type);
        _Scope(_VType type, std::basic_string<CharT> &&name);
        _Scope(BasicValue<CharT> &&value, std::basic_string<CharT> &&name);

        _Scope(_Scope &&other);

        bool isAnonymous() const;
    };

    void _finishTopItem();
    void _embedValueInTopOfScopes(_V &&value, _Name &&name);
    void _embedScopeInTopOfScopes(_Scope &&scope);

    std::basic_string<CharT> _key;
    std::stack<_Scope> _stack;
};
} // namespace json::assemble

// Implementations

namespace json::assemble
{
    
template<typename CharT>
Assembler<CharT>::_Scope::_Scope(_VType type)
    : value(type)
{
}

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

            _embedValueInTopOfScopes(std::move(primitive), std::move(_key));
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
    if (_stack.empty())
    {
        _stack.emplace(_VType::primitive);
    }
    
    return _stack.top().value;
}

template<typename CharT>
const json::BasicValue<CharT> &Assembler<CharT>::root() const
{
    if (_stack.empty())
    {
        _stack.emplace(_VType::primitive);
    }
    
    return _stack.top().value;
}

template<typename CharT>
void Assembler<CharT>::_finishTopItem()
{
    _Scope current = std::move(_stack.top());
    _stack.pop();

    _embedScopeInTopOfScopes(std::move(current));
}

template<typename CharT>
void Assembler<CharT>::_embedValueInTopOfScopes(_V &&value, _Name &&name)
{
    _Scope &top = _stack.top();
    _V &topValue = top.value;

    switch (topValue.type())
    {
    case _VType::object:
        topValue[std::move(name)] = std::move(value);
        break;
    case _VType::array:
        topValue.append(std::move(value));
        break;
    default:
        break;
    }
}

template<typename CharT>
void Assembler<CharT>::_embedScopeInTopOfScopes(_Scope &&scope)
{
    _embedValueInTopOfScopes(std::move(scope.value), std::move(scope.name));
}
} // namespace json::assemble