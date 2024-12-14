#ifndef RFL_TIMESTAMP_HPP_
#define RFL_TIMESTAMP_HPP_

#include <ctime>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Result.hpp"
#include "internal/StringLiteral.hpp"

namespace rfl {

/// For serializing and deserializing time stamps.
template <internal::StringLiteral _format>
class Timestamp {
  constexpr static const internal::StringLiteral format_ = _format;

 public:
  using Format = rfl::Literal<_format>;

  using ReflectionType = std::string;

  Timestamp() : tm_(std::tm{}) {}

  Timestamp(const char* _str) : tm_(std::tm{}) {
    std::istringstream ss(_str);
    ss >> std::get_time(&tm_, _format.str().c_str());
    if (ss.fail()) {
      throw std::runtime_error("String '" + std::string(_str) +
                               "' did not match format '" + Format().str() +
                               "'.");
    }
  }

  Timestamp(const std::string& _str) : Timestamp(_str.c_str()) {}

  Timestamp(const std::tm& _tm) : tm_(_tm) {}

  ~Timestamp() = default;

  /// Returns a result containing the timestamp when successful or an Error
  /// otherwise.
  static Result<Timestamp> from_string(const char* _str) noexcept {
    try {
      return Timestamp(_str);
    } catch (std::exception& e) {
      return Error(e.what());
    }
  }

  /// Returns a result containing the timestamp when successful or an Error
  /// otherwise.
  static Result<Timestamp> from_string(const std::string& _str) {
    return from_string(_str.c_str());
  }

  /// Necessary for the serialization to work.
  ReflectionType reflection() const {
    char outstr[200];
    strftime(outstr, 200, format_.str().c_str(), &tm_);
    return std::string(outstr);
  }

  /// Expresses the underlying timestamp as a string.
  std::string str() const { return reflection(); }

  /// Trivial accessor to the underlying time stamp.
  std::tm& tm() { return tm_; }

  /// Trivial (const) accessor to the underlying time stamp.
  const std::tm& tm() const { return tm_; }

 private:
  /// The underlying time stamp.
  std::tm tm_;
};

}  // namespace rfl

#endif
