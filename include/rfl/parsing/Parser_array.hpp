#ifndef RFL_PARSING_PARSER_ARRAY_HPP_
#define RFL_PARSING_PARSER_ARRAY_HPP_

#include <array>
#include <type_traits>
#include <vector>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parser_base.hpp"

namespace rfl {
namespace parsing {
template <class R, class W, class T, size_t _size>
  requires AreReaderAndWriter<R, W, std::array<T, _size>>
struct Parser<R, W, std::array<T, _size>> {
 public:
  using InputArrayType = typename R::InputArrayType;
  using InputVarType = typename R::InputVarType;

  using OutputArrayType = typename W::OutputArrayType;
  using OutputVarType = typename W::OutputVarType;

  static Result<std::array<T, _size>> read(const R& _r,
                                           const InputVarType& _var) noexcept {
    const auto to_vec = [&](const auto& _arr) { return _r.to_vec(_arr); };

    const auto check_size = [](auto _vec) -> Result<std::vector<InputVarType>> {
      if (_vec.size() != _size) {
        return Error("Expected " + std::to_string(_size) + " fields, got " +
                     std::to_string(_vec.size()) + ".");
      }
      return std::move(_vec);
    };

    const auto extract = [&_r](auto _vec) {
      return extract_field_by_field(_r, std::move(_vec));
    };

    return _r.to_array(_var)
        .transform(to_vec)
        .and_then(check_size)
        .and_then(extract);
  }

  static OutputVarType write(const W& _w,
                             const std::array<T, _size>& _arr) noexcept {
    auto arr = _w.new_array();
    for (auto it = _arr.begin(); it != _arr.end(); ++it) {
      _w.add(Parser<R, W, std::remove_cvref_t<T>>::write(_w, *it), &arr);
    }
    return OutputVarType(arr);
  }

 private:
  /// Extracts values from the array, field by field.
  template <class... AlreadyExtracted>
  static Result<std::array<T, _size>> extract_field_by_field(
      const R& _r, std::vector<InputVarType> _vec,
      AlreadyExtracted&&... _already_extracted) noexcept {
    constexpr size_t i = sizeof...(AlreadyExtracted);
    if constexpr (i == _size) {
      return std::array<T, _size>{std::move(_already_extracted)...};
    } else {
      const auto extract_next = [&](auto&& new_entry) {
        return extract_field_by_field(_r, std::move(_vec),
                                      std::move(_already_extracted)...,
                                      std::move(new_entry));
      };
      return extract_single_field<i>(_r, _vec).and_then(extract_next);
    }
  }

  /// Extracts a single field from a JSON.
  template <int _i>
  static auto extract_single_field(
      const R& _r, const std::vector<InputVarType>& _vec) noexcept {
    return Parser<R, W, T>::read(_r, _vec[_i]);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
