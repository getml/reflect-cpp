#ifndef RFL_PARSING_VECTORPARSER_HPP_
#define RFL_PARSING_VECTORPARSER_HPP_

#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "MapParser.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "is_forward_list.hpp"
#include "is_map_like.hpp"
#include "is_map_like_not_multimap.hpp"
#include "is_set_like.hpp"
#include "schema/Type.hpp"

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

  using ParentType = Parent<W>;

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

  template <class P>
  static void write(const W& _w, const VecType& _vec,
                    const P& _parent) noexcept {
    if constexpr (treat_as_map()) {
      MapParser<R, W, VecType>::write(_w, _vec, _parent);
    } else {
      auto arr = ParentType::add_array(
          _w, std::distance(_vec.begin(), _vec.end()), _parent);
      const auto new_parent = typename ParentType::Array{&arr};
      for (const auto& v : _vec) {
        Parser<R, W, std::remove_cvref_t<T>>::write(_w, v, new_parent);
      }
      _w.end_array(&arr);
    }
  }

  /// Generates a schema for the underlying type.
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using Type = schema::Type;
    return Type{Type::TypedArray{
        .type_ = Ref<Type>::make(Parser<R, W, T>::to_schema(_definitions))}};
  }

 private:
  static auto get_elem(const R& _r, auto& _v) {
    return Parser<R, W, std::remove_cvref_t<T>>::read(_r, _v).value();
  };

  static auto get_pair(const R& _r, auto& _v) {
    using K = std::remove_cvref_t<typename T::first_type>;
    using V = std::remove_cvref_t<typename T::second_type>;
    return Parser<R, W, std::remove_cvref_t<std::pair<K, V>>>::read(_r, _v)
        .value();
  }

  static VecType to_container(const R& _r, InputArrayType&& _arr) {
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
        using U = std::remove_cvref_t<typename T::first_type::ReflectionType>;
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
