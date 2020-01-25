#pragma once

#include <variant>

namespace json::utils {
template <typename T, typename E>
class Result {
 public:
  template <typename TT, typename EE>
  friend Result<TT, EE> Ok(TT t);

  template <typename TT, typename EE>
  friend Result<TT, EE> Error(EE e);

  bool is_ok() const;
  bool is_error() const;

  T &result();
  const T &result() const;

  E &error();
  const E &error() const;

 private:
  std::variant<T, E> data_;
};

template <typename TT, typename EE>
Result<TT, EE> Ok(TT t);

template <typename TT, typename EE>
Result<TT, EE> Error(EE e);
}  // namespace json::utils

// Implementation

namespace json::utils {
template <typename T, typename E>
bool Result<T, E>::is_ok() const {
  return data_.index() == 0;
}

template <typename T, typename E>
bool Result<T, E>::is_error() const {
  return data_.index() == 1;
}

template <typename T, typename E>
T &Result<T, E>::result() {
  return std::get<0>(data_);
}

template <typename T, typename E>
const T &Result<T, E>::result() const {
  return std::get<0>(data_);
}

template <typename T, typename E>
E &Result<T, E>::error() {
  return std::get<1>(data_);
}

template <typename T, typename E>
const E &Result<T, E>::error() const {
  return std::get<1>(data_);
}

template <typename TT, typename EE>
Result<TT, EE> Ok(TT t) {
  Result<TT, EE> result;
  result.data_.template emplace<0>(t);

  return result;
}

template <typename TT, typename EE>
Result<TT, EE> Error(EE e) {
  Result<TT, EE> result;
  result.data_.template emplace<1>(e);

  return result;
}
}  // namespace json::utils
