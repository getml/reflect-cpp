#ifndef RFL_PARSING_MAPREADER_HPP_
#define RFL_PARSING_MAPREADER_HPP_

#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl::parsing {

template <class R, class W, class MapType>
class MapReader {
 private:
  using InputVarType = typename R::InputVarType;
  using KeyType = std::remove_cvref_t<typename MapType::value_type::first_type>;
  using ValueType =
      std::remove_cvref_t<typename MapType::value_type::second_type>;

 public:
  MapReader(const R* _r, MapType* _map, std::vector<Error>* _errors)
      : r_(_r), map_(_map), errors_(_errors) {}

  ~MapReader() = default;

  void read(const std::string_view& _name,
            const InputVarType& _var) const noexcept {
    auto res = get_pair(_name, _var);
    if (res) {
      map_->emplace(std::move(*res));
    } else {
      errors_->push_back(Error("Failed to parse field '" + std::string(_name) +
                               "': " + res.error()->what()));
    }
  }

 private:
  template <class T>
  Result<T> key_to_numeric(auto& _pair) const noexcept {
    try {
      if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
        return static_cast<T>(std::stoll(_pair.first));
      } else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>) {
        return static_cast<T>(std::stoull(_pair.first));
      } else if constexpr (std::is_floating_point_v<T>) {
        return static_cast<T>(std::stod(_pair.first));
      } else {
        static_assert(always_false_v<T>, "Unsupported type");
      }
    } catch (std::exception& e) {
      return Error(e.what());
    }
  }

  Result<std::pair<KeyType, ValueType>> make_key(auto& _pair) const noexcept {
    const auto to_pair =
        [&](auto _key) -> Result<std::pair<KeyType, ValueType>> {
      try {
        return std::make_pair(KeyType(std::move(_key)),
                              std::move(_pair.second));
      } catch (std::exception& e) {
        return Error(e.what());
      }
    };

    if constexpr (std::is_integral_v<KeyType> ||
                  std::is_floating_point_v<KeyType>) {
      return key_to_numeric<KeyType>(_pair).and_then(to_pair);

    } else if constexpr (internal::has_reflection_type_v<KeyType>) {
      using ReflT = typename KeyType::ReflectionType;

      if constexpr (std::is_integral_v<ReflT> ||
                    std::is_floating_point_v<ReflT>) {
        return key_to_numeric<ReflT>(_pair).and_then(to_pair);
      } else {
        return to_pair(_pair.first);
      }

    } else {
      return std::move(_pair);
    }
  }

  Result<std::pair<KeyType, ValueType>> get_pair(
      const std::string_view& _name, const InputVarType& _var) const noexcept {
    const auto to_pair = [&](ValueType&& _val) {
      auto pair = std::make_pair(std::string(_name), std::move(_val));
      return make_key(pair);
    };
    return Parser<R, W, std::remove_cvref_t<ValueType>>::read(*r_, _var)
        .and_then(to_pair);
  }

 private:
  /// The underlying reader.
  const R* r_;

  /// The underlying map.
  MapType* map_;

  /// Collects any errors we may have come across.
  std::vector<Error>* errors_;
};

}  // namespace rfl::parsing

#endif
