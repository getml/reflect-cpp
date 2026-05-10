#ifndef RFL_TIMESTAMP_HPP_
#define RFL_TIMESTAMP_HPP_

#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Literal.hpp"
#include "Result.hpp"
#include "internal/StringLiteral.hpp"

namespace rfl {

/// For serializing and deserializing timestamps with a specific format.
/// This class wraps a std::tm structure and provides parsing/formatting
/// using a compile-time format string (strptime/strftime format).
/// Useful for handling dates and times in various formats (ISO8601, RFC3339, custom formats).
/// @tparam _format The compile-time format string (e.g., "%Y-%m-%d %H:%M:%S")
template <internal::StringLiteral _format>
class Timestamp {
  constexpr static const internal::StringLiteral format_ = _format;

 public:
  using Format = rfl::Literal<_format>;

  using ReflectionType = std::string;

  /// Default constructor - initializes with zero time.
  Timestamp() : tm_(std::tm{}) {}

  /// Constructs a timestamp from a C-string matching the format.
  /// @param _str The string to parse
  /// @throws std::runtime_error if the string doesn't match the format
  Timestamp(const char* _str) : tm_(std::tm{}) {
    const auto r = strptime(_str, _format.str().c_str(), &tm_);
    if (r == NULL) {
      throw std::runtime_error("String '" + std::string(_str) +
                               "' did not match format '" + Format().str() +
                               "'.");
    }
  }

  /// Constructs a timestamp from a std::string matching the format.
  /// @param _str The string to parse
  /// @throws std::runtime_error if the string doesn't match the format
  Timestamp(const std::string& _str) : Timestamp(_str.c_str()) {}

  /// Constructs a timestamp from a std::tm structure.
  /// @param _tm The time structure to wrap
  Timestamp(const std::tm& _tm) : tm_(_tm) {}

  /// Constructs a timestamp from a time_t value.
  /// @param _t The time_t value representing seconds since epoch
  Timestamp(const time_t _t) : tm_(std::tm{}) {
    auto t = _t;
#if defined(_MSC_VER) || defined(__MINGW32__)
    gmtime_s(&tm_, &t);
#else
    gmtime_r(&t, &tm_);
#endif
  }

  /// Destructor.
  ~Timestamp() = default;

  /// Returns a result containing the timestamp when successful or an Error otherwise.
  /// This is an alternative to the throwing constructor for error handling without exceptions.
  /// @param _str The string to parse in the format specified by the template parameter
  /// @return Result containing the Timestamp if parsing succeeded, or an error otherwise
  static Result<Timestamp> from_string(const char* _str) noexcept {
    try {
      return Timestamp(_str);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  /// Returns a result containing the timestamp when successful or an Error otherwise.
  /// This is an alternative to the throwing constructor for error handling without exceptions.
  /// @param _str The string to parse in the format specified by the template parameter
  /// @return Result containing the Timestamp if parsing succeeded, or an error otherwise
  static Result<Timestamp> from_string(const std::string& _str) {
    return from_string(_str.c_str());
  }

  /// Generic factory method for creating a Timestamp from a string.
  /// Returns a result containing the timestamp when successful or an Error otherwise.
  /// @param _str The string to parse (can be const char* or std::string)
  /// @return Result containing the Timestamp if parsing succeeded, or an error otherwise
  static Result<Timestamp> make(const auto& _str) noexcept {
    return from_string(_str);
  }

  /// Returns the underlying object.
  const std::tm& get() const noexcept { return tm_; }

  /// Returns the underlying object.
  std::tm& get() noexcept { return tm_; }

  /// Returns the underlying object.
  std::tm& operator*() noexcept { return tm_; }

  /// Returns the underlying object.
  const std::tm& operator*() const noexcept { return tm_; }

  /// Returns the underlying object.
  std::tm& operator()() noexcept { return tm_; }

  /// Returns the underlying object.
  const std::tm& operator()() const noexcept { return tm_; }

  /// Returns the underlying object.
  std::tm& value() noexcept { return tm_; }

  /// Returns the underlying object.
  const std::tm& value() const noexcept { return tm_; }

  /// Returns the underlying std::tm structure for reflection/serialization.
  /// Formats the timestamp according to the format string and returns it as a string.
  /// This method is called by the serialization framework.
  /// @return The formatted timestamp string
  ReflectionType reflection() const {
    char outstr[200];
    strftime(outstr, 200, format_.str().c_str(), &tm_);
    return std::string(outstr);
  }

  /// Expresses the underlying timestamp as a formatted string.
  /// Uses the format specified in the template parameter.
  /// @return The formatted timestamp string
  std::string str() const { return reflection(); }

  /// Accessor to the underlying std::tm structure.
  /// @return Reference to the internal std::tm
  std::tm& tm() { return tm_; }

  /// Accessor to the underlying std::tm structure (const).
  /// @return Const reference to the internal std::tm
  const std::tm& tm() const { return tm_; }

  /// Converts the timestamp to a time_t value (seconds since Unix epoch in UTC).
  /// @return The time_t representation of this timestamp
  time_t to_time_t() const {
    auto tm = tm_;
#if defined(_MSC_VER) || defined(__MINGW32__)
    return _mkgmtime(&tm);
#else
    return static_cast<time_t>(timegm(&tm) - tm_.tm_gmtoff);
#endif
  }

 private:
#if defined(_MSC_VER) || defined(__MINGW32__)
  // This workaround is necessary, because strptime is not available on Windows.
  char* strptime(const char* _s, const char* _f, std::tm* _tm) {
    std::istringstream input(_s);
    input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
    input >> std::get_time(_tm, _f);
    if (input.fail()) {
      return NULL;
    }
    return (char*)(_s + input.tellg());
  }
#endif

 private:
  /// The underlying time stamp.
  std::tm tm_;
};

}  // namespace rfl

#endif
