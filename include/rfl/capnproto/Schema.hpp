#ifndef RFL_CAPNPROTO_SCHEMA_HPP_
#define RFL_CAPNPROTO_SCHEMA_HPP_

#include <capnp/schema.h>

#include <type_traits>

#include "../Ref.hpp"
#include "SchemaImpl.hpp"

namespace rfl::capnproto {

template <class T>
class Schema {
 public:
  using Type = std::remove_cvref_t<T>;

  Schema(const std::string& _str) : impl_(Ref<SchemaImpl>::make(_str)) {}

  static Result<Schema<T>> from_json(const std::string& _json_str) noexcept {
    try {
      return Schema<T>(_json_str);
    } catch (std::exception& e) {
      return Error(e.what());
    }
  }

  /// The JSON string used to create this schema.
  const std::string& str() const { return impl_->str(); }

  /// The interface used to generate new values.
  const capnp::StructSchema& value() const { return impl_->value(); };

 private:
 private:
  /// We are using the "pimpl"-pattern
  Ref<SchemaImpl> impl_;
};

}  // namespace rfl::capnproto

#endif
