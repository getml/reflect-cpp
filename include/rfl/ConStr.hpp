#ifndef RFL_UUID_HPP_
#define RFL_UUID_HPP_

#include <ctime>
#include <iterator>
#include <regex>
#include <stdexcept>
#include <string>

#include "internal/StringLiteral.hpp"
#include "rfl/Literal.hpp"
#include "rfl/Result.hpp"

namespace rfl {

template <internal::StringLiteral _pattern = "", int8_t _min = -1,
          int8_t _max = -1>
class ConStr {

  constexpr static const internal::StringLiteral pattern_ = _pattern;

public:
  using ReflectionType = std::string;
  ConStr(const char *_str) {
    auto unvalidated_str = std::string(_str);
    if constexpr (pattern_ != internal::StringLiteral{""}) {
      std::regex regex_pattern(pattern_.str());
      if (!std::regex_match(unvalidated_str, regex_pattern)) {
        throw std::runtime_error("String '" + unvalidated_str +
                                 "' did not match format " + pattern_.str() +
                                 " format");
      }
    }
    if constexpr (_min >= 0) {
      if (unvalidated_str.length() < _min) {
        throw std::runtime_error("String '" + unvalidated_str +
                                 "' is shorter than min length" +
                                 std::to_string(_min));
      }
    }
    if constexpr (_max >= 0) {
      if (unvalidated_str.length() > _max) {
        throw std::runtime_error("String '" + unvalidated_str +
                                 "' is shorter than max length" +
                                 std::to_string(_max));
      }
    }
    value_ = _str;
  }

  ConStr(const std::string &_str) : ConStr(_str.c_str()){};
  ~ConStr() = default;

  std::string value_;

  ReflectionType reflection() const { return value_; }

  /// Returns a result containing the ConStr when successful or an Error
  /// otherwise.
  static Result<ConStr> from_string(const char *_str) noexcept {
    try {
      return ConStr(_str);
    } catch (std::exception &e) {
      return Error(e.what());
    }
  }

  /// Returns a result containing the ConStr when successful or an Error
  /// otherwise.
  static Result<ConStr> from_string(const std::string &_str) {
    return from_string(_str.c_str());
  }
};
/// UUID
using UUIDv1 = ConStr<
    R"(^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-1[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$)">;
using UUIDv2 = ConStr<
    R"(^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-2[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$)">;
using UUIDv3 = ConStr<
    R"(^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-3[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$)">;
using UUIDv4 = ConStr<
    R"(^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-4[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$)">;

// This pattern will match valid Base64 encoded strings with or without padding.
using Base64Encoded = ConStr<R"(^[A-Za-z0-9+/]*(={0,2})?$)">;

/// email
using Email = ConStr<R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)">;

} // namespace rfl

#endif
