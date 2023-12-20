#ifndef RFL_PARSING_PARSER_RESULT_HPP_
#define RFL_PARSING_PARSER_RESULT_HPP_

#include <type_traits>

#include "rfl/Result.hpp"
#include "rfl/always_false.hpp"
#include "rfl/internal/StringLiteral.hpp"
#include "rfl/parsing/Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, Result<T>>
struct Parser<R, W, Result<T>> {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  using ErrorType = NamedTuple<Field<"error", std::string>>;
  using VariantType = std::variant<std::decay_t<T>, ErrorType>;

  static Result<Result<T>> read(const R& _r,
                                const InputVarType& _var) noexcept {
    const auto handle = [](auto&& _t) -> Result<T> {
      using Type = std::decay_t<decltype(_t)>;
      if constexpr (std::is_same<Type, ErrorType>()) {
        return Error(_t.template get<"error">());
      } else {
        return std::forward<std::decay_t<T>>(_t);
      }
    };

    const auto to_res = [&](VariantType&& _v) -> Result<T> {
      return std::visit(handle, std::forward<VariantType>(_v));
    };

    return Result<Result<T>>(
        Parser<R, W, VariantType>::read(_r, _var).transform(to_res));
  }

  static OutputVarType write(const W& _w, const Result<T>& _r) noexcept {
    const auto write_t = [&](const auto& _t) -> OutputVarType {
      return Parser<R, W, std::decay_t<T>>::write(_w, _t);
    };

    const auto write_err = [&](const auto& _err) -> OutputVarType {
      return Parser<R, W, ErrorType>::write(
          _w, ErrorType(make_field<"error">(_err.what())));
    };

    return *_r.transform(write_t).or_else(write_err);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
