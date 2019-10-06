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
#include <istream>
#include <string>
#include "json/token/token.h"
#include "json/utils/convert.h"
#include "json/utils/letters.h"

namespace json::token {
template <typename CharT>
class Tokenizer {
 public:
  using InputStream = std::basic_istream<CharT>;
  Tokenizer(InputStream &input_stream);

  /**
   * Take an input iterator to extract letter to process
   * @param iter the input iterator to extract the letter from.
   * @param end the end input iterator
   * @returns returns after a iterator has been found
   */
  void Extract();

  bool Done();

  /**
   * Get a reference to the current token
   * @returns a reference to the current token
   */
  Token<CharT> &token();

 private:
  void String();
  void Number();
  void True();
  void False();
  void Null();

  Token<CharT> token_;
  InputStream &input_stream_;
};
}  // namespace json::token

// Implementations

namespace json::token {
template <typename CharT>
Tokenizer<CharT>::Tokenizer(InputStream &input_stream)
    : input_stream_(input_stream) {}

template <typename CharT>
bool Tokenizer<CharT>::Done() {
  // can't directly use eof() method here.
  // eof() would only return true after eof has been read.
  // However, this method is supposed to return true when it sees eof()
  return input_stream_.peek() == std::char_traits<CharT>::eof();
}

template <typename CharT>
void Tokenizer<CharT>::Extract() {
  using namespace json::utils;
  using TType = typename Token<CharT>::Type;

  while (!this->Done()) {
    CharT letter = input_stream_.peek();
    switch (letter) {
      case ' ':
      case '\r':
      case '\t':
      case '\n':
        input_stream_.get();
        continue;
      case '{':
        token_.type = TType::kBeginObject;
        input_stream_.get();
        return;
      case '}':
        token_.type = TType::kEndObject;
        input_stream_.get();
        return;
      case '[':
        token_.type = TType::kBeginArray;
        input_stream_.get();
        return;
      case ']':
        token_.type = TType::kEndArray;
        input_stream_.get();
        return;
      case ',':
        token_.type = TType::kValueSeparator;
        input_stream_.get();
        return;
      case ':':
        token_.type = TType::kKeyValueSeparator;
        input_stream_.get();
        return;
      case '\"':
        input_stream_.get();
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
        break;
    }
  }
}

template <typename CharT>
void Tokenizer<CharT>::String() {
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

  while (!this->Done()) {
    CharT letter;
    input_stream_.get(letter);

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

template <typename CharT>
void Tokenizer<CharT>::Number() {
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

  while (!this->Done()) {
    CharT letter = input_stream_.peek();

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
            input_stream_.get();
            break;
          case State::beforeDecimalPoint:
            value *= 10.0;
            value += number::FromDec<double>(letter);
            input_stream_.get();
            break;
          case State::afterDecimalPoint:
            value += number::FromDec<double>(letter) * postDecimalScale;
            postDecimalScale /= 10.0;
            input_stream_.get();
            break;
          case State::afterE:
            state = State::afterESign;
            break;
          case State::afterESign:
            scale *= 10;
            scale += number::FromDec<double>(letter);
            input_stream_.get();
            break;
          default:
            input_stream_.get();
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

        input_stream_.get(letter);
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

        input_stream_.get(letter);
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
        input_stream_.get();
        break;
      default:
        break;
    }
  }

  token_.FormNumber(exportNumber());
}

template <typename CharT>
void Tokenizer<CharT>::True() {
  const char letters[] = {'t', 'r', 'u', 'e'};
  int i = 0;

  while (!this->Done()) {
    CharT letter = input_stream_.peek();

    if (letter != static_cast<CharT>(letters[i])) {
      // TODO: Error handling
    }

    if (i == 3) {
      input_stream_.get(letter);
      return token_.FormBoolean(true);
    }

    ++i;
    input_stream_.get(letter);
  }

  // TODO: Error handling
}

template <typename CharT>
void Tokenizer<CharT>::False() {
  const char letters[] = {'f', 'a', 'l', 's', 'e'};
  int i = 0;

  while (!this->Done()) {
    CharT letter = input_stream_.peek();

    if (letter != static_cast<CharT>(letters[i])) {
      // TODO: Error handling
    }

    if (i == 4) {
      input_stream_.get(letter);
      return token_.FormBoolean(false);
    }

    ++i;
    input_stream_.get(letter);
  }

  // TODO: Error handling
}

template <typename CharT>
void Tokenizer<CharT>::Null() {
  const char letters[] = {'n', 'u', 'l', 'l'};
  int i = 0;

  while (!this->Done()) {
    CharT letter = input_stream_.peek();

    if (letter != static_cast<CharT>(letters[i])) {
      // TODO: Error handling
    }

    if (i == 3) {
      token_.type = Token<CharT>::Type::kNull;
      input_stream_.get(letter);
      return;
    }

    ++i;
    input_stream_.get(letter);
  }

  // TODO: Error handling
}

template <typename CharT>
Token<CharT> &Tokenizer<CharT>::token() {
  return token_;
}
}  // namespace json::token
