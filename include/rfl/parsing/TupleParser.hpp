#ifndef RFL_PARSING_TUPLEPARSER_HPP_
#define RFL_PARSING_TUPLEPARSER_HPP_

#include <tuple>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, bool _ignore_empty_containers, class... Ts>
requires AreReaderAndWriter<R, W, std::tuple<Ts...>>
struct TupleParser {
 public:
  using InputArrayType = typename R::InputArrayType;
  using InputVarType = typename R::InputVarType;

  using OutputArrayType = typename W::OutputArrayType;
  using OutputVarType = typename W::OutputVarType;

  static Result<std::tuple<Ts...>> read(const R& _r,
                                        const InputVarType& _var) noexcept {
    const auto to_vec = [&](auto _arr) { return _r.to_vec(_arr); };

    const auto check_size = [](auto _vec) -> Result<std::vector<InputVarType>> {
      if (_vec.size() > sizeof...(Ts)) {
        return Error("Expected " + std::to_string(sizeof...(Ts)) +
                     " fields, got " + std::to_string(_vec.size()) + ".");
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
                             const std::tuple<Ts...>& _tup) noexcept {
    auto arr = _w.new_array();
    to_array<0>(_w, _tup, &arr);
    return arr;
  }

 private:
  template <class... AlreadyExtracted>
  static Result<std::tuple<Ts...>> extract_field_by_field(
      const R& _r, std::vector<InputVarType> _vec,
      AlreadyExtracted&&... _already_extracted) noexcept {
    constexpr size_t i = sizeof...(AlreadyExtracted);
    if constexpr (i == sizeof...(Ts)) {
      return std::tuple<Ts...>(
          std::make_tuple(std::move(_already_extracted)...));
    } else {
      const auto extract_next = [&](auto&& new_entry) {
        return extract_field_by_field(_r, std::move(_vec),
                                      std::move(_already_extracted)...,
                                      std::move(new_entry));
      };
      return extract_single_field<i>(_r, _vec).and_then(extract_next);
    }
  }

  template <int _i>
  static auto extract_single_field(
      const R& _r, const std::vector<InputVarType>& _vec) noexcept {
    using NewFieldType =
        std::decay_t<typename std::tuple_element<_i, std::tuple<Ts...>>::type>;

    using ResultType = Result<NewFieldType>;

    if (_i >= _vec.size()) {
      if constexpr (is_required<NewFieldType, _ignore_empty_containers>()) {
        return ResultType(Error("Array is of length " +
                                std::to_string(_vec.size()) + ", but field " +
                                std::to_string(_i + 1) + " is required."));
      } else {
        return ResultType(NewFieldType());
      }
    }

    return Parser<R, W, NewFieldType>::read(_r, _vec[_i]);
  }

  template <int _i>
  static void to_array(const W& _w, const std::tuple<Ts...>& _tup,
                       OutputArrayType* _ptr) noexcept {
    if constexpr (_i < sizeof...(Ts)) {
      using NewFieldType = std::decay_t<
          typename std::tuple_element<_i, std::tuple<Ts...>>::type>;

      const auto val =
          Parser<R, W, NewFieldType>::write(_w, std::get<_i>(_tup));
      _w.add(val, _ptr);
      to_array<_i + 1>(_w, _tup, _ptr);
    }
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
