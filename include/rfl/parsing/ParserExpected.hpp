#ifndef RFL_PARSING_PARSER_EXPECTED_HPP_
#define RFL_PARSING_PARSER_EXPECTED_HPP_

#include <map>
#include <type_traits>

#include "../Field.hpp"
#include "../NamedTuple.hpp"
#include "../Result.hpp"
#include "../Variant.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

#if __has_include(<expected>)
#include <expected>
#endif

namespace rfl::parsing {

template <class T>
struct is_expected : std::false_type {};

/// @brief Primary declaration; defined below if std::expected is available.
template <class R, class W, class ExpectedType, class ProcessorsType>
struct ParserExpected;

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L

template <class T, class E>
struct is_expected<std::expected<T, E>> : std::true_type {};

/**
 * @brief Parser specialization for std::expected.
 *
 * A std::expected<T, E> is serialized as an rfl::Variant<T, E>: the success
 * value and the error are treated as the two alternatives of a variant.
 */
template <class R, class W, class ExpectedType, class ProcessorsType>
struct ParserExpected {
  using T = std::remove_cvref_t<ExpectedType>;

  using ValueType = typename T::value_type;

  using ErrorType = typename T::error_type;

  using WrappedErrorType = NamedTuple<Field<"error", ErrorType>>;

  using VariantType = rfl::Variant<ValueType, WrappedErrorType>;

  using InputVarType = typename R::InputVarType;

  static_assert(!std::is_void_v<ValueType>,
                "std::expected<void, E> is not supported by reflect-cpp.");

  /**
   * @brief Reads a std::expected from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed std::expected or an error.
   */
  static Result<T> read(const R& _r, const InputVarType& _var) noexcept {
    const auto to_expected = [](auto&& _alternative) -> T {
      using AltType = std::remove_cvref_t<decltype(_alternative)>;
      if constexpr (std::is_same_v<AltType, WrappedErrorType>) {
        return T(std::unexpected(
            std::forward<ErrorType>(_alternative.template get<"error">())));
      } else {
        return T(std::forward<ValueType>(_alternative));
      }
    };
    return Parser<R, W, VariantType, ProcessorsType>::read(_r, _var).transform(
        [&](auto&& _variant) -> T {
          return std::forward<VariantType>(_variant).visit(to_expected);
        });
  }

  /**
   * @brief Writes a std::expected to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _var The std::expected to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const T& _var, const P& _parent) {
    const VariantType variant =
        _var.has_value() ? VariantType(_var.value())
                         : VariantType(WrappedErrorType(_var.error()));
    Parser<R, W, VariantType, ProcessorsType>::write(_w, variant, _parent);
  }

  /**
   * @brief Generates the schema for the std::expected.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, VariantType, ProcessorsType>::to_schema(_definitions);
  }
};

#endif  // __cpp_lib_expected

template <class T>
constexpr bool is_expected_v = is_expected<std::remove_cvref_t<T>>::value;

}  // namespace rfl::parsing

#endif
