#ifndef RFL_CAPNPROTO_NATIVEHANDLE_HPP_
#define RFL_CAPNPROTO_NATIVEHANDLE_HPP_

#ifndef RFL_CAPNPROTO_MODULE_INTERFACE
#include <capnp/dynamic.h>
#else
namespace capnp {
struct DynamicValue;
struct DynamicStruct;
struct DynamicList;
}  // namespace capnp
#endif
#include <memory>
#include <type_traits>
#include <utility>

#include "../common.hpp"

namespace rfl::capnproto::detail {
enum class NativeKind { value, object, list };

// The native view is allocated and destroyed in an ordinary translation unit.
// Native access is dependent, so importing read/write does not load KJ's types.
template <class Native, bool IsBuilder>
class RFL_API NativeHandle {
 public:
  NativeHandle() = default;

  template <class T>
    requires(!IsBuilder && requires { typename T::Reads; })
  NativeHandle(const T& _value)
      : impl_(make(&_value, kind<typename T::Reads>())) {}

  template <class T>
    requires(IsBuilder && requires { typename T::Builds; })
  NativeHandle(T _value) : impl_(make(&_value, kind<typename T::Builds>())) {}

  template <class N = Native>
  decltype(auto) native() const {
    if constexpr (IsBuilder)
      return *static_cast<typename N::Builder*>(native_ptr());
    else
      return *static_cast<const typename N::Reader*>(native_ptr());
  }

  // Retain native access for custom constructors which include Cap'n Proto.
  template <class T, class N = Native>
  auto as() const {
    return native<N>().template as<T>();
  }
  template <class N = Native>
  auto getType() const {
    return native<N>().getType();
  }
  template <class N = Native>
  auto getSchema() const {
    return native<N>().getSchema();
  }
  template <class... Args, class N = Native>
  auto get(Args&&... _args) const {
    return native<N>().get(std::forward<Args>(_args)...);
  }
  template <class... Args, class N = Native>
  auto set(Args&&... _args) const {
    return native<N>().set(std::forward<Args>(_args)...);
  }
  template <class... Args, class N = Native>
  auto init(Args&&... _args) const {
    return native<N>().init(std::forward<Args>(_args)...);
  }
  template <class T, class N = Native>
  auto operator[](T&& _index) const {
    return native<N>()[std::forward<T>(_index)];
  }

 private:
  template <class T>
  static constexpr NativeKind kind() {
    static_assert(std::is_same_v<Native, capnp::DynamicValue> ||
                  std::is_same_v<Native, T>);
    if constexpr (std::is_same_v<T, capnp::DynamicValue>)
      return NativeKind::value;
    else if constexpr (std::is_same_v<T, capnp::DynamicStruct>)
      return NativeKind::object;
    else {
      static_assert(std::is_same_v<T, capnp::DynamicList>);
      return NativeKind::list;
    }
  }
  struct Impl;
  static std::shared_ptr<Impl> make(const void*, NativeKind);
  void* native_ptr() const;
  std::shared_ptr<Impl> impl_;
};

using ValueReader = NativeHandle<capnp::DynamicValue, false>;
using StructReader = NativeHandle<capnp::DynamicStruct, false>;
using ListReader = NativeHandle<capnp::DynamicList, false>;
using ValueBuilder = NativeHandle<capnp::DynamicValue, true>;
using StructBuilder = NativeHandle<capnp::DynamicStruct, true>;
using ListBuilder = NativeHandle<capnp::DynamicList, true>;
}  // namespace rfl::capnproto::detail
#endif
