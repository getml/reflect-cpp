#ifndef RFL_PARSING_MAPPARSER_HPP_
#define RFL_PARSING_MAPPARSER_HPP_

#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "MapReader.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"
#include "to_single_error_message.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class MapType, class ProcessorsType>
requires AreReaderAndWriter<R, W, MapType>
struct MapParser {
 public:
  using InputObjectType = typename R::InputObjectType;
  using InputVarType = typename R::InputVarType;

  using OutputObjectType = typename W::OutputObjectType;
  using OutputVarType = typename W::OutputVarType;

  using KeyType = std::remove_cvref_t<typename MapType::value_type::first_type>;
  using ValueType =
      std::remove_cvref_t<typename MapType::value_type::second_type>;

  using ParentType = Parent<W>;

  static Result<MapType> read(const R& _r, const InputVarType& _var) noexcept {
    const auto to_map = [&](auto obj) -> Result<MapType> {
      return make_map(_r, obj);
    };
    return _r.to_object(_var).and_then(to_map);
  }

  template <class P>
  static void write(const W& _w, const MapType& _m, const P& _parent) noexcept {
    auto obj = ParentType::add_object(_w, _m.size(), _parent);
    for (const auto& [k, v] : _m) {
      if constexpr (internal::has_reflection_type_v<KeyType>) {
        using ReflT = typename KeyType::ReflectionType;

        if constexpr (std::is_integral_v<ReflT> ||
                      std::is_floating_point_v<ReflT>) {
          const auto name = std::to_string(k.reflection());
          const auto new_parent = typename ParentType::Object{name, &obj};
          Parser<R, W, std::remove_cvref_t<ValueType>, ProcessorsType>::write(
              _w, v, new_parent);
        } else {
          const auto name = k.reflection();
          const auto new_parent = typename ParentType::Object{name, &obj};
          Parser<R, W, std::remove_cvref_t<ValueType>, ProcessorsType>::write(
              _w, v, new_parent);
        }

      } else if constexpr (std::is_integral_v<KeyType> ||
                           std::is_floating_point_v<KeyType>) {
        const auto name = std::to_string(k);
        const auto new_parent = typename ParentType::Object{name, &obj};
        Parser<R, W, std::remove_cvref_t<ValueType>, ProcessorsType>::write(
            _w, v, new_parent);
      } else {
        const auto new_parent = typename ParentType::Object{k, &obj};
        Parser<R, W, std::remove_cvref_t<ValueType>, ProcessorsType>::write(
            _w, v, new_parent);
      }
    }
    _w.end_object(&obj);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return schema::Type{schema::Type::StringMap{Ref<schema::Type>::make(
        Parser<R, W, ValueType, ProcessorsType>::to_schema(_definitions))}};
  }

 private:
  static Result<MapType> make_map(const R& _r, const InputObjectType& _obj) {
    MapType map;
    std::vector<Error> errors;
    const auto map_reader =
        MapReader<R, W, MapType, ProcessorsType>(&_r, &map, &errors);
    const auto err = _r.read_object(map_reader, _obj);
    if (err) {
      return *err;
    }
    if (errors.size() != 0) {
      return to_single_error_message(errors);
    }
    return map;
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
