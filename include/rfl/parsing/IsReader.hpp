#ifndef RFL_PARSING_ISREADER_HPP_
#define RFL_PARSING_ISREADER_HPP_

#include <array>
#include <concepts>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>

#include "../Result.hpp"
#include "../internal/is_basic_type.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "SupportsTaggedUnions.hpp"
#include "schemaful/IsSchemafulReader.hpp"

namespace rfl {
namespace parsing {

template <class R>
struct MockArrayReader {
  std::optional<Error> read(typename R::InputVarType&) const {
    return std::nullopt;
  }
};

template <class R>
struct MockObjectReader {
  void read(const std::string_view&, typename R::InputVarType&) const {}
};

template <class R, class T>
concept IsReader =
    (SupportsTaggedUnions<R> || schemaful::IsSchemafulReader<R>) &&
    requires(R r, std::string name,
             std::function<std::int16_t(std::string_view)> fct,
             MockArrayReader<R> array_reader, MockObjectReader<R> object_reader,
             typename R::InputArrayType arr, typename R::InputObjectType obj,
             typename R::InputVarType var, size_t idx) {
      /// Any Reader needs to define the following:
      ///
      /// 1) An InputArrayType, which must be an array-like data structure.
      /// 2) An InputObjectType, which must contain key-value pairs.
      /// 3) An InputVarType, which must be able to represent either
      ///    InputArrayType, InputObjectType or a basic type (bool, integral,
      ///    floating point, std::string).
      /// 4) A static constexpr bool has_custom_constructor, that determines
      ///    whether the class in question as a custom constructor, which might
      ///    be called something like from_json_obj(...).

      /// Determines whether a variable is empty (the NULL type).
      { r.is_empty(var) } -> std::same_as<bool>;

      /// Iterates through an array and writes the contained vars into
      /// an array reader.
      { r.read_array(array_reader, arr) } -> std::same_as<std::optional<Error>>;

      /// Iterates through an object and writes the key-value pairs into an
      /// object reader. This is what we use to handle structs and named tuples,
      /// making it a very important function.
      {
        r.read_object(object_reader, obj)
      } -> std::same_as<std::optional<Error>>;

      /// Transforms var to a basic type (bool, integral,
      /// floating point, std::string)
      {
        r.template to_basic_type<internal::wrap_in_rfl_array_t<T>>(var)
      } -> std::same_as<rfl::Result<internal::wrap_in_rfl_array_t<T>>>;

      /// Casts var as an InputArrayType.
      {
        r.to_array(var)
      } -> std::same_as<rfl::Result<typename R::InputArrayType>>;

      /// Casts var as an InputObjectType.
      {
        r.to_object(var)
      } -> std::same_as<rfl::Result<typename R::InputObjectType>>;

      /// Uses the custom constructor, if it has been determined that T has one
      /// (see above).
      {
        r.template use_custom_constructor<internal::wrap_in_rfl_array_t<T>>(var)
      } -> std::same_as<rfl::Result<internal::wrap_in_rfl_array_t<T>>>;
    };

}  // namespace parsing
}  // namespace rfl

#endif
