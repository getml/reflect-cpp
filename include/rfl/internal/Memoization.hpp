#ifndef RFL_INTERNAL_MEMOIZATION_HPP_
#define RFL_INTERNAL_MEMOIZATION_HPP_

#include <atomic>
#include <mutex>

namespace rfl {
namespace internal {

/// For a thread-safe memoization pattern.
template <class T>
class Memoization {
 public:
  Memoization() { flag_.clear(); }

  ~Memoization() = default;

 public:
  /// Returns the underlying value.
  template <class F>
  const T& value(const F& _f) {
    if (flag_.test()) {
      return value_;
    }

    std::lock_guard<std::mutex> guard(mtx_);

    if (flag_.test()) {
      return value_;
    }

    _f(&value_);

    flag_.test_and_set();

    return value_;
  }

 private:
  /// Signifies whether t_ has been set.
  std::atomic_flag flag_;

  /// A mutex, only needed for writing.
  std::mutex mtx_;

  /// The type to be initialized.
  T value_;
};

}  // namespace internal
}  // namespace rfl

#endif
