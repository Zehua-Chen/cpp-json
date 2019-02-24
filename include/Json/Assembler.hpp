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
 *
 * The assembler operates through a stack of scopes.
 * - the top of the scope is considered to be the "current" scope.
 *   - inserting key-value pair is always done in the "current" scope;
 *   - when "current" scope has finishes contruction, the value will be
 *   - popped and inserted into the new current scope.
 * - If scope A precedes scope B in the stack, then A is said to be the child
 *   of B and B is said to be the parent of A.
 * - Due to the fact that values are popped when their construction are done,
 *   a parent will only have one direct child at any instance of time.
 * - The last object is never popped, since it serves as the root of the json
 *   structure.
 *
 * A scope is a json object or
 * json array, and the name of the object or array. Scopes gives the assembler
 * contextual information to guides its actions: inserting values into parent
 * value may need a name, and correctly inserting values into the json tree
 * requires knowing what json value the assembler is in.
 *
 * There are two types of scopes: anonymous scopes and named scopes.
 * In the stack used to construct the root json value:
 * - named scope should precedes scope of json object, in another word,
 * named scope should be child of a scope of a json object.
 * - anonymous scopes should appear either at the root
 * or precedes scopes of json array, in another word, anonymous scope is
 * either the rot or child of a json array.
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
     * Remove all contents from the json tree and be ready for a session of
     * json parsing
     */
    void clear();

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

    /**
     * The scope holding the json value and the name
     */
    struct _Scope
    {
        /**
         * The value associated wit the current scope.
         */
        BasicValue<CharT> value;

        /**
         * The value associated wit the current scope.
         */
        std::basic_string<CharT> name;

        /**
         * Constructs an anonymous scope using the type of the json value.
         * @param type the type of json value to construct
         */
        _Scope(_VType type);

        /**
         * Constructs an named scope using the type of the json value.
         * @param type the type of json value to construct
         * @param name the name of the scope
         */
        _Scope(_VType type, std::basic_string<CharT> &&name);

        /**
         * Constructs an named scope using a json value. The resources of
         * the json value will be transfered.
         * @param value the json value of the scope
         * @param name the name of the scope
         */
        _Scope(BasicValue<CharT> &&value, std::basic_string<CharT> &&name);

        /**
         * Constructs a scope from an exsiting scope,
         * while moving array its resources
         * @param other the other scope
         */
        _Scope(_Scope &&other);

        /**
         * If the scope is anonymous
         * @returns true if name is empty
         */
        bool isAnonymous() const;
    };

    /**
     * Pop the "current" value and insert it into its parent scope, which
     * is the new "current" scope.
     */
    void _insertCurrentValueInParentScope();

    /**
     * Insert a value into the value held by the current scope using a name
     * @param value the value to insert
     * @param name the name to associated with the value when the value held by
     * the current scope is a json object.
     */
    void _insertValueInCurrentScope(_V &&value, _Name &&name);

    /**
     * The key used to be associated with values.
     */
    std::basic_string<CharT> _key;

    /**
     * The stack used to construct the json value.
     */
    std::stack<_Scope> _stack;
};
} // namespace json::assemble

// Implementations

namespace json::assemble
{
/**
 * Constructs an anonymous scope using the type of the json value.
 * @param type the type of json value to construct
 */
template<typename CharT>
Assembler<CharT>::_Scope::_Scope(_VType type)
    : value(type)
{
}

/**
 * Constructs an named scope using the type of the json value.
 * @param type the type of json value to construct
 * @param name the name of the scope
 */
template<typename CharT>
Assembler<CharT>::_Scope::_Scope(_VType type, std::basic_string<CharT> &&name)
    : value(type)
    , name(std::move(name))
{
}

/**
 * Constructs an named scope using a json value. The resources of
 * the json value will be transfered.
 * @param value the json value of the scope
 * @param name the name of the scope
 */
template<typename CharT>
Assembler<CharT>::_Scope::_Scope(
    BasicValue<CharT> &&value, std::basic_string<CharT> &&name)
    : value(std::move(value))
    , name(std::move(name))
{
}

/**
 * Constructs a scope from an exsiting scope,
 * while moving array its resources
 * @param other the other scope
 */
template<typename CharT>
Assembler<CharT>::_Scope::_Scope(_Scope &&other)
    : value(std::move(other.value))
    , name(std::move(other.name))
{
}

/**
 * If the scope is anonymous
 * @returns true if name is empty
 */
template<typename CharT>
bool Assembler<CharT>::_Scope::isAnonymous() const
{
    return name.empty();
}

// Assembler<CharT> Implementation

/**
 * Take a token and use the token to assemble the json value
 * @param token the token used to assemble.
 */
template<typename CharT>
void Assembler<CharT>::operator()(const json::token::Token<CharT> &token)
{
    takeToken(token);
}

/**
 * Take a token and use the token to assemble the json value
 * @param token the token used to assemble.
 */
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
            _insertCurrentValueInParentScope();
        }

        break;
    }
    case TType::endArray:
    {
        // Cannot touch the last one, needs it to return the root
        if (_stack.size() > size_t{ 1 })
        {
            _insertCurrentValueInParentScope();
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

            _insertValueInCurrentScope(std::move(primitive), std::move(_key));
        }

        break;
    }
    case TType::comment:
        break;
    default:
        break;
    }
}

/**
 * Remove all contents from the json tree and be ready for a session of
 * json parsing
 */
template<typename CharT>
void Assembler<CharT>::clear()
{
    while (!_stack.empty())
    {
        _stack.pop();
    }
}

/**
 * The root json value
 * @returns a reference to the root of the json value. If the the json
 * tree is empty, append a new json null primitive to the tree and returns
 * it.
 */
template<typename CharT>
json::BasicValue<CharT> &Assembler<CharT>::root()
{
    if (_stack.empty())
    {
        _stack.emplace(_VType::primitive);
    }

    return _stack.top().value;
}

/**
 * The root json value
 * @returns a constant reference to the root of the json value. If the the
 * json tree is empty, append a new json null primitive to the tree and
 * returns it.
 */
template<typename CharT>
const json::BasicValue<CharT> &Assembler<CharT>::root() const
{
    if (_stack.empty())
    {
        _stack.emplace(_VType::primitive);
    }

    return _stack.top().value;
}

/**
 * Pop the "current" value and insert it into its parent scope, which
 * is the new "current" scope.
 */
template<typename CharT>
void Assembler<CharT>::_insertCurrentValueInParentScope()
{
    _Scope current = std::move(_stack.top());
    _stack.pop();

    _insertValueInCurrentScope(
        std::move(current.value), std::move(current.name));
}

/**
 * Insert a value into the value held by the current scope using a name
 * @param value the value to insert
 * @param name the name to associated with the value when the value held by
 * the current scope is a json object.
 */
template<typename CharT>
void Assembler<CharT>::_insertValueInCurrentScope(_V &&value, _Name &&name)
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
} // namespace json::assemble