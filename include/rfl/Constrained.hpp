#ifndef RFL_CONSTRAINED_HPP_
#define RFL_CONSTRAINED_HPP_

#include "rfl/Result.hpp"

namespace rfl {

/// A constrained type is a numeric value that is constrained is some way, such
/// as it must be greater than a specific threshold known at compile time.
template <class T, class C>
class Constrained {
   public:
    using ReflectionType = T;

    Constrained(T _value) : value_(C::check(_value).value()) {}

    ~Constrained() = default;

    /// Exposes the underlying value.
    T& value() { return value_; }

    /// Exposes the underlying value.
    T value() const { return value_; }

    /// Necessary for the serialization to work.
    T reflection() const { return value_; }

   private:
    /// The underlying value.
    T value_;
};

}  // namespace rfl

#endif
