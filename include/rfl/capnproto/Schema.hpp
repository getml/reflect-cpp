#ifndef RFL_CAPNPROTO_SCHEMA_HPP_
#define RFL_CAPNPROTO_SCHEMA_HPP_

#include <capnp/schema-parser.h>

#include <type_traits>

#include "../Ref.hpp"
#include "SchemaImpl.hpp"

namespace rfl::capnproto {

template <class T>
class Schema {
 public:
  using Type = std::remove_cvref_t<T>;

  Schema(const std::string& _str) : impl_(Ref<SchemaImpl>::make(_str)) {}

  static Result<Schema<T>> from_string(const std::string& _str) noexcept {
    try {
      return Schema<T>(_str);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  /// The string used to create this schema.
  const std::string& str() const { return impl_->str(); }

  /// The struct schema used to generate new values.
  const capnp::ParsedSchema& value() const { return impl_->value(); };

 private:
  /// We are using the "pimpl"-pattern
  Ref<SchemaImpl> impl_;
};

}  // namespace rfl::capnproto

#endif
