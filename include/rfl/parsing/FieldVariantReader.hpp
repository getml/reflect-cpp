#ifndef RFL_PARSING_FIELDVARIANTREADER_HPP_
#define RFL_PARSING_FIELDVARIANTREADER_HPP_

#include <array>
#include <optional>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

#include "../Result.hpp"
#include "../internal/is_array.hpp"

namespace rfl::parsing {

template <class R, class W, class ProcessorsType, class... FieldTypes>
class FieldVariantReader {
 private:
  using InputVarType = typename R::InputVarType;
  using FieldVariantType = std::variant<FieldTypes...>;
  using ResultType = Result<FieldVariantType>;

 public:
  FieldVariantReader(const R* _r,
                     std::optional<Result<FieldVariantType>>* _field_variant)
      : r_(_r), field_variant_(_field_variant) {}

  ~FieldVariantReader() = default;

  template <size_t _i = 0>
  void read(const std::string_view& _disc_value,
            const InputVarType& _var) const noexcept {
    if constexpr (_i < sizeof...(FieldTypes)) {
      if constexpr (_i == 0) {
        if (*field_variant_) {
          *field_variant_ = Error(
              "Could not parse: Expected the object to have "
              "exactly one field, but found more than one.");
          return;
        }
      }

      using FieldType = std::remove_cvref_t<
          typename std::tuple_element<_i, std::tuple<FieldTypes...>>::type>;

      using ValueType = std::remove_cvref_t<typename FieldType::Type>;

      constexpr auto key = FieldType::name_.string_view();

      if (key == _disc_value) {
        const auto to_variant = [](ValueType&& _val) {
          return std::variant<FieldTypes...>(FieldType(std::move(_val)));
        };
        const auto embellish_error = [&](const Error& _e) {
          return Error("Could not parse std::variant with field '" +
                       std::string(_disc_value) + "': " + _e.what());
        };
        *field_variant_ =
            Parser<R, W, ValueType, ProcessorsType>::read(*r_, _var)
                .transform(to_variant)
                .or_else(embellish_error);
        return;
      } else {
        read<_i + 1>(_disc_value, _var);
      }
    } else {
      *field_variant_ = Error(
          "Could not parse std::variant, could not match field named "
          "'" +
          std::string(_disc_value) + "'.");
    }
  }

 private:
  /// The underlying reader.
  const R* r_;

  /// The underlying field variant.
  std::optional<Result<FieldVariantType>>* field_variant_;
};

}  // namespace rfl::parsing

#endif
