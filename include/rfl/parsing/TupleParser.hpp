#ifndef RFL_PARSING_TUPLEPARSER_HPP_
#define RFL_PARSING_TUPLEPARSER_HPP_

#include <cstddef>
#include <map>
#include <type_traits>
#include <utility>

#include "../Result.hpp"
#include "../Tuple.hpp"
#include "../always_false.hpp"
#include "../internal/nth_element_t.hpp"
#include "../internal/ptr_cast.hpp"
#include "Parent.hpp"
#include "TupleReader.hpp"
#include "call_destructors_on_tuple_where_necessary.hpp"
#include "schema/Type.hpp"
#include "schemaful/IsSchemafulReader.hpp"
#include "schemaful/IsSchemafulWriter.hpp"
#include "schemaful/tuple_to_named_tuple.hpp"
#include "schemaful/tuple_to_named_tuple_t.hpp"

namespace rfl::parsing {

template <class R, class W, bool _ignore_empty_containers, bool _all_required,
          class ProcessorsType, class TupleType>
  requires AreReaderAndWriter<R, W, TupleType>
struct TupleParser {
 public:
  using InputArrayType = typename R::InputArrayType;
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  static Result<TupleType> read(const R& _r,
                                const InputVarType& _var) noexcept {
    if constexpr (schemaful::IsSchemafulReader<R>) {
      using NamedTupleType = schemaful::tuple_to_named_tuple_t<TupleType>;
      const auto to_tuple = [](auto&& _named_tuple) {
        return [&]<int... _is>(std::integer_sequence<int, _is...>) {
          return TupleType(std::move(rfl::get<_is>(_named_tuple))...);
        }(std::make_integer_sequence<int, NamedTupleType::size()>());
      };
      return Parser<R, W, NamedTupleType, ProcessorsType>::read(_r, _var)
          .transform(to_tuple);

    } else {
      const auto parse = [&](const InputArrayType& _arr) -> Result<TupleType> {
        alignas(TupleType) unsigned char buf[sizeof(TupleType)]{};
        auto ptr = internal::ptr_cast<TupleType*>(&buf);
        const auto tuple_reader =
            TupleReader<R, W, TupleType, _ignore_empty_containers,
                        _all_required, ProcessorsType>(&_r, ptr);
        auto err = _r.read_array(tuple_reader, _arr);
        if (err) {
          call_destructors_on_tuple_where_necessary(tuple_reader.num_set(),
                                                    ptr);
          return error(*err);
        }
        err = tuple_reader.handle_missing_fields();
        if (err) {
          call_destructors_on_tuple_where_necessary(tuple_reader.num_set(),
                                                    ptr);
          return error(*err);
        }
        auto res = Result<TupleType>(std::move(*ptr));
        call_destructors_on_tuple_where_necessary(tuple_reader.num_set(), ptr);
        return res;
      };

      return _r.to_array(_var).and_then(parse);
    }
  }

  template <class P>
  static void write(const W& _w, const TupleType& _tup,
                    const P& _parent) noexcept {
    if constexpr (schemaful::IsSchemafulWriter<W>) {
      const auto named_tuple = schemaful::tuple_to_named_tuple(_tup);
      Parser<R, W, std::remove_cvref_t<decltype(named_tuple)>,
             ProcessorsType>::write(_w, named_tuple, _parent);

    } else {
      constexpr auto size = rfl::tuple_size_v<TupleType>;
      auto arr = ParentType::add_array(_w, size, _parent);
      const auto new_parent = typename ParentType::Array{&arr};
      to_array(_w, _tup, new_parent, std::make_integer_sequence<int, size>());
      _w.end_array(&arr);
    }
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    std::vector<schema::Type> types;
    build_schema(
        _definitions, &types,
        std::make_integer_sequence<int, rfl::tuple_size_v<TupleType>>());
    return schema::Type{schema::Type::Tuple{.types_ = std::move(types)}};
  }

 private:
  template <size_t _i>
  static void add_to_schema(std::map<std::string, schema::Type>* _definitions,
                            std::vector<schema::Type>* _types) noexcept {
    using U = std::remove_cvref_t<rfl::tuple_element_t<_i, TupleType>>;
    _types->push_back(Parser<R, W, U, ProcessorsType>::to_schema(_definitions));
  }

  template <int... _is>
  static void build_schema(std::map<std::string, schema::Type>* _definitions,
                           std::vector<schema::Type>* _types,
                           std::integer_sequence<int, _is...>) noexcept {
    (add_to_schema<_is>(_definitions, _types), ...);
  }

  template <int _i, class P>
  static void add_to_array(const W& _w, const TupleType& _tup,
                           const P& _parent) noexcept {
    using NewFieldType =
        std::remove_cvref_t<rfl::tuple_element_t<_i, TupleType>>;
    Parser<R, W, NewFieldType, ProcessorsType>::write(_w, rfl::get<_i>(_tup),
                                                      _parent);
  }

  template <int... _is, class P>
  static void to_array(const W& _w, const TupleType& _tup, const P& _parent,
                       std::integer_sequence<int, _is...>) noexcept {
    (add_to_array<_is>(_w, _tup, _parent), ...);
  }
};

}  // namespace rfl::parsing

#endif
