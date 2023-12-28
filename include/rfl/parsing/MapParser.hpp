#ifndef RFL_PARSING_MAPPARSER_HPP_
#define RFL_PARSING_MAPPARSER_HPP_

#include <stdexcept>
#include <string>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class MapType>
requires AreReaderAndWriter<R, W, MapType>
struct MapParser {
 public:
  using InputObjectType = typename R::InputObjectType;
  using InputVarType = typename R::InputVarType;

  using OutputObjectType = typename W::OutputObjectType;
  using OutputVarType = typename W::OutputVarType;

  using KeyType = std::decay_t<typename MapType::value_type::first_type>;
  using ValueType = std::decay_t<typename MapType::value_type::second_type>;

  static Result<MapType> read(const R& _r, const InputVarType& _var) noexcept {
    const auto to_map = [&](const auto& _obj) { return make_map(_r, _obj); };
    return _r.to_object(_var).and_then(to_map);
  }

  static OutputVarType write(const W& _w, const MapType& _m) noexcept {
    auto obj = _w.new_object();
    for (const auto& [k, v] : _m) {
      auto parsed_val = Parser<R, W, std::decay_t<ValueType>>::write(_w, v);
      if constexpr (internal::has_reflection_type_v<KeyType>) {
        using ReflT = typename KeyType::ReflectionType;

        if constexpr (std::is_integral_v<ReflT> ||
                      std::is_floating_point_v<ReflT>) {
          _w.set_field(std::to_string(k.reflection()), std::move(parsed_val),
                       &obj);
        } else {
          _w.set_field(k.reflection(), std::move(parsed_val), &obj);
        }

      } else if constexpr (std::is_integral_v<KeyType> ||
                           std::is_floating_point_v<KeyType>) {
        _w.set_field(std::to_string(k), std::move(parsed_val), &obj);
      } else {
        _w.set_field(k, std::move(parsed_val), &obj);
      }
    }
    return obj;
  }

 private:
  template <class T>
  static Result<T> key_to_numeric(auto& _pair) noexcept {
    try {
      if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(std::stoi(_pair.first));
      } else if constexpr (std::is_floating_point_v<T>) {
        return static_cast<T>(std::stod(_pair.first));
      } else {
        static_assert(always_false_v<T>, "Unsupported type");
      }
    } catch (std::exception& e) {
      return Error(e.what());
    }
  }

  static Result<std::pair<KeyType, ValueType>> make_key(auto& _pair) noexcept {
    const auto to_pair = [&](auto _key) {
      return std::make_pair(KeyType(std::move(_key)), std::move(_pair.second));
    };

    if constexpr (std::is_integral_v<KeyType> ||
                  std::is_floating_point_v<KeyType>) {
      return key_to_numeric<KeyType>(_pair).transform(to_pair);

    } else if constexpr (internal::has_reflection_type_v<KeyType>) {
      using ReflT = typename KeyType::ReflectionType;

      if constexpr (std::is_integral_v<ReflT> ||
                    std::is_floating_point_v<ReflT>) {
        return key_to_numeric<ReflT>(_pair).transform(to_pair);
      } else {
        return to_pair(_pair.first);
      }

    } else {
      return std::move(_pair);
    }
  }

  static std::pair<KeyType, ValueType> get_pair(const R& _r, auto& _pair) {
    const auto to_pair = [&](ValueType&& _val) {
      auto pair = std::make_pair(std::move(_pair.first), std::move(_val));
      return make_key(pair);
    };
    return Parser<R, W, std::decay_t<ValueType>>::read(_r, _pair.second)
        .and_then(to_pair)
        .value();
  }

  static Result<MapType> make_map(const R& _r, const auto& _obj) noexcept {
    auto m = _r.to_map(_obj);
    MapType res;
    try {
      for (auto& p : m) {
        res.insert(get_pair(_r, p));
      }
    } catch (std::exception& e) {
      return Error(e.what());
    }
    return res;
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
