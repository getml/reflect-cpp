#ifndef RFL_PARSING_PARSER_TIME_POINT_HPP_
#define RFL_PARSING_PARSER_TIME_POINT_HPP_

#include <chrono>
#include <ctime>
#include <map>
#include <sstream>
#include <string>

#include "../Result.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class Clock, class Duration, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::chrono::time_point<Clock, Duration>>
struct Parser<R, W, std::chrono::time_point<Clock, Duration>, ProcessorsType> {
 public:
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  using TimePointType = std::chrono::time_point<Clock, Duration>;

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
        std::chrono::time_point_cast<std::chrono::microseconds>(_tp);
    const auto epoch = sys_time.time_since_epoch();
    const auto secs = std::chrono::duration_cast<std::chrono::seconds>(epoch);
    const auto usecs =
        std::chrono::duration_cast<std::chrono::microseconds>(epoch - secs);

    auto t = static_cast<std::time_t>(secs.count());
    std::tm tm{};
#if defined(_MSC_VER) || defined(__MINGW32__)
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif

    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &tm);

    const auto us = usecs.count();
    if (us != 0) {
      char frac[16];
      // Write microseconds, then strip trailing zeros.
      snprintf(frac, sizeof(frac), ".%06lld",
               static_cast<long long>(us < 0 ? -us : us));
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
        // Pad to microseconds (6 digits).
        while (digits < 6) {
          frac *= 10;
          ++digits;
        }
        // Truncate beyond microseconds.
        while (digits > 6) {
          frac /= 10;
          --digits;
        }
        tp += std::chrono::microseconds(frac);
      }

      return std::chrono::time_point_cast<Duration>(tp);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  static const char* parse_datetime(const char* _str, std::tm* _tm) {
#if defined(_MSC_VER) || defined(__MINGW32__)
    std::istringstream input(_str);
    input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
    input >> std::get_time(_tm, "%Y-%m-%dT%H:%M:%S");
    if (input.fail()) {
      return nullptr;
    }
    return _str + static_cast<std::ptrdiff_t>(input.tellg());
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
