#ifndef RFL_BOX_HPP_
#define RFL_BOX_HPP_

#include <memory>
#include <stdexcept>

#include "Result.hpp"

namespace rfl {

/// The Box class behaves very similarly to the unique_ptr, but unlike the
/// unique_ptr, it is 100% guaranteed to be filled at all times (unless the user
/// tries to access it after calling std::move does something else that is
/// clearly bad practice).
template <class T>
class Box {
 public:
  /// The only way of creating new boxes is
  /// Box<T>::make(...).
  template <class... Args>
  static Box<T> make(Args&&... _args) {
    return Box<T>(std::make_unique<T>(std::forward<Args>(_args)...));
  }

  /// You can generate them from unique_ptrs as well, in which case it will
  /// return an Error, if the unique_ptr is not set.
  static Result<Box<T>> make(std::unique_ptr<T>&& _ptr) {
    if (!_ptr) {
      return Error("std::unique_ptr was a nullptr.");
    }
    return Box<T>(std::move(_ptr));
  }

  Box() : ptr_(std::make_unique<T>()) {}

  Box(const Box<T>& _other) = delete;

  Box(Box<T>&& _other) = default;

  template <class U>
  Box(Box<U>&& _other) noexcept
      : ptr_(std::forward<std::unique_ptr<U>>(_other.ptr())) {}

  ~Box() = default;

  /// Returns a pointer to the underlying object
  T* get() const { return ptr_.get(); }

  /// Copy assignment operator
  Box<T>& operator=(const Box<T>& _other) = delete;

  /// Move assignment operator
  Box<T>& operator=(Box<T>&& _other) noexcept = default;

  /// Move assignment operator
  template <class U>
  Box<T>& operator=(Box<U>&& _other) noexcept {
    ptr_ = std::forward<std::unique_ptr<U>>(_other.ptr());
    return *this;
  }

  /// Returns the underlying object.
  T& operator*() { return *ptr_; }

  /// Returns the underlying object.
  T& operator*() const { return *ptr_; }

  /// Returns the underlying object.
  T* operator->() { return ptr_.get(); }

  /// Returns the underlying object.
  T* operator->() const { return ptr_.get(); }

  /// Returns the underlying unique_ptr
  std::unique_ptr<T>& ptr() { return ptr_; }

  /// Returns the underlying unique_ptr
  const std::unique_ptr<T>& ptr() const { return ptr_; }

 private:
  /// Only make is allowed to use this constructor.
  explicit Box(std::unique_ptr<T>&& _ptr) : ptr_(std::move(_ptr)) {}

 private:
  /// The underlying unique_ptr_
  std::unique_ptr<T> ptr_;
};

/// Generates a new Ref<T>.
template <class T, class... Args>
auto make_box(Args&&... _args) {
  return Box<T>::make(std::forward<Args>(_args)...);
}

template <class T1, class T2>
inline auto operator<=>(const Box<T1>& _b1, const Box<T2>& _b2) {
  return _b1.ptr() <=> _b2.ptr();
}

template <class CharT, class Traits, class T>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& _os, const Box<T>& _b) {
  _os << _b.get();
  return _os;
}

}  // namespace rfl

namespace std {

template <class T>
struct hash<rfl::Box<T>> {
  size_t operator()(const rfl::Box<T>& _b) const {
    return hash<unique_ptr<T>>()(_b.ptr());
  }
};

template <class T>
inline void swap(rfl::Box<T>& _b1, rfl::Box<T>& _b2) {
  return swap(_b1.ptr(), _b2.ptr());
}

}  // namespace std

#endif
