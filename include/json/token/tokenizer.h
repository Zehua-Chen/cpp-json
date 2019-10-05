//
//  Tokenizer.hpp
//  CPP
//
//  Created by Zehua Chen on 4/15/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#pragma once

#include <bitset>
#include <cmath>
#include <string>
#include "json/token/token.h"
#include "json/utils/convert.h"
#include "json/utils/letters.h"

namespace json::token {
template <typename CharT, typename IterT>
class Tokenizer {
 public:
  Tokenizer(IterT begin, IterT end);

  /**
   * Take an input iterator to extract letter to process
   * @param iter the input iterator to extract the letter from.
   * @param end the end input iterator
   * @returns returns after a iterator has been found
   */
  void Extract();

  bool IsAtEndOfString();

  /**
   * Get a reference to the current token
   * @returns a reference to the current token
   */
  Token<CharT> &token();

  operator bool();

 private:
  void String();
  void Number();
  void True();
  void False();
  void Null();

  Token<CharT> token_;
  IterT begin_;
  IterT end_;
};
}  // namespace json::token

// Implementations

namespace json::token {
template <typename CharT, typename IterT>
Tokenizer<CharT, IterT>::Tokenizer(IterT begin, IterT end)
    : begin_(begin), end_(end) {}

template <typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::Extract() {
  using namespace json::utils;
  using TType = typename Token<CharT>::Type;

  while (begin_ != end_) {
    CharT letter = *begin_;

    switch (letter) {
      case ' ':
      case '\r':
      case '\t':
      case '\n':
        ++begin_;
        continue;
      case '{':
        token_.type = TType::kBeginObject;
        ++begin_;
        return;
      case '}':
        token_.type = TType::kEndObject;
        ++begin_;
        return;
      case '[':
        token_.type = TType::kBeginArray;
        ++begin_;
        return;
      case ']':
        token_.type = TType::kEndArray;
        ++begin_;
        return;
      case ',':
        token_.type = TType::kValueSeparator;
        ++begin_;
        return;
      case ':':
        token_.type = TType::kKeyValueSeparator;
        ++begin_;
        return;
      case '\"':
        ++begin_;
        return String();
      case '-':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        return Number();
      case 't':
        return True();
      case 'f':
        return False();
      case 'n':
        return Null();
      default:
        // TODO: Error handling
        ++begin_;
        break;
    }
  }
}

template <typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::String() {
  enum class State {
    kRegular,
    kEscape,
    kHex,
  };

  using namespace json::utils;
  using namespace json::utils::convert;

  using TType = typename Token<CharT>::Type;

  int8_t hex_counter = 0;
  int8_t hex_append_count = 0;
  int32_t hex_value = 0;
  State state = State::kRegular;
  std::basic_string<CharT> buffer;

  while (begin_ != end_) {
    CharT letter = *begin_;
    ++begin_;

    switch (state) {
      case State::kRegular:
        switch (letter) {
          // end of string
          case '\"':
            token_.type = TType::kString;
            token_.data = std::move(buffer);
            return;
          // start of escape sequence
          case '\\':
            state = State::kEscape;
            break;
          // regular text
          default:
            buffer += letter;
        }

        break;
      case State::kEscape:
        switch (letter) {
          case letters::kB<CharT>:
            state = State::kRegular;
            buffer += letters::kBackspace<CharT>;
            break;
          case letters::kF<CharT>:
            state = State::kRegular;
            buffer += letters::kFormfeed<CharT>;
            break;
          case letters::kN<CharT>:
            state = State::kRegular;
            buffer += letters::kEndline<CharT>;
            break;
          case letters::kR<CharT>:
            state = State::kRegular;
            buffer += letters::kCarriageReturn<CharT>;
            break;
          case letters::kT<CharT>:
            state = State::kRegular;
            buffer += letters::kTab<CharT>;
            break;
          case letters::kSolidus<CharT>:
            state = State::kRegular;
            buffer += letters::kSolidus<CharT>;
            break;
          case letters::kBackSolidus<CharT>:
            state = State::kRegular;
            buffer += letters::kBackSolidus<CharT>;
            break;
          case letters::kDoubleQuote<CharT>:
            state = State::kRegular;
            buffer += letters::kDoubleQuote<CharT>;
            break;
          case letters::kU<CharT>:
            state = State::kHex;
            break;
        }
        break;
      case State::kHex: {
        ++hex_counter;
        // stop after enough hex
        auto current_hex_value = number::FromHex<int8_t>(letter);

        // TODO:
        // handle error if currentHexValue is -1

        // utf8
        if constexpr (sizeof(CharT) == 1) {
          hex_value <<= 4;
          hex_value |= current_hex_value;

          ++hex_append_count;

          if (hex_append_count == 2) {
            buffer += hex_value;

            hex_value = 0;
            hex_append_count = 0;
          }
        }
        // utf16 and utf32
        else {
          hex_value <<= 4;
          hex_value |= current_hex_value;

          ++hex_append_count;

          if (hex_append_count == 4) {
            buffer += hex_value;

            hex_value = 0;
            hex_append_count = 0;
          }
        }

        if (hex_counter == 4) {
          state = State::kRegular;
          hex_counter = 0;
        }
        break;
      }
    }
  }
}

template <typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::Number() {
  using namespace utils::convert;
  // Number format:
  // -010.2e+3
  enum class State {
    undefined,
    beforeDecimalPoint,
    afterDecimalPoint,
    afterE,
    afterESign,
  };

  State state = State::undefined;
  double value = 0.0;
  double sign = 1;
  double postDecimalScale = 0.1;
  double scaleSign = 1.0;
  double scale = 0.0;

  const auto exportNumber = [&]() {
    scale *= scaleSign;
    return value * sign * std::pow(10, scale);
  };

  while (begin_ != end_) {
    CharT letter = *begin_;

    switch (letter) {
      case ']':
      case '}':
      case ' ':
      case ',':
      case '\t':
      case '\r':
      case '\n':
        return token_.FormNumber(exportNumber());
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        switch (state) {
          case State::undefined:
            value *= 10.0;
            value += number::FromDec<double>(letter);
            state = State::beforeDecimalPoint;
            ++begin_;
            break;
          case State::beforeDecimalPoint:
            value *= 10.0;
            value += number::FromDec<double>(letter);
            ++begin_;
            break;
          case State::afterDecimalPoint:
            value += number::FromDec<double>(letter) * postDecimalScale;
            postDecimalScale /= 10.0;
            ++begin_;
            break;
          case State::afterE:
            state = State::afterESign;
            break;
          case State::afterESign:
            scale *= 10;
            scale += number::FromDec<double>(letter);
            ++begin_;
            break;
          default:
            ++begin_;
            break;
        }
        break;
      case 'e':
      case 'E':
        switch (state) {
          case State::undefined:
          case State::beforeDecimalPoint:
          case State::afterDecimalPoint:
            state = State::afterE;
            break;
          default:
            break;
        }
        ++begin_;
        break;
      case '.':
        switch (state) {
          case State::undefined:
          case State::beforeDecimalPoint:
            state = State::afterDecimalPoint;
            break;
          default:
            break;
        }
        ++begin_;
        break;
      case '-':
        switch (state) {
          case State::undefined:
            sign = -1;
            state = State::beforeDecimalPoint;
            break;
          case State::afterE:
            scaleSign = -1;
            state = State::afterESign;
            break;
          default:
            break;
        }
        ++begin_;
        break;
      default:
        break;
    }
  }

  token_.FormNumber(exportNumber());
}

template <typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::True() {
  const char letters[] = {'t', 'r', 'u', 'e'};
  int i = 0;

  while (begin_ != end_) {
    CharT letter = *begin_;

    if (letter != static_cast<CharT>(letters[i])) {
      // TODO: Error handling
    }

    if (i == 3) {
      ++begin_;
      return token_.FormBoolean(true);
    }

    ++i;
    ++begin_;
  }

  // TODO: Error handling
}

template <typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::False() {
  const char letters[] = {'f', 'a', 'l', 's', 'e'};
  int i = 0;

  while (begin_ != end_) {
    CharT letter = *begin_;

    if (letter != static_cast<CharT>(letters[i])) {
      // TODO: Error handling
    }

    if (i == 4) {
      ++begin_;
      return token_.FormBoolean(false);
    }

    ++i;
    ++begin_;
  }

  // TODO: Error handling
}

template <typename CharT, typename IterT>
void Tokenizer<CharT, IterT>::Null() {
  const char letters[] = {'n', 'u', 'l', 'l'};
  int i = 0;

  while (begin_ != end_) {
    CharT letter = *begin_;

    if (letter != static_cast<CharT>(letters[i])) {
      // TODO: Error handling
    }

    if (i == 3) {
      token_.type = Token<CharT>::Type::kNull;
      ++begin_;
      return;
    }

    ++i;
    ++begin_;
  }

  // TODO: Error handling
}

template <typename CharT, typename IterT>
Token<CharT> &Tokenizer<CharT, IterT>::token() {
  return token_;
}

template <typename CharT, typename IterT>
Tokenizer<CharT, IterT>::operator bool() {
  return begin_ != end_;
}
}  // namespace json::token
