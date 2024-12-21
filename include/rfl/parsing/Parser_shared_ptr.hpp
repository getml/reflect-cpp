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
#include "schemaful/IsSchemafulReader.hpp"
#include "schemaful/IsSchemafulWriter.hpp"
#include "schemaful/SharedPtrReader.hpp"

namespace rfl::parsing {

template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::shared_ptr<T>>
struct Parser<R, W, std::shared_ptr<T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  static Result<std::shared_ptr<T>> read(const R& _r,
                                         const InputVarType& _var) noexcept {
    if constexpr (schemaful::IsSchemafulReader<R>) {
      using S = schemaful::SharedPtrReader<R, W, std::remove_cvref_t<T>,
                                           ProcessorsType>;
      const auto to_shared = [&](const auto& _u) -> Result<std::shared_ptr<T>> {
        return _r.template read_union<std::shared_ptr<T>, S>(_u);
      };
      return _r.to_union(_var).and_then(to_shared);
    } else {
      if (_r.is_empty(_var)) {
        return std::shared_ptr<T>();
      }
      return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r,
                                                                        _var)
          .transform([](T&& _t) { return std::make_shared<T>(std::move(_t)); });
    }
  }

  template <class P>
  static void write(const W& _w, const std::shared_ptr<T>& _s,
                    const P& _parent) noexcept {
    if constexpr (schemaful::IsSchemafulWriter<W>) {
      auto u = ParentType::add_union(_w, _parent);
      using UnionType = typename ParentType::template Union<decltype(u)>;
      auto p =
          UnionType{.index_ = static_cast<size_t>(_s ? 0 : 1), .union_ = &u};
      if (_s) {
        Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, *_s, p);
      } else {
        ParentType::add_null(_w, p);
      }
    } else {
      if (_s) {
        Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, *_s,
                                                                    _parent);
      } else {
        ParentType::add_null(_w, _parent);
      }
    }
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
