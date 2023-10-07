#ifndef RFL_REF_HPP_
#define RFL_REF_HPP_

#include <memory>
#include <stdexcept>

namespace rfl {

/// The Ref class behaves very similarly to the shared_ptr, but unlike the
/// shared_ptr, it is 100% guaranteed to be filled at all times.
template <class T>
class Ref {
   public:
    /// The default way of creating new references is
    /// Ref<T>::make(...) or make_ref<T>(...).
    template <class... Args>
    static Ref<T> make(Args&&... _args) {
        return Ref<T>(std::make_shared<T>(std::forward<Args>(_args)...));
    }

    /// Wrapper around a shared_ptr, leads to a runtime error,
    /// if the shared_ptr is not allocated.
    explicit Ref(const std::shared_ptr<T>& _ptr) {
        assert_true(_ptr);
        if (!_ptr) {
            throw std::runtime_error(
                "Could not create Ref from shared_ptr, because shared_ptr was "
                "not "
                "set.");
        }
        ptr_ = _ptr;
    }

    template <class Y>
    Ref(const Ref<Y>& _other) : ptr_(_other.ptr()) {}

    ~Ref() = default;

    /// Returns a pointer to the underlying object
    inline T* get() const { return ptr_.get(); }

    /// Returns the underlying object.
    inline T& operator*() const { return *ptr_; }

    /// Returns the underlying object.
    inline T* operator->() const { return ptr_.get(); }

    /// Returns the underlying shared_ptr
    inline const std::shared_ptr<T>& ptr() const { return ptr_; }

    /// Copy assignment operator.
    template <class Y>
    Ref<T>& operator=(const Ref<Y>& _other) {
        ptr_ = _other.ptr();
        return *this;
    }

   private:
    /// Only make is allowed to use this constructor.
    explicit Ref(std::shared_ptr<T>&& _ptr) : ptr_(std::move(_ptr)) {}

   private:
    /// The underlying shared_ptr_
    std::shared_ptr<T> ptr_;
};

/// Generates a new Ref<T>.
template <class T, class... Args>
Ref<T> make_ref(Args&&... _args) {
    return Ref<T>::make(std::forward<Args>(_args)...);
}

}  // namespace rfl

#endif  // RFL_REF_HPP_

