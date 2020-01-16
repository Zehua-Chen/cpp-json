#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <variant>

namespace json {
/**
 * @brief A key object that can be used to index json objects
 *
 * A key can be in two states, with ownership and without ownerships. The ones
 * do not have ownerships must not outlive the storage that provides the string
 */
template <typename CharT>
class BasicKey {
 public:
  /**
   * @brief Type to represent a string, aka. a with-ownership string
   */
  using String = std::basic_string<CharT>;

  /**
   * @brief Type to represent a string view, aka. a without-ownership string
   */
  using StringView = std::basic_string_view<CharT>;

  /**
   * @brief Type to store the data of the key
   */
  using Data = std::variant<String, StringView>;

  /**
   * @brief Construt a key with-ownership
   * @param key the key to copy from
   */
  BasicKey(const std::basic_string<CharT> &key);

  /**
   * @brief Construt a key without-ownership
   * @param key the key to reference to
   */
  BasicKey(const CharT *key);

  /**
   * @brief Determine if the key has ownership to the string
   * @returns `true` if has ownership, `false` otherwise
   */
  bool HasOwnership() const;

  /**
   * @brief Determine if two keys are equal
   * @returns `true` if equal, false otherwise
   */
  bool operator==(const BasicKey<CharT> &other) const;

  /**
   * @brief Returns 0 if has ownership, 1 otherwise
   * @returns an integer value
   */
  int index() const;

  /**
   * @brief Returns a constant reference to the underlying data
   * @returns a reference to the underlying data
   */
  const Data &data() const;

 private:
  Data data_;
};
}  // namespace json

namespace json {
template <typename CharT>
BasicKey<CharT>::BasicKey(const std::basic_string<CharT> &key) {
  data_.template emplace<0>(key);
}

template <typename CharT>
BasicKey<CharT>::BasicKey(const CharT *key) {
  data_.template emplace<1>(key);
}

template <typename CharT>
bool BasicKey<CharT>::HasOwnership() const {
  return data_.index() == 0;
}

template <typename CharT>
int BasicKey<CharT>::index() const {
  return data_.index();
}

template <typename CharT>
const typename BasicKey<CharT>::Data &BasicKey<CharT>::data() const {
  return data_;
}

template <typename CharT>
bool BasicKey<CharT>::operator==(const BasicKey<CharT> &other) const {
  using std::get;

  if (data_.index() == 0 && other.index() == 1) {
    return get<String>(data_) == get<StringView>(other.data_);
  } else if (data_.index() == 1 && other.index() == 0) {
    return get<StringView>(data_) == get<String>(other.data_);
  }

  return data_ == other.data_;
}
}  // namespace json

// Hashing

namespace std {
template <typename CharT>
struct hash<json::BasicKey<CharT>> {
  size_t operator()(const json::BasicKey<CharT> &key) const {
    switch (key.index()) {
      case 0: {
        hash<basic_string<CharT>> hasher;
        return hasher(get<basic_string<CharT>>(key.data()));
      }
      case 1: {
        hash<basic_string_view<CharT>> hasher;
        return hasher(get<basic_string_view<CharT>>(key.data()));
      }
      default:
        return 0;
    }
  }
};
}  // namespace std