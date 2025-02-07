#ifndef RFL_PARSING_FIELDVARIANTREADER_HPP_
#define RFL_PARSING_FIELDVARIANTREADER_HPP_

#include <array>
#include <optional>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "../Result.hpp"
#include "../Variant.hpp"
#include "../internal/is_array.hpp"
#include "../internal/nth_element_t.hpp"

namespace rfl::parsing {

template <class R, class W, class ProcessorsType, class... FieldTypes>
class FieldVariantReader {
 private:
  using InputVarType = typename R::InputVarType;
  using FieldVariantType = rfl::Variant<FieldTypes...>;
  using ResultType = Result<FieldVariantType>;

 public:
  FieldVariantReader(const R* _r,
                     std::optional<Result<FieldVariantType>>* _field_variant)
      : r_(_r), field_variant_(_field_variant) {}

  ~FieldVariantReader() = default;

  void read(const std::string_view& _disc_value,
            const InputVarType& _var) const noexcept {
    try_matching_fields(
        _disc_value, _var,
        std::make_integer_sequence<int, sizeof...(FieldTypes)>());
    if (!*field_variant_) {
      std::stringstream stream;
      stream << "Could not parse rfl::Variant, could not match field named "
                "'"
             << _disc_value << "'.";
      *field_variant_ = error(stream.str());
    }
  }

 private:
  template <int... _is>
  void try_matching_fields(const std::string_view& _disc_value,
                           const InputVarType& _var,
                           std::integer_sequence<int, _is...>) const noexcept {
    (try_one<_is>(_disc_value, _var), ...);
  }

  template <int _i>
  void try_one(const std::string_view& _disc_value,
               const InputVarType& _var) const noexcept {
    using FieldType =
        std::remove_cvref_t<internal::nth_element_t<_i, FieldTypes...>>;
    using ValueType = std::remove_cvref_t<typename FieldType::Type>;
    constexpr auto key = FieldType::name_.string_view();
    if (key == _disc_value) {
      const auto to_variant = [](ValueType&& _val) {
        return rfl::Variant<FieldTypes...>(FieldType(std::move(_val)));
      };
      const auto embellish_error = [&](const Error& _e) {
        std::stringstream stream;
        stream << "Could not parse rfl::Variant with field '"
               << std::string(_disc_value) << "': " << _e.what();
        return Error(stream.str());
      };
      *field_variant_ = Parser<R, W, ValueType, ProcessorsType>::read(*r_, _var)
                            .transform(to_variant)
                            .transform_error(embellish_error);
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
