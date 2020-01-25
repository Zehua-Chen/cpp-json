#pragma once

#include <string>
#include <vector>
#include "json/token/token.h"
#include "json/token/tokenizer.h"
#include "json/value/basic_value.h"
#include "json/utils/result.h"

namespace json::parser {
template <typename CharT>
class Parser {
 public:
  Parser();
  void take(const token::Token<CharT> &token);
  BasicValue<CharT> root();

 private:
  using _TType = typename token::Token<CharT>::Type;
  using _VType = typename json::BasicValue<CharT>::Type;

  struct _Scope {
    std::basic_string<CharT> name;
    BasicValue<CharT> value;

    template <typename... Arg>
    _Scope(std::basic_string<CharT> &&name, Arg... args)
        : name{std::move(name)}, value{args...} {}

    _Scope(_Scope &&other)
        : name{std::move(other.name)}, value{std::move(other.value)} {}
  };

  enum class _State {
    start,
    error,
    finished,
    objectStart,
    objectHasKeyString,
    objectHasKey,
    objectHasValue,
    objectReady,
    arrayStart,
    arrayHasValue,
    arrayReady,
  };

  enum class _Context {
    undefined,
    none,
    object,
    array,
  };

  _Context _context();

  // State handlers

  void _start(const token::Token<CharT> &token);
  void _finished(const token::Token<CharT> &token);
  void _objectStart(const token::Token<CharT> &token);
  void _objectHasKeyString(const token::Token<CharT> &token);
  void _objectHasKey(const token::Token<CharT> &token);
  void _objectHasValue(const token::Token<CharT> &token);
  void _objectReady(const token::Token<CharT> &token);
  void _arrayStart(const token::Token<CharT> &token);
  void _arrayHasValue(const token::Token<CharT> &token);
  void _arrayReady(const token::Token<CharT> &token);

  _State _state;
  std::vector<_Scope> _stack;
  std::basic_string<CharT> _key;
};
}  // namespace json::parser

