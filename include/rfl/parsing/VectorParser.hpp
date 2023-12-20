#ifndef RFL_PARSING_VECTORPARSER_HPP_
#define RFL_PARSING_VECTORPARSER_HPP_

#include <stdexcept>
#include <string>
#include <type_traits>

#include "rfl/Result.hpp"
#include "rfl/always_false.hpp"
#include "rfl/parsing/MapParser.hpp"
#include "rfl/parsing/Parser_base.hpp"
#include "rfl/parsing/is_forward_list.hpp"
#include "rfl/parsing/is_map_like.hpp"
#include "rfl/parsing/is_map_like_not_multimap.hpp"
#include "rfl/parsing/is_set_like.hpp"

namespace rfl {
namespace parsing {

/// This can be used for data structures that would be expressed as array in
/// serialized format (std::vector, std::set, std::deque, ...),
/// but also includes map-like types, when the key is not of type
/// std::string.
template <class R, class W, class VecType>
requires AreReaderAndWriter<R, W, VecType>
struct VectorParser {
 public:
  using InputArrayType = typename R::InputArrayType;
  using InputVarType = typename R::InputVarType;

  using OutputArrayType = typename W::OutputArrayType;
  using OutputVarType = typename W::OutputVarType;

  using T = typename VecType::value_type;

  static Result<VecType> read(const R& _r, const InputVarType& _var) noexcept {
    if constexpr (treat_as_map()) {
      return MapParser<R, W, VecType>::read(_r, _var);
    } else {
      const auto to_res = [&](auto&& _v) {
        return to_result(_r, std::move(_v));
      };
      return _r.to_array(_var).and_then(to_res);
    }
  }

  static OutputVarType write(const W& _w, const VecType& _vec) noexcept {
    if constexpr (treat_as_map()) {
      return MapParser<R, W, VecType>::write(_w, _vec);
    } else {
      auto arr = _w.new_array();
      for (const auto& v : _vec) {
        _w.add(Parser<R, W, std::decay_t<T>>::write(_w, v), &arr);
      }
      return OutputVarType(arr);
    }
  }

 private:
  static auto get_elem(const R& _r, auto& _v) {
    return Parser<R, W, std::decay_t<T>>::read(_r, _v).value();
  };

  static auto get_pair(const R& _r, auto& _v) {
    using K = std::decay_t<typename T::first_type>;
    using V = std::decay_t<typename T::second_type>;
    return Parser<R, W, std::decay_t<std::pair<K, V>>>::read(_r, _v).value();
  }

  static Result<VecType> to_container(const R& _r, InputArrayType&& _arr) {
    auto input_vars = _r.to_vec(_arr);
    VecType vec;
    if constexpr (is_forward_list<VecType>()) {
      for (auto it = input_vars.rbegin(); it != input_vars.rend(); ++it) {
        vec.emplace_front(get_elem(_r, *it));
      }
    } else if constexpr (is_map_like<VecType>()) {
      for (auto& v : input_vars) {
        vec.insert(get_pair(_r, v));
      }
    } else if constexpr (is_set_like<VecType>()) {
      for (auto& v : input_vars) {
        vec.insert(get_elem(_r, v));
      }
    } else {
      for (auto& v : input_vars) {
        vec.emplace_back(get_elem(_r, v));
      }
    }
    return vec;
  }

  static Result<VecType> to_result(const R& _r, InputArrayType&& _arr) {
    try {
      return to_container(_r, std::forward<InputArrayType>(_arr));
    } catch (std::exception& e) {
      return Error(e.what());
    }
  }

  static constexpr bool treat_as_map() {
    if constexpr (is_map_like_not_multimap<VecType>()) {
      if constexpr (internal::has_reflection_type_v<typename T::first_type>) {
        using U = std::decay_t<typename T::first_type::ReflectionType>;
        return std::is_same<U, std::string>() || std::is_integral_v<U> ||
               std::is_floating_point_v<U>;

        // We do not need std::string here, it is already caught by the template
        // specialization.
      } else if constexpr (std::is_integral_v<typename T::first_type> ||
                           std::is_floating_point_v<typename T::first_type>) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
