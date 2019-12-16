#pragma once

#include <string_view>
#include "json/parser/parser.h"
#include "json/value/basic_value.h"

namespace json {
template <typename CharT = char>
BasicValue<CharT> parse(std::basic_istream<CharT> &istream);

template <typename CharT = char>
BasicValue<CharT> parse(std::basic_string_view<CharT> &str_view);

using Value = BasicValue<char>;
using Key = BasicKey<char>;
}  // namespace json

namespace json {
template <typename CharT>
BasicValue<CharT> parse(std::basic_istream<CharT> &istream) {
  token::Tokenizer<CharT> tokenizer{istream};
  parser::Parser<CharT> parser;

  while (!tokenizer.Done()) {
    tokenizer.Extract();
    parser.take(tokenizer.token());
  }

  return parser.root();
}

template <typename CharT>
BasicValue<CharT> parse(std::basic_string_view<CharT> &str_view) {
  std::stringstream ss;
  ss << str_view;

  return parse(ss);
}
}  // namespace json