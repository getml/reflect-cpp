#ifndef RFL_INTERNAL_STRINGLITERAL_HPP_
#define RFL_INTERNAL_STRINGLITERAL_HPP_

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace rfl {
namespace internal {

/// Normal strings cannot be used as template
/// parameters, but this can. This is needed
/// for the parameters names in the NamedTuples.
template <size_t N>
struct StringLiteral {
  constexpr StringLiteral(const auto... _chars) : arr_{_chars..., '\0'} {}

  constexpr StringLiteral(const std::array<char, N> _arr) : arr_(_arr) {}

  constexpr StringLiteral(const char (&_str)[N]) {
    std::copy_n(_str, N, std::data(arr_));
  }

  /// Returns the value as a string.
  std::string str() const { return std::string(string_view()); }

  /// Returns the value as a string.
  constexpr std::string_view string_view() const {
    return std::string_view(std::data(arr_), N - 1);
  }

  std::array<char, N> arr_{};
};

template <size_t N1, size_t N2>
constexpr inline bool operator==(const StringLiteral<N1>& _first,
                                 const StringLiteral<N2>& _second) {
  if constexpr (N1 != N2) {
    return false;
  }
  return _first.string_view() == _second.string_view();
}

template <size_t N1, size_t N2>
constexpr inline bool operator!=(const StringLiteral<N1>& _first,
                                 const StringLiteral<N2>& _second) {
  return !(_first == _second);
}

}  // namespace internal
}  // namespace rfl

#endif
