#pragma once

#include <string_view>
#include "json/parser/parser.h"
#include "json/value/basic_value.h"

namespace json {
/**
 * @brief Parse json value
 * @param istream, the input stream to parse the json from
 */
template <typename CharT = char>
BasicValue<CharT> parse(std::basic_istream<CharT> &istream);

/**
 * @brief Parse json value
 * @param istream, the string view to parse the json from
 */
template <typename CharT = char>
BasicValue<CharT> parse(std::basic_string_view<CharT> &str_view);

/**
 * @brief UTF8 Value
 */
using Value = BasicValue<char>;

/**
 * @brief UTF8 Key
 */
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