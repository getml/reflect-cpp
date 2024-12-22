#ifndef RFL_PARSING_PARSER_SKIP_HPP_
#define RFL_PARSING_PARSER_SKIP_HPP_

#include <map>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/Skip.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, bool _skip_serialization,
          bool _skip_deserialization, class ProcessorsType>
  requires AreReaderAndWriter<
      R, W, internal::Skip<T, _skip_serialization, _skip_deserialization>>
struct Parser<R, W,
              internal::Skip<T, _skip_serialization, _skip_deserialization>,
              ProcessorsType> {
  using InputVarType = typename R::InputVarType;
  using ParentType = Parent<W>;

  static Result<internal::Skip<T, _skip_serialization, _skip_deserialization>>
  read(const R& _r, const InputVarType& _var) noexcept {
    if constexpr (_skip_deserialization) {
      return internal::Skip<T, _skip_serialization, _skip_deserialization>(
          std::remove_cvref_t<T>());
    } else {
      const auto to_skip = [&](auto&& _t) {
        return internal::Skip<T, _skip_serialization, _skip_deserialization>(
            std::move(_t));
      };
      return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r,
                                                                        _var)
          .transform(to_skip);
    }
  }

  template <class P>
  static void write(const W& _w,
                    const internal::Skip<T, _skip_serialization,
                                         _skip_deserialization>& _skip,
                    const P& _parent) noexcept {
    if constexpr (_skip_serialization) {
      ParentType::add_null(_w, _parent);
    } else {
      Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(
          _w, _skip.value(), _parent);
    }
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::to_schema(
        _definitions);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
