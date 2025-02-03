#ifndef RFL_AVRO_SCHEMA_HPP_
#define RFL_AVRO_SCHEMA_HPP_

#include <type_traits>

#include "../Ref.hpp"
#include "SchemaImpl.hpp"

namespace rfl::avro {

template <class T>
class Schema {
 public:
  using Type = std::remove_cvref_t<T>;

  Schema(const std::string& _json_str)
      : impl_(Ref<SchemaImpl>::make(_json_str)) {}

  static Result<Schema<T>> from_json(const std::string& _json_str) noexcept {
    try {
      return Schema<T>(_json_str);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  /// The JSON string used to create this schema.
  const std::string& json_str() const { return impl_->json_str(); }

  /// The JSON string used to create this schema.
  const std::string& str() const { return impl_->json_str(); }

  /// The interface used to create new values.
  avro_value_iface_t* iface() const { return impl_->iface(); };

 private:
  /// We are using the "pimpl"-pattern
  Ref<SchemaImpl> impl_;
};

}  // namespace rfl::avro

#endif
