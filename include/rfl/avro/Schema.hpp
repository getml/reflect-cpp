#ifndef RFL_AVRO_SCHEMA_HPP_
#define RFL_AVRO_SCHEMA_HPP_

#include <type_traits>

#include "SchemaImpl.hpp"

namespace rfl::avro {

template <class T>
class Schema : public SchemaImpl {
 public:
  using Type = std::remove_cvref_t<T>;

  Schema(const std::string& _json_str) : SchemaImpl(_json_str) {}

  static Result<Schema<T>> from_json(const std::string& _json_str) noexcept {
    try {
      return Schema<T>(_json_str);
    } catch (std::exception& e) {
      return Error(e.what());
    }
  }
};

}  // namespace rfl::avro

#endif
