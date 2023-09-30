#ifndef RFL_INTERNAL_STRINGLITERAL_HPP_
#define RFL_INTERNAL_STRINGLITERAL_HPP_

#include <algorithm>
#include <string>
#include <string_view>

namespace rfl {
namespace internal {

/// Normal strings cannot be used as template
/// parameters, but this can. This is needed
/// for the parameters names in the NamedTuples.
template <size_t N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value_);
    }

    char value_[N];

    /// Returns the value as a string.
    std::string str() const { return std::string(value_); }
};

template <size_t N1, size_t N2>
constexpr inline bool operator==(const StringLiteral<N1>& _first,
                                 const StringLiteral<N2>& _second) {
    if constexpr (N1 != N2) {
        return false;
    }
    return std::string_view(_first.value_, N1) ==
           std::string_view(_second.value_, N2);
}

template <size_t N1, size_t N2>
constexpr inline bool operator!=(const StringLiteral<N1>& _first,
                                 const StringLiteral<N2>& _second) {
    return !(_first == _second);
}

}  // namespace internal
}  // namespace rfl

#endif
