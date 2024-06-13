#ifndef RFL_PARSING_TUPLEPARSER_HPP_
#define RFL_PARSING_TUPLEPARSER_HPP_

#include <map>
#include <tuple>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parent.hpp"
#include "TupleReader.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, bool _ignore_empty_containers, bool _all_required,
          class ProcessorsType, class... Ts>
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
    const auto parse =
        [&](const InputArrayType& _arr) -> Result<std::tuple<Ts...>> {
      alignas(std::tuple<Ts...>) unsigned char buf[sizeof(std::tuple<Ts...>)];
      auto ptr = reinterpret_cast<std::tuple<Ts...>*>(buf);
      const auto tuple_reader =
          TupleReader<R, W, std::tuple<Ts...>, _ignore_empty_containers,
                      _all_required, ProcessorsType>(&_r, ptr);
      auto err = _r.read_array(tuple_reader, _arr);
      if (err) {
        tuple_reader.call_destructors_where_necessary();
        return *err;
      }
      err = tuple_reader.handle_missing_fields();
      if (err) {
        tuple_reader.call_destructors_where_necessary();
        return *err;
      }
      return std::move(*ptr);
    };

    return _r.to_array(_var).and_then(parse);
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
      _types.push_back(
          Parser<R, W, U, ProcessorsType>::to_schema(_definitions));
      return to_schema<_i + 1>(_definitions, std::move(_types));
    }
  }

 private:
  template <int _i, class P>
  static void to_array(const W& _w, const std::tuple<Ts...>& _tup,
                       const P& _parent) noexcept {
    if constexpr (_i < sizeof...(Ts)) {
      using NewFieldType = std::remove_cvref_t<
          typename std::tuple_element_t<_i, std::tuple<Ts...>>>;
      Parser<R, W, NewFieldType, ProcessorsType>::write(_w, std::get<_i>(_tup),
                                                        _parent);
      to_array<_i + 1>(_w, _tup, _parent);
    }
  }
};

}  // namespace rfl::parsing

#endif
