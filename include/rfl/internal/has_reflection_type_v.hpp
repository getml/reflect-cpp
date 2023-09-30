#ifndef RFL_HASREFLECTIONTYPEV_HPP_
#define RFL_HASREFLECTIONTYPEV_HPP_

#include <cstdint>
#include <utility>

namespace rfl {
namespace internal {

template <class Wrapper>
class HasReflectionType {
   private:
    template <class U>
    static std::int64_t foo(...);

    template <class U>
    static std::int32_t foo(typename U::ReflectionType*);

   public:
    static constexpr bool value =
        sizeof(foo<Wrapper>(nullptr)) == sizeof(std::int32_t);
};

/// Utility parameter for named tuple parsing, can be used by the
/// parsers to determine whether a class or struct defines a type
/// called "ReflectionType".
template <typename Wrapper>
constexpr bool has_reflection_type_v = HasReflectionType<Wrapper>::value;

}  // namespace internal
}  // namespace rfl

#endif  // RFL_HASNAMEDTUPLETYPEV_HPP_
