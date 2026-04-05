#ifndef RFL_PARSING_PARSER_TIME_POINT_HPP_
#define RFL_PARSING_PARSER_TIME_POINT_HPP_

#include <chrono>
#include <cstring>
#include <ctime>
#include <map>
#include <optional>
#include <sstream>
#include <string>

#include "../Result.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class Duration, class ProcessorsType>
  requires AreReaderAndWriter<
      R, W, std::chrono::time_point<std::chrono::system_clock, Duration>>
struct Parser<R, W,
              std::chrono::time_point<std::chrono::system_clock, Duration>,
              ProcessorsType> {
 public:
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  using TimePointType =
      std::chrono::time_point<std::chrono::system_clock, Duration>;

  static Result<TimePointType> read(const R& _r,
                                    const InputVarType& _var) noexcept {
    return Parser<R, W, std::string, ProcessorsType>::read(_r, _var).and_then(
        from_string);
  }

  template <class P>
  static void write(const W& _w, const TimePointType& _tp, const P& _parent) {
    Parser<R, W, std::string, ProcessorsType>::write(_w, to_string(_tp),
                                                     _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::string, ProcessorsType>::to_schema(_definitions);
  }

 private:
  static std::string to_string(const TimePointType& _tp) {
    const auto sys_time =
        std::chrono::time_point_cast<std::chrono::nanoseconds>(_tp);
    const auto epoch = sys_time.time_since_epoch();
    const auto secs = std::chrono::duration_cast<std::chrono::seconds>(epoch);
    const auto nsecs =
        std::chrono::duration_cast<std::chrono::nanoseconds>(epoch - secs);

    auto t = static_cast<std::time_t>(secs.count());
    std::tm tm{};
#if defined(_MSC_VER) || defined(__MINGW32__)
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif

    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &tm);

    const auto ns = nsecs.count();
    if (ns != 0) {
      char frac[16];
      // Write nanoseconds, then strip trailing zeros.
      snprintf(frac, sizeof(frac), ".%09lld",
               static_cast<long long>(ns < 0 ? -ns : ns));
      auto len = strlen(frac);
      while (len > 1 && frac[len - 1] == '0') {
        --len;
      }
      frac[len] = '\0';
      return std::string(buf) + frac + "Z";
    }
    return std::string(buf) + "Z";
  }

  static Result<TimePointType> from_string(const std::string& _str) noexcept {
    try {
      std::tm tm{};
      const char* str = _str.c_str();
      const char* rest = parse_datetime(str, &tm);
      if (!rest) {
        return error("Could not parse time point from '" + _str + "'.");
      }

      auto t = to_time_t(tm);
      auto tp = std::chrono::system_clock::from_time_t(t);

      // Parse fractional seconds if present.
      if (*rest == '.') {
        ++rest;
        long long frac = 0;
        int digits = 0;
        while (*rest >= '0' && *rest <= '9' && digits < 9) {
          frac = frac * 10 + (*rest - '0');
          ++rest;
          ++digits;
        }
        // Pad to nanoseconds (9 digits).
        while (digits < 9) {
          frac *= 10;
          ++digits;
        }
        // Truncate beyond nanoseconds.
        while (digits > 9) {
          frac /= 10;
          --digits;
        }
        tp += std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::nanoseconds(frac));
      }

      // Parse timezone: 'Z', '+HH:MM', '-HH:MM', or end of string.
      if (*rest == '+' || *rest == '-') {
        const auto offset = parse_tz_offset(rest);
        if (!offset) {
          return error("Could not parse timezone offset from '" + _str + "'.");
        }
        tp -= *offset;
      } else if (*rest != 'Z' && *rest != '\0') {
        return error("Could not parse time point from '" + _str +
                     "': expected 'Z', timezone offset, or end of string.");
      }

      return std::chrono::time_point_cast<Duration>(tp);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  static bool is_digit(char c) { return c >= '0' && c <= '9'; }

  static int two_digits(const char* s) {
    return (s[0] - '0') * 10 + (s[1] - '0');
  }

  /// Parses a timezone offset like "+05:30" or "-08:00".
  /// Returns the offset as a chrono duration, or std::nullopt on failure.
  static std::optional<std::chrono::minutes> parse_tz_offset(const char* _str) {
    if (*_str != '+' && *_str != '-') {
      return std::nullopt;
    }
    const int sign = (*_str == '+') ? 1 : -1;
    ++_str;
    // Expect HH:MM or HHMM.
    if (!is_digit(_str[0]) || !is_digit(_str[1])) {
      return std::nullopt;
    }
    const int hours = two_digits(_str);
    _str += 2;
    if (*_str == ':') {
      ++_str;
    }
    if (!is_digit(_str[0]) || !is_digit(_str[1])) {
      return std::nullopt;
    }
    const int minutes = two_digits(_str);
    return std::chrono::minutes(sign * (hours * 60 + minutes));
  }

  static const char* parse_datetime(const char* _str, std::tm* _tm) {
#if defined(_MSC_VER) || defined(__MINGW32__)
    std::istringstream input(_str);
    input.imbue(std::locale::classic());
    input >> std::get_time(_tm, "%Y-%m-%dT%H:%M:%S");
    if (input.fail()) {
      return nullptr;
    }
    const auto pos = input.tellg();
    if (pos == std::streampos(-1)) {
      // Stream reached EOF after parsing — all input was consumed.
      return _str + std::strlen(_str);
    }
    return _str + static_cast<std::ptrdiff_t>(pos);
#else
    return strptime(_str, "%Y-%m-%dT%H:%M:%S", _tm);
#endif
  }

  static std::time_t to_time_t(std::tm& _tm) {
#if defined(_MSC_VER) || defined(__MINGW32__)
    return _mkgmtime(&_tm);
#else
    return timegm(&_tm);
#endif
  }
};

}  // namespace rfl::parsing

#endif
