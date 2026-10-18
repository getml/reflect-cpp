#include "rfl/capnproto/NativeHandle.hpp"

#include <stdexcept>

namespace rfl::capnproto::detail {
template <class Native, bool IsBuilder>
struct NativeHandle<Native, IsBuilder>::Impl {
  using Value = std::conditional_t<IsBuilder, typename Native::Builder,
                                   typename Native::Reader>;
  explicit Impl(Value _value) : value(std::move(_value)) {}
  Value value;
};

template <class Native, bool IsBuilder>
std::shared_ptr<typename NativeHandle<Native, IsBuilder>::Impl>
NativeHandle<Native, IsBuilder>::make(const void* _value, NativeKind _kind) {
  using Value = typename Impl::Value;
  if constexpr (IsBuilder) {
    // Builder construction passes a non-const local copy of the native view.
    // DynamicValue::Builder's copy constructor deliberately takes Builder&.
    if constexpr (std::is_same_v<Native, capnp::DynamicValue>) {
      switch (_kind) {
        case NativeKind::value:
          return std::make_shared<Impl>(
              *static_cast<Value*>(const_cast<void*>(_value)));
        case NativeKind::object:
          return std::make_shared<Impl>(
              Value(*static_cast<capnp::DynamicStruct::Builder*>(
                  const_cast<void*>(_value))));
        case NativeKind::list:
          return std::make_shared<Impl>(
              Value(*static_cast<capnp::DynamicList::Builder*>(
                  const_cast<void*>(_value))));
      }
    } else
      return std::make_shared<Impl>(
          *static_cast<Value*>(const_cast<void*>(_value)));
  } else if constexpr (std::is_same_v<Native, capnp::DynamicValue>) {
    switch (_kind) {
      case NativeKind::value:
        return std::make_shared<Impl>(*static_cast<const Value*>(_value));
      case NativeKind::object:
        return std::make_shared<Impl>(
            Value(*static_cast<const capnp::DynamicStruct::Reader*>(_value)));
      case NativeKind::list:
        return std::make_shared<Impl>(
            Value(*static_cast<const capnp::DynamicList::Reader*>(_value)));
    }
  } else
    return std::make_shared<Impl>(*static_cast<const Value*>(_value));
  throw std::runtime_error("Invalid Cap'n Proto native view.");
}
template <class Native, bool IsBuilder>
void* NativeHandle<Native, IsBuilder>::native_ptr() const {
  if (!impl_) throw std::logic_error("Empty Cap'n Proto native handle.");
  return &impl_->value;
}

template class NativeHandle<capnp::DynamicValue, false>;
template class NativeHandle<capnp::DynamicStruct, false>;
template class NativeHandle<capnp::DynamicList, false>;
template class NativeHandle<capnp::DynamicValue, true>;
template class NativeHandle<capnp::DynamicStruct, true>;
template class NativeHandle<capnp::DynamicList, true>;
}  // namespace rfl::capnproto::detail
