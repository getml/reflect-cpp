#ifndef FLEXBUF_OUTPUTVALUE_HPP_
#define FLEXBUF_OUTPUTVALUE_HPP_

#include <flatbuffers/flexbuffers.h>

#include <optional>
#include <string>
#include <utility>

#include "../Ref.hpp"
#include "../always_false.hpp"
#include "OutputVar.hpp"

namespace rfl {
namespace flexbuf {

template <class T>
class OutputValue : public OutputVar {
 public:
  OutputValue(T _val) : val_(_val) {}

  ~OutputValue() = default;

  /// Inserts all elements into the builder.
  void insert(const std::optional<std::string>& _key,
              flexbuffers::Builder* _fbb) final {
    if constexpr (std::is_same<std::decay_t<T>, std::string>()) {
      if (_key) {
        _fbb->String(_key->c_str(), val_);
      } else {
        _fbb->String(val_);
      }
    } else if constexpr (std::is_same<std::decay_t<T>, bool>()) {
      if (_key) {
        _fbb->Bool(_key->c_str(), val_);
      } else {
        _fbb->Bool(val_);
      }
    } else if constexpr (std::is_floating_point<std::decay_t<T>>()) {
      if (_key) {
        _fbb->Double(_key->c_str(), val_);
      } else {
        _fbb->Double(val_);
      }
    } else if constexpr (std::is_integral<std::decay_t<T>>()) {
      if (_key) {
        _fbb->Int(_key->c_str(), val_);
      } else {
        _fbb->Int(val_);
      }
    } else {
      static_assert(always_false_v<T>, "Unsupported type");
    }
  };

  /// Whether this is null.
  bool is_null() const final { return false; }

 private:
  /// The underlying value.
  T val_;
};

}  // namespace flexbuf
}  // namespace rfl

#endif
