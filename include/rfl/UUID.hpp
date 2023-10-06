#ifndef RFL_TIMESTAMP_HPP_
#define RFL_TIMESTAMP_HPP_

#include <ctime>
#include <iterator>
#include <stdexcept>
#include <string>
#include <regex>

#include "internal/StringLiteral.hpp"
#include "rfl/Literal.hpp"
#include "rfl/Result.hpp"


namespace rfl {


template <internal::StringLiteral _version>
class UUID {
  using ReflectionType = std::string;
  
   constexpr static const internal::StringLiteral version_ = _version; 
   static constexpr const char* version_pattern = [] {
      if constexpr (version_ == "1") {
         return "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-1[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$";
      } else if constexpr (version_ == "2") {
         return "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-2[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$";
    } else if constexpr (version_ == "3") {
        return "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-3[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$";
      }
      else if constexpr (version_ == "3") {
        return "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-4[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$";
    } else {
       static_assert("UUID Version must be one the following: 1, 2, 3, 4.");
    }
  }();
  
  public:
     UUID(const char* _str) {
        std::regex uuid_regex(version_pattern);
        if (!std::regex_match(uuid_regex, std::string(_str))) {
            throw std::runtime_error(
        "String '" + std::string(_str) + "' did not match format the UUIDv" + std::string(version_) + " format");
        }
        uuid = _str; 
     }

    UUID(const std::string& _str) : UUID(_str.c_str()) {};

    ReflectionType reflection() const {
      return std::string(uuid);
    }
    
    //std::string str() const {return reflection; }

  private:
    //fixed size char instead of string
  //
    std::string uuid;
    //char uuid[36];
};
}
