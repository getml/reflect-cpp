#ifndef RFL_PARSING_SCHEMAFUL_VARIANTREADER_HPP_
#define RFL_PARSING_SCHEMAFUL_VARIANTREADER_HPP_

#include <type_traits>
#include <utility>

#include "../../Result.hpp"
#include "../../internal/nth_element_t.hpp"

namespace rfl::parsing::schemaful {

template <class R, class W, class VariantType, class ProcessorsType,
          class... AlternativeTypes>
class VariantReader {
 public:
  static Result<VariantType> read(
      const R& _r, const size_t _index,
      const typename R::InputVarType& _var) noexcept {
    return [&]<size_t... _is>(std::integer_sequence<size_t, _is...>) {
      Result<VariantType> result =
          error("Could not parse union: Index out of bounds.");
      (try_one_type<_is>(_r, _index, _var, &result), ...);
      return result;
    }(std::make_integer_sequence<size_t, sizeof...(AlternativeTypes)>());
  }

 private:
  template <size_t _i>
  static void try_one_type(const R& _r, const size_t _index,
                           const typename R::InputVarType& _var,
                           Result<VariantType>* _result) noexcept {
    if (_index == _i) {
      using T = internal::nth_element_t<_i, AlternativeTypes...>;
      *_result =
          Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var)
              .transform(
                  [](auto&& _val) -> VariantType { return std::move(_val); });
    }
  }
};

}  // namespace rfl::parsing::schemaful

#endif
