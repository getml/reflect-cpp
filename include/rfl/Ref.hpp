#ifndef RFL_REF_HPP_
#define RFL_REF_HPP_

#include <memory>
#include <stdexcept>

#include "Result.hpp"

namespace rfl {

/// A smart pointer wrapper that is guaranteed to always contain a valid object with shared ownership.
/// The Ref class behaves very similarly to shared_ptr, but unlike shared_ptr,
/// it is 100% guaranteed to be filled at all times (unless the user
/// tries to access it after calling std::move or does something else that is
/// clearly bad practice).
/// @tparam T The type of object to contain
template <class T>
class Ref {
 public:
  /// The default way of creating new references is Ref<T>::make(...) or make_ref<T>(...).
  /// Constructs a new Ref with the given arguments forwarded to T's constructor.
  /// @tparam Args Types of constructor arguments
  /// @param _args Arguments to forward to T's constructor
  /// @return A new Ref containing the constructed object
  template <class... Args>
  static Ref<T> make(Args&&... _args) {
    return Ref<T>(std::make_shared<T>(std::forward<Args>(_args)...));
  }

  /// Creates a Ref from a shared_ptr (move version), returns an Error if the shared_ptr is null.
  /// @param _ptr The shared_ptr to convert to a Ref
  /// @return Result containing the Ref or an error if _ptr is nullptr
  static Result<Ref<T>> make(std::shared_ptr<T>&& _ptr) {
    if (!_ptr) {
      return error("std::shared_ptr was a nullptr.");
    }
    return Ref<T>(std::move(_ptr));
  }

  /// Creates a Ref from a shared_ptr (copy version), returns an Error if the shared_ptr is null.
  /// @param _ptr The shared_ptr to convert to a Ref
  /// @return Result containing the Ref or an error if _ptr is nullptr
  static Result<Ref<T>> make(const std::shared_ptr<T>& _ptr) {
    if (!_ptr) {
      return error("std::shared_ptr was a nullptr.");
    }
    return Ref<T>(_ptr);
  }

  /// Default constructor - creates a Ref containing a default-constructed T.
  Ref() : ptr_(std::make_shared<T>()) {}

  /// Copy constructor.
  /// @param _other The Ref to copy from
  Ref(const Ref<T>& _other) = default;

  /// Move constructor.
  /// @param _other The Ref to move from
  Ref(Ref<T>&& _other) = default;

  /// Copy constructor from a Ref with a different but compatible type.
  /// @tparam U Type convertible to T
  /// @param _other The Ref to copy from
  template <class U>
  Ref(const Ref<U>& _other) : ptr_(_other.ptr()) {}

  /// Move constructor from a Ref with a different but compatible type.
  /// @tparam U Type convertible to T
  /// @param _other The Ref to move from
  template <class U>
  Ref(Ref<U>&& _other) noexcept
      : ptr_(std::forward<std::shared_ptr<U>>(_other.ptr())) {}

  /// Destructor.
  ~Ref() = default;

  /// Returns a pointer to the underlying object.
  /// @return Raw pointer to the contained object
  T* get() const { return ptr_.get(); }

  /// Dereference operator - returns the underlying object.
  /// @return Reference to the contained object
  T& operator*() { return *ptr_; }

  /// Dereference operator (const) - returns the underlying object.
  /// @return Const reference to the contained object
  T& operator*() const { return *ptr_; }

  /// Arrow operator - provides access to the underlying object's members.
  /// @return Pointer to the contained object
  T* operator->() { return ptr_.get(); }

  /// Arrow operator (const) - provides access to the underlying object's members.
  /// @return Const pointer to the contained object
  T* operator->() const { return ptr_.get(); }

  /// Returns the underlying shared_ptr.
  /// @return Reference to the internal shared_ptr
  std::shared_ptr<T>& ptr() { return ptr_; }

  /// Returns the underlying shared_ptr (const).
  /// @return Const reference to the internal shared_ptr
  const std::shared_ptr<T>& ptr() const { return ptr_; }

  /// Copy assignment operator from Ref with different type.
  /// @tparam U Type convertible to T
  /// @param _other The Ref to copy from
  /// @return Reference to this Ref
  template <class U>
  Ref<T>& operator=(const Ref<U>& _other) {
    ptr_ = _other.ptr();
    return *this;
  }

  /// Move assignment operator from Ref with different type.
  /// @tparam U Type convertible to T
  /// @param _other The Ref to move from
  /// @return Reference to this Ref
  template <class U>
  Ref<T>& operator=(Ref<U>&& _other) noexcept {
    ptr_ = std::forward<std::shared_ptr<U>>(_other.ptr());
    return *this;
  }

  /// Move assignment operator.
  /// @param _other The Ref to move from
  /// @return Reference to this Ref
  Ref<T>& operator=(Ref<T>&& _other) noexcept = default;

  /// Copy assignment operator.
  /// @param _other The Ref to copy from
  /// @return Reference to this Ref
  Ref<T>& operator=(const Ref<T>& _other) = default;

 private:
  /// Only make() is allowed to use this constructor (move version).
  /// @param _ptr The shared_ptr to wrap
  explicit Ref(std::shared_ptr<T>&& _ptr) : ptr_(std::move(_ptr)) {}

  /// Only make() is allowed to use this constructor (copy version).
  /// @param _ptr The shared_ptr to wrap
  explicit Ref(const std::shared_ptr<T>& _ptr) : ptr_(_ptr) {}

 private:
  /// The underlying shared_ptr.
  std::shared_ptr<T> ptr_;
};

/// Generates a new Ref<T> by forwarding arguments to T's constructor.
/// @tparam T The type to wrap in a Ref
/// @tparam Args Types of constructor arguments
/// @param _args Arguments to forward to T's constructor
/// @return A new Ref<T>
template <class T, class... Args>
auto make_ref(Args&&... _args) {
  return Ref<T>::make(std::forward<Args>(_args)...);
}

/// Three-way comparison operator for Refs.
/// @tparam T1 Type of the first Ref
/// @tparam T2 Type of the second Ref
/// @param _t1 The first Ref to compare
/// @param _t2 The second Ref to compare
/// @return The ordering relationship between the underlying pointers
template <class T1, class T2>
inline auto operator<=>(const Ref<T1>& _t1, const Ref<T2>& _t2) {
  return _t1.ptr() <=> _t2.ptr();
}

/// Stream insertion operator for Ref.
/// @tparam CharT Character type
/// @tparam Traits Character traits
/// @tparam T Type contained in the Ref
/// @param _os The output stream
/// @param _b The Ref to output
/// @return The output stream
template <class CharT, class Traits, class T>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& _os, const Ref<T>& _b) {
  _os << _b.get();
  return _os;
}

}  // namespace rfl

namespace std {

/// Specialization of std::hash for Ref.
/// Allows Ref to be used in unordered containers.
/// @tparam T The type contained in the Ref
template <class T>
struct hash<rfl::Ref<T>> {
  /// Computes hash of the Ref by hashing its underlying shared_ptr.
  /// @param _r The Ref to hash
  /// @return The hash value
  size_t operator()(const rfl::Ref<T>& _r) const {
    return std::hash<std::shared_ptr<T>>{}(_r.ptr());
  }
};

/// Specialization of std::swap for Ref.
/// @tparam T The type contained in the Ref
/// @param _r1 The first Ref to swap
/// @param _r2 The second Ref to swap
template <class T>
inline void swap(rfl::Ref<T>& _r1, rfl::Ref<T>& _r2) {
  return swap(_r1.ptr(), _r2.ptr());
}

}  // namespace std

#endif  // RFL_REF_HPP_