namespace json::parser {
template <typename CharT>
Parser<CharT>::Parser() : _state(_State::start) {}

template <typename CharT>
void Parser<CharT>::take(const token::Token<CharT> &token) {
  switch (_state) {
    case _State::start:
      return _start(token);
    case _State::finished:
      return _finished(token);
    case _State::objectStart:
      return _objectStart(token);
    case _State::objectHasKeyString:
      return _objectHasKeyString(token);
    case _State::objectHasKey:
      return _objectHasKey(token);
    case _State::objectHasValue:
      return _objectHasValue(token);
    case _State::objectReady:
      return _objectReady(token);
    case _State::arrayStart:
      return _arrayStart(token);
    case _State::arrayHasValue:
      return _arrayHasValue(token);
    case _State::arrayReady:
      return _arrayReady(token);
    case _State::error:
      return;
  }
}

template <typename CharT>
BasicValue<CharT> Parser<CharT>::root() {
  return _stack.back().value;
}

template <typename CharT>
typename Parser<CharT>::_Context Parser<CharT>::_context() {
  if (_stack.size() <= 1) {
    return _Context::none;
  }

  _Scope &parent = _stack[_stack.size() - 2];

  switch (parent.value.type()) {
    case BasicValue<CharT>::Type::kObject:
      return _Context::object;
    case BasicValue<CharT>::Type::kArray:
      return _Context::array;
    default:
      return _Context::none;
  }
}
template <typename CharT>
void Parser<CharT>::_start(const token::Token<CharT> &token) {
  switch (token.type) {
    case _TType::kString:
      _stack.emplace_back("", token.string());
      _state = _State::finished;
      return;
    case _TType::kNumber:
      _stack.emplace_back("", token.number());
      _state = _State::finished;
      return;
    case _TType::kBoolean:
      _stack.emplace_back("", token.boolean());
      _state = _State::finished;
      return;
    case _TType::kNull:
      _stack.emplace_back("");
      _state = _State::finished;
      return;
    case _TType::kBeginObject:
      _stack.emplace_back("", _VType::kObject);
      _state = _State::objectStart;
      return;
    case _TType::kBeginArray:
      _stack.emplace_back("", _VType::kArray);
      _state = _State::arrayStart;
      return;
    default:
      // TODO: Handle error
      return;
  }
}
template <typename CharT>
void Parser<CharT>::_finished(const token::Token<CharT> &token) {
  // TODO: Handle error
}
template <typename CharT>
void Parser<CharT>::_objectStart(const token::Token<CharT> &token) {
  switch (token.type) {
    case _TType::kEndObject: {
      switch (_context()) {
        case _Context::object:
          _state = _State::objectHasValue;
          break;
        case _Context::array:
          _state = _State::arrayHasValue;
          break;
        case _Context::none:
          _state = _State::finished;
          break;
        case _Context::undefined:
          // TODO: Handle error
          break;
      }

      return;
    }
    case _TType::kString:
      _key = token.string();
      _state = _State::objectHasKeyString;
      return;
    default:
      // TODO: Handle error
      return;
  }
}
template <typename CharT>
void Parser<CharT>::_objectHasKeyString(const token::Token<CharT> &token) {
  switch (token.type) {
    case _TType::kKeyValueSeparator:
      _state = _State::objectHasKey;
      break;
    default:
      // TODO: Handle error
      return;
  }
}
template <typename CharT>
void Parser<CharT>::_objectHasKey(const token::Token<CharT> &token) {
  switch (token.type) {
    case _TType::kString:
      _stack.emplace_back(std::move(_key), token.string());
      _state = _State::objectHasValue;
      return;
    case _TType::kNumber:
      _stack.emplace_back(std::move(_key), token.number());
      _state = _State::objectHasValue;
      return;
    case _TType::kBoolean:
      _stack.emplace_back(std::move(_key), token.boolean());
      _state = _State::objectHasValue;
      return;
    case _TType::kNull:
      _stack.emplace_back(std::move(_key));
      _state = _State::objectHasValue;
      return;
    case _TType::kBeginObject:
      _stack.emplace_back(std::move(_key), _VType::kObject);
      _state = _State::objectStart;
      return;
    case _TType::kBeginArray:
      _stack.emplace_back(std::move(_key), _VType::kArray);
      _state = _State::arrayStart;
      return;
    default:
      // TODO: Handle error
      return;
  }
}
template <typename CharT>
void Parser<CharT>::_objectHasValue(const token::Token<CharT> &token) {
  _Scope top = std::move(_stack.back());
  _stack.pop_back();

  _Scope &parent = _stack.back();
  parent.value[top.name] = std::move(top.value);

  switch (token.type) {
    case _TType::kEndObject: {
      switch (_context()) {
        case _Context::object:
          _state = _State::objectHasValue;
          break;
        case _Context::array:
          _state = _State::arrayHasValue;
          break;
        case _Context::none:
          _state = _State::finished;
          break;
        case _Context::undefined:
          // TODO: Handle error
          break;
      }

      return;
    }
    case _TType::kValueSeparator:
      _state = _State::objectReady;
      return;
    default:
      // TODO: Handle error
      return;
  }
}

template <typename CharT>
void Parser<CharT>::_objectReady(const token::Token<CharT> &token) {
  switch (token.type) {
    case _TType::kString:
      _key = token.string();
      _state = _State::objectHasKeyString;
      return;
    default:
      // TODO: Handle error
      return;
  }
}
template <typename CharT>
void Parser<CharT>::_arrayStart(const token::Token<CharT> &token) {
  switch (token.type) {
    case _TType::kString:
      _stack.emplace_back(std::move(_key), token.string());
      _state = _State::arrayHasValue;
      return;
    case _TType::kNumber:
      _stack.emplace_back(std::move(_key), token.number());
      _state = _State::arrayHasValue;
      return;
    case _TType::kBoolean:
      _stack.emplace_back(std::move(_key), token.boolean());
      _state = _State::arrayHasValue;
      return;
    case _TType::kNull:
      _stack.emplace_back(std::move(_key));
      _state = _State::arrayHasValue;
      return;
    case _TType::kBeginObject:
      _stack.emplace_back(std::move(_key), _VType::kObject);
      _state = _State::objectStart;
      return;
    case _TType::kBeginArray:
      _stack.emplace_back(std::move(_key), _VType::kArray);
      _state = _State::arrayStart;
      return;
    default:
      // TODO: Handle error
      return;
  }
}
template <typename CharT>
void Parser<CharT>::_arrayHasValue(const token::Token<CharT> &token) {
  _Scope top = std::move(_stack.back());
  _stack.pop_back();

  _Scope &parent = _stack.back();
  parent.value.Append(std::move(top.value));

  switch (token.type) {
    case _TType::kEndArray: {
      switch (_context()) {
        case _Context::object:
          _state = _State::objectHasValue;
          break;
        case _Context::array:
          _state = _State::arrayHasValue;
          break;
        case _Context::none:
          _state = _State::finished;
          break;
        case _Context::undefined:
          // TODO: Handle error
          break;
      }

      return;
    }
    case _TType::kValueSeparator:
      _state = _State::arrayReady;
      return;
    default:
      // TODO: Handle error
      return;
  }
}
template <typename CharT>
void Parser<CharT>::_arrayReady(const token::Token<CharT> &token) {
  switch (token.type) {
    case _TType::kString:
      _stack.emplace_back(std::move(_key), token.string());
      _state = _State::arrayHasValue;
      return;
    case _TType::kNumber:
      _stack.emplace_back(std::move(_key), token.number());
      _state = _State::arrayHasValue;
      return;
    case _TType::kBoolean:
      _stack.emplace_back(std::move(_key), token.boolean());
      _state = _State::arrayHasValue;
      return;
    case _TType::kNull:
      _stack.emplace_back(std::move(_key));
      _state = _State::arrayHasValue;
      return;
    case _TType::kBeginObject:
      _stack.emplace_back(std::move(_key), _VType::kObject);
      _state = _State::objectStart;
      return;
    case _TType::kBeginArray:
      _stack.emplace_back(std::move(_key), _VType::kArray);
      _state = _State::arrayStart;
      return;
    default:
      // TODO: Handle error
      return;
  }
}

}  // namespace json::parser
