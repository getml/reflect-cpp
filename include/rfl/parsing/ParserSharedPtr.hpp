#ifndef RFL_PARSING_PARSER_SHARED_PTR_HPP_
#define RFL_PARSING_PARSER_SHARED_PTR_HPP_

#include <map>
#include <memory>
#include <type_traits>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../atomic/is_atomic.hpp"
#include "../atomic/remove_atomic_t.hpp"
#include "../atomic/set_atomic.hpp"
#include "../internal/default_if_missing_v.hpp"
#include "../internal/has_default_val_v.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"
#include "schemaful/IsSchemafulReader.hpp"
#include "schemaful/IsSchemafulWriter.hpp"
#include "schemaful/SharedPtrReader.hpp"

namespace rfl::parsing {

template <class T>
struct is_shared_ptr : std::false_type {};

template <class T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <class T>
constexpr bool is_shared_ptr_v = is_shared_ptr<std::remove_cvref_t<T>>::value;

template <class R, class W, class T, class ProcessorsType>
struct ParserSharedPtr {
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  /**
   * @brief Reads a shared_ptr from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed shared_ptr or an error.
   */
  static Result<std::shared_ptr<T>> read(const R& _r,
                                         const InputVarType& _var) noexcept {
    if constexpr (atomic::is_atomic_v<T>) {
      using RemoveAtomicT = std::shared_ptr<atomic::remove_atomic_t<T>>;

      static_assert(!internal::has_default_val_v<RemoveAtomicT>,
                    "Atomic types cannot be mixed with rfl::DefaultVal");
      static_assert(!internal::default_if_missing_v<ProcessorsType>,
                    "Atomic types cannot be mixed with rfl::DefaultIfMissing");

      return Parser<R, W, RemoveAtomicT, ProcessorsType>::read(_r, _var)
          .transform([](auto&& _t) {
            if (!_t) {
              return std::shared_ptr<T>();
            }
            auto atomic_shared_ptr = std::make_shared<T>();
            atomic::set_atomic(std::move(*_t), atomic_shared_ptr.get());
            return atomic_shared_ptr;
          });

    } else if constexpr (schemaful::IsSchemafulReader<R>) {
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

  /**
   * @brief Writes a shared_ptr to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _s The shared_ptr to write.
   * @param _parent The parent object.
   */
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

  /**
   * @brief Generates the schema for the shared_ptr.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using U = std::remove_cvref_t<T>;
    return schema::Type{schema::Type::Optional{Ref<schema::Type>::make(
        Parser<R, W, U, ProcessorsType>::to_schema(_definitions))}};
  }
};

}  // namespace rfl::parsing

#endif
