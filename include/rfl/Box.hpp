#ifndef RFL_BOX_HPP_
#define RFL_BOX_HPP_

#include <memory>
#include <stdexcept>

#include "Result.hpp"

namespace rfl {


enum class Copyability {
    COPYABLE,
    NON_COPYABLE
};

/// The Box class behaves very similarly to the unique_ptr, but unlike the
/// unique_ptr, it is 100% guaranteed to be filled at all times (unless the user
/// tries to access it after calling std::move does something else that is
/// clearly bad practice).
///
/// By default Box behaves like a unique_ptr in relation to copying, but it can be
/// configured to add copy constructor and assignment operators that call the
/// same function of the contained type T.

template <class T, Copyability C = Copyability::NON_COPYABLE>
class Box {
 public:
  /// The only way of creating new boxes is
  /// Box<T>::make(...).
  template <class... Args>
  static Box<T, C> make(Args&&... _args) {
    return Box<T, C>(std::make_unique<T>(std::forward<Args>(_args)...));
  }

  /// You can generate them from unique_ptrs as well, in which case it will
  /// return an Error, if the unique_ptr is not set.
  static Result<Box<T, C>> make(std::unique_ptr<T>&& _ptr) {
    if (!_ptr) {
      return error("std::unique_ptr was a nullptr.");
    }
    return Box<T, C>(std::move(_ptr));
  }

  Box() : ptr_(std::make_unique<T>()) {}

  /// Copy constructor if copyable
  Box(const Box<T, C>& _other) requires (C == Copyability::COPYABLE)
  {
    ptr_ = std::make_unique<T>(*_other);
  }

  /// Copy constructor if not copyable
  Box(const Box<T, C>& _other) requires (C == Copyability::NON_COPYABLE) = delete;

  Box(Box<T, C>&& _other) = default;

  template <class U, Copyability C2>
  Box(Box<U, C2>&& _other) noexcept
      : ptr_(std::forward<std::unique_ptr<U>>(_other.ptr())) {}

  ~Box() = default;

  /// Returns a pointer to the underlying object
  T* get() const { return ptr_.get(); }

  /// Copy assignment operator if copyable
  Box<T, C>& operator=(const Box<T>& other) requires (C == Copyability::COPYABLE) {
    if(this != &other) {
      ptr_ = std::make_unique<T>(*other);
    }
    return *this;
  }

  /// Copy assignment operator if not copyable
  Box<T, C>& operator=(const Box<T>& _other) requires (C == Copyability::NON_COPYABLE)  = delete;

  /// Move assignment operator
  Box<T, C>& operator=(Box<T, C>&& _other) noexcept = default;

  /// Move assignment operator
  template <class U>
  Box<T, C>& operator=(Box<U>&& _other) noexcept {
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

/// Template specialization for a box that is copyable.
template<typename T>
using CopyableBox = Box<T, Copyability::COPYABLE>;

template <class T, class... Args>
auto make_copyable_box(Args&&... _args) {
    return CopyableBox<T>::make(std::forward<Args>(_args)...);
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
