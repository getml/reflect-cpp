#ifndef RFL_AVRO_TOSCHEMA_HPP_
#define RFL_AVRO_TOSCHEMA_HPP_

#include <map>
#include <string>
#include <type_traits>

#include "../Literal.hpp"
#include "../Processors.hpp"
#include "../Variant.hpp"
#include "../json.hpp"
#include "../parsing/schema/Type.hpp"
#include "../parsing/schema/ValidationType.hpp"
#include "../parsing/schema/make.hpp"
#include "Reader.hpp"
#include "Schema.hpp"
#include "Writer.hpp"
#include "schema/Type.hpp"

namespace rfl::avro {

std::string to_json_representation(
    const parsing::schema::Definition& internal_schema);

/// This ensures that the schema is only generated once.
template <class T, class... Ps>
struct SchemaHolder {
  static SchemaHolder<T, Ps...> make() noexcept {
    const auto internal_schema =
        parsing::schema::make<Reader, Writer, T,
                              Processors<SnakeCaseToCamelCase, Ps...>>();
    const auto json_str = to_json_representation(internal_schema);
    return SchemaHolder<T, Ps...>{Schema<T>::from_json(json_str)};
  }

  rfl::Result<Schema<T>> schema_;
};

template <class T, class... Ps>
static const SchemaHolder<T, Ps...> schema_holder =
    SchemaHolder<T, Ps...>::make();

/// Returns the Avro schema for a class.
template <class T, class... Ps>
Schema<T> to_schema() {
  return schema_holder<T, Ps...>.schema_.value();
}
}  // namespace rfl::avro

#endif
