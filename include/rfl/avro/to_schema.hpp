#ifndef RFL_AVRO_TOSCHEMA_HPP_
#define RFL_AVRO_TOSCHEMA_HPP_

#include <string>

#include "../Processors.hpp"
#include "../common.hpp"
#include "../parsing/schema/make.hpp"
#include "Reader.hpp"
#include "Schema.hpp"
#include "Writer.hpp"

namespace rfl::avro {

RFL_API std::string to_json_representation(
    const parsing::schema::Definition& internal_schema);

/// Returns the Avro schema for a class.
template <class T, class... Ps>
Schema<T> to_schema() noexcept {
  const auto internal_schema =
      parsing::schema::make<Reader, Writer, T, Processors<Ps...>>();
  const auto json_str = to_json_representation(internal_schema);
  return std::move(Schema<T>::from_json(json_str).value());
}
}  // namespace rfl::avro

#endif
