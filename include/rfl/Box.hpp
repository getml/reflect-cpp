#ifndef RFL_BOX_HPP_
#define RFL_BOX_HPP_

#include <memory>
#include <stdexcept>

namespace rfl {

/// The Box class behaves very similarly to the unique_ptr, but unlike the
/// unique_ptr, it is 100% guaranteed to be filled at all times.
template <class T>
class Box {
   public:
    /// The only way of creating new boxes is
    /// Box<T>::make(...).
    template <class... Args>
    static Box<T> make(Args&&... _args) {
        return Box<T>(std::make_unique<T>(std::forward<Args>(_args)...));
    }

    Box(Box<T>&& _other) : ptr_(std::move(_other.ptr_)) {}

    ~Box() = default;

    /// Returns a pointer to the underlying object
    inline T* get() const { return ptr_.get(); }

    /// Returns the underlying object.
    inline T& operator*() const { return *ptr_; }

    /// Returns the underlying object.
    inline T* operator->() const { return ptr_.get(); }

    /// Returns the underlying unique_ptr
    inline const std::unique_ptr<T>& ptr() const { return ptr_; }

   private:
    /// Only make is allowed to use this constructor.
    explicit Box(std::unique_ptr<T>&& _ptr) : ptr_(std::move(_ptr)) {}

   private:
    /// The underlying unique_ptr_
    std::unique_ptr<T> ptr_;
};

/// Generates a new Ref<T>.
template <class T, class... Args>
Box<T> make_box(Args&&... _args) {
    return Box<T>::make(std::forward<Args>(_args)...);
}

}  // namespace rfl

#endif
