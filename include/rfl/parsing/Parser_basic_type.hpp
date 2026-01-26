#ifndef RFL_PARSING_PARSER_BASIC_TYPE_HPP_
#define RFL_PARSING_PARSER_BASIC_TYPE_HPP_

#include <map>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../enums.hpp"
#include "../from_named_tuple.hpp"
#include "../internal/has_default_val_v.hpp"
#include "../internal/has_reflection_method_v.hpp"
#include "../internal/has_reflection_type_v.hpp"
#include "../internal/has_reflector.hpp"
#include "../internal/is_basic_type.hpp"
#include "../internal/is_description.hpp"
#include "../internal/is_literal.hpp"
#include "../internal/is_underlying_enums_v.hpp"
#include "../internal/is_validator.hpp"
#include "../internal/processed_t.hpp"
#include "../internal/ptr_cast.hpp"
#include "../internal/to_ptr_named_tuple.hpp"
#include "../thirdparty/enchantum/enchantum.hpp"
#include "../to_view.hpp"
#include "AreReaderAndWriter.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "call_destructors_where_necessary.hpp"
#include "is_tagged_union_wrapper.hpp"
#include "make_type_name.hpp"
#include "schema/Type.hpp"
#include "schemaful/IsSchemafulReader.hpp"
#include "schemaful/IsSchemafulWriter.hpp"

namespace rfl::parsing {

/// Default case - anything that cannot be explicitly matched.
template <class R, class W, class T, class ProcessorsType>
  requires internal::is_basic_type_v<T> && AreReaderAndWriter<R, W, T>
struct Parser<R, W, T, ProcessorsType> {
 public:
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  /// Expresses the variables as type T.
  static Result<T> read(const R& _r, const InputVarType& _var) noexcept {
    return _r.template to_basic_type<std::remove_cvref_t<T>>(_var);
  }

  template <class P>
  static void write(const W& _w, const T& _var, const P& _parent) {
    ParentType::add_value(_w, _var, _parent);
  }

  /// Generates a schema for the underlying type.
  static schema::Type to_schema(std::map<std::string, schema::Type>*) {
    using U = std::remove_cvref_t<T>;
    using Type = schema::Type;
    if constexpr (std::is_same<U, bool>()) {
      return Type{Type::Boolean{}};

    } else if constexpr (std::is_same<U, std::int32_t>()) {
      return Type{Type::Int32{}};

    } else if constexpr (std::is_same<U, std::int64_t>()) {
      return Type{Type::Int64{}};

    } else if constexpr (std::is_same<U, std::uint32_t>()) {
      return Type{Type::UInt32{}};

    } else if constexpr (std::is_same<U, std::uint64_t>()) {
      return Type{Type::UInt64{}};

    } else if constexpr (std::is_integral<U>()) {
      return Type{Type::Integer{}};

    } else if constexpr (std::is_same<U, float>()) {
      return Type{Type::Float{}};

    } else if constexpr (std::is_floating_point_v<U>) {
      return Type{Type::Double{}};

    } else if constexpr (std::is_same<U, std::string>()) {
      return Type{Type::String{}};

    } else {
      static_assert(rfl::always_false_v<U>, "Unsupported type.");
    }
  }
};

}  // namespace rfl::parsing

#endif
