#ifndef RFL_PARSING_PARSER_RESULT_HPP_
#define RFL_PARSING_PARSER_RESULT_HPP_

#include <map>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/StringLiteral.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, Result<T>>
struct Parser<R, W, Result<T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  using ErrorType = NamedTuple<Field<"error", std::string>>;
  using VariantType = std::variant<std::remove_cvref_t<T>, ErrorType>;

  static Result<Result<T>> read(const R& _r,
                                const InputVarType& _var) noexcept {
    const auto handle = [](auto&& _t) -> Result<T> {
      using Type = std::remove_cvref_t<decltype(_t)>;
      if constexpr (std::is_same<Type, ErrorType>()) {
        return error(_t.template get<"error">());
      } else {
        return std::forward<std::remove_cvref_t<T>>(_t);
      }
    };

    const auto to_res = [&](VariantType&& _v) -> Result<T> {
      return std::visit(handle, std::forward<VariantType>(_v));
    };

    return Result<Result<T>>(
        Parser<R, W, VariantType, ProcessorsType>::read(_r, _var).transform(
            to_res));
  }

  template <class P>
  static void write(const W& _w, const Result<T>& _r,
                    const P& _parent) noexcept {
    // const auto write_t = [&](const auto& _t) -> Nothing {
    //   Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, _t,
    //                                                               _parent);
    //   return Nothing{};
    // };

    // const auto write_err = [&](const auto& _err) -> Nothing {
    //   Parser<R, W, ErrorType, ProcessorsType>::write(
    //       _w, ErrorType(make_field<"error">(_err.what())), _parent);
    //   return Nothing{};
    // };
    if (_r) {
      Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(
          _w, _r.value(), _parent);
    } else {
      Parser<R, W, ErrorType, ProcessorsType>::write(
          _w, ErrorType(make_field<"error">(_r.error().what())), _parent);
    }

    // _r.transform(write_t).transform_error(write_err);
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
