#include "rfl/capnproto/Reader.hpp"

#include "rfl/parsing/schemaful/IsSchemafulReader.hpp"

namespace rfl::capnproto {

static_assert(parsing::schemaful::IsSchemafulReader<Reader>,
              "This must be a schemaful reader.");

std::optional<std::pair<capnp::StructSchema::Field, size_t>>
Reader::identify_discriminant(const InputUnionType& _union) const noexcept {
  size_t ix = 0;
  for (auto field : _union.val_.getSchema().getFields()) {
    if (_union.val_.has(field)) {
      return std::make_pair(field, ix);
    }
    ++ix;
  }
  return std::optional<std::pair<capnp::StructSchema::Field, size_t>>();
}

}  // namespace rfl::capnproto
