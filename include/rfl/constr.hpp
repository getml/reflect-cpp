#ifndef RFL_UUID_HPP_
#define RFL_UUID_HPP_

#include <ctime>
#include <iterator>
#include <stdexcept>
#include <string>
#include <regex>

#include "internal/StringLiteral.hpp"
#include "rfl/Literal.hpp"
#include "rfl/Result.hpp"


namespace rfl {

//template <size_t N>
//consteval const auto _get_pattern(const internal::StringLiteral<N> _v) {
//      if constexpr (_v == internal::StringLiteral{"1"}) {
//         return "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-1[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$";
//      } else if constexpr (_v == internal::StringLiteral{"2"}) {
//         return "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-2[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$";
//    } else if constexpr (_v == internal::StringLiteral{"3"}) {
//        return "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-3[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$";
//      }
//      else if constexpr (_v == internal::StringLiteral{"4"}) {
//        return "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-4[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$";
//    } else {
//       static_assert("UUID Version must be one the following: 1, 2, 3, 4.");
//    }
//}

template <internal::StringLiteral _pattern>
class ConStr {
  using ReflectionType = std::string;
  
   constexpr static const internal::StringLiteral pattern_ = _pattern; 

  //constexpr static const char* version_pattern = _get_pattern(version_);
  
  public:
     ConStr(const char* _str) {
        std::regex regex_pattern(pattern_.str());
        if (!std::regex_match(std::string(_str), regex_pattern)) {
            throw std::runtime_error(
        "String '" + std::string(_str) + "' did not match format " + pattern_.str() + " format");
        }
        value_ = _str;
     }

    ConStr(const std::string& _str) : ConStr(_str.c_str()) {};
    ~ConStr() = default;

    std::string value_;

    ReflectionType reflection() const {
      return value_;
    }
     
    /// Returns a result containing the timestamp when successful or an Error
    /// otherwise.
    static Result<ConStr> from_string(const char* _str) noexcept {
        try {
            return ConStr(_str);
        } catch (std::exception& e) {
            return Error(e.what());
        }
    }

    /// Returns a result containing the timestamp when successful or an Error
    /// otherwise.
    static Result<ConStr> from_string(const std::string& _str) {
        return from_string(_str.c_str());
    }
    /// Expresses the underlying timestamp as a string.
    std::string str() const { return reflection; }

};
/// UUID
using UUIDv1 = ConStr<R"(^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-1[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$)">;
//using UUIDv2 = ConStr<"^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-2[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$">;
//using UUIDv3 = ConStr<"^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-3[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$">;
//using UUIDv4 = ConStr<"^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-4[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$">;

/// email
//using email = ConStr<"^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-4[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$">;
}


#endif
