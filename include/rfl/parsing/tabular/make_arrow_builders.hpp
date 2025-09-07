#ifndef RFL_PARSING_TABULAR_MAKEARROWBUILDERS_HPP_
#define RFL_PARSING_TABULAR_MAKEARROWBUILDERS_HPP_

#include <arrow/api.h>

#include <array>
#include <cinttypes>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "../../named_tuple_t.hpp"
#include "ArrowTypes.hpp"

namespace rfl::parsing::tabular {

template <class T, SerializationType _s>
using arrow_builder_t =
    typename ArrowTypes<std::remove_cvref_t<std::remove_pointer_t<T>>,
                        _s>::BuilderType;

template <class T, SerializationType _s>
struct ArrowBuildersType;

template <SerializationType _s, class... FieldTypes>
struct ArrowBuildersType<NamedTuple<FieldTypes...>, _s> {
  using Type = Tuple<arrow_builder_t<typename FieldTypes::Type, _s>...>;

  static auto data_types() {
    return [&]<size_t... _is>(std::integer_sequence<size_t, _is...>) {
      return std::array<std::shared_ptr<arrow::DataType>,
                        sizeof...(FieldTypes)>(
          {ArrowTypes<typename FieldTypes::Type, _s>::data_type()...});
    }(std::make_integer_sequence<size_t, sizeof...(FieldTypes)>());
  }

  static Type make_builders() {
    return Type(ArrowTypes<typename FieldTypes::Type, _s>::make_builder()...);
  }

  static auto schema() {
    const auto fields =
        std::vector<std::shared_ptr<arrow::Field>>({arrow::field(
            typename FieldTypes::Name().str(),
            ArrowTypes<typename FieldTypes::Type, _s>::data_type())...});
    return arrow::schema(fields);
  }
};

template <class T, SerializationType _s>
auto make_arrow_builders() {
  return ArrowBuildersType<std::remove_cvref_t<T>, _s>::make_builders();
}

}  // namespace rfl::parsing::tabular

#endif
