#ifndef RFL_PARSING_TUPLEPARSER_HPP_
#define RFL_PARSING_TUPLEPARSER_HPP_

#include <map>
#include <tuple>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parent.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, bool _ignore_empty_containers, bool _all_required,
          class... Ts>
requires AreReaderAndWriter<R, W, std::tuple<Ts...>>
struct TupleParser {
 public:
  using InputArrayType = typename R::InputArrayType;
  using InputVarType = typename R::InputVarType;

  using OutputArrayType = typename W::OutputArrayType;
  using OutputVarType = typename W::OutputVarType;

  using ParentType = Parent<W>;

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

  template <class P>
  static void write(const W& _w, const std::tuple<Ts...>& _tup,
                    const P& _parent) noexcept {
    auto arr = ParentType::add_array(_w, sizeof...(Ts), _parent);
    const auto new_parent = typename ParentType::Array{&arr};
    to_array<0>(_w, _tup, new_parent);
    _w.end_array(&arr);
  }

  template <size_t _i = 0>
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions,
      std::vector<schema::Type> _types = {}) {
    using Type = schema::Type;
    constexpr size_t size = sizeof...(Ts);
    if constexpr (_i == size) {
      return Type{Type::Tuple{.types_ = _types}};
    } else {
      using U =
          std::remove_cvref_t<std::tuple_element_t<_i, std::tuple<Ts...>>>;
      _types.push_back(Parser<R, W, U>::to_schema(_definitions));
      return to_schema<_i + 1>(_definitions, std::move(_types));
    }
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
        std::remove_cvref_t<std::tuple_element_t<_i, std::tuple<Ts...>>>;

    using ResultType = Result<NewFieldType>;

    if (_i >= _vec.size()) {
      if constexpr (_all_required ||
                    is_required<NewFieldType, _ignore_empty_containers>()) {
        return ResultType(Error("Array is of length " +
                                std::to_string(_vec.size()) + ", but field " +
                                std::to_string(_i + 1) + " is required."));
      } else {
        return ResultType(NewFieldType());
      }
    }

    return Parser<R, W, NewFieldType>::read(_r, _vec[_i]);
  }

  template <int _i, class P>
  static void to_array(const W& _w, const std::tuple<Ts...>& _tup,
                       const P& _parent) noexcept {
    if constexpr (_i < sizeof...(Ts)) {
      using NewFieldType = std::remove_cvref_t<
          typename std::tuple_element<_i, std::tuple<Ts...>>::type>;
      Parser<R, W, NewFieldType>::write(_w, std::get<_i>(_tup), _parent);
      to_array<_i + 1>(_w, _tup, _parent);
    }
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
