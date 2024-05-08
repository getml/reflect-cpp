#ifndef RFL_PARSING_PARSER_SHARED_PTR_HPP_
#define RFL_PARSING_PARSER_SHARED_PTR_HPP_

#include <map>
#include <memory>
#include <type_traits>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class T, class ProcessorsType>
requires AreReaderAndWriter<R, W, std::shared_ptr<T>>
struct Parser<R, W, std::shared_ptr<T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  using ParentType = Parent<W>;

  static Result<std::shared_ptr<T>> read(const R& _r,
                                         const InputVarType& _var) noexcept {
    if (_r.is_empty(_var)) {
      return std::shared_ptr<T>();
    }
    const auto to_ptr = [](auto&& _t) {
      return std::make_shared<T>(std::move(_t));
    };
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var)
        .transform(to_ptr);
  }

  template <class P>
  static void write(const W& _w, const std::shared_ptr<T>& _s,
                    const P& _parent) noexcept {
    if (!_s) {
      ParentType::add_null(_w, _parent);
      return;
    }
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, *_s,
                                                                _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using U = std::remove_cvref_t<T>;
    return schema::Type{schema::Type::Optional{Ref<schema::Type>::make(
        Parser<R, W, U, ProcessorsType>::to_schema(_definitions))}};
  }
};

}  // namespace rfl::parsing

#endif
