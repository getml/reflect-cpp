#ifndef RFL_PARSING_PARSER_REF_HPP_
#define RFL_PARSING_PARSER_REF_HPP_

#include <map>
#include <type_traits>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../atomic/is_atomic.hpp"
#include "../atomic/remove_atomic_t.hpp"
#include "../atomic/set_atomic.hpp"
#include "../internal/default_if_missing_v.hpp"
#include "../internal/has_default_val_v.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_ref : std::false_type {};

template <class T>
struct is_ref<Ref<T>> : std::true_type {
  using element_type = T;
};

template <class T>
constexpr bool is_ref_v = is_ref<std::remove_cvref_t<T>>::value;

template <class R, class W, class T, class ProcessorsType>
struct ParserRef {
  using InputVarType = typename R::InputVarType;

  /**
   * @brief Reads a Ref from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed Ref or an error.
   */
  static Result<Ref<T>> read(const R& _r, const InputVarType& _var) noexcept {
    if constexpr (atomic::is_atomic_v<T>) {
      using RemoveAtomicT = atomic::remove_atomic_t<T>;

      static_assert(!internal::has_default_val_v<RemoveAtomicT>,
                    "Atomic types cannot be mixed with rfl::DefaultVal");
      static_assert(!internal::default_if_missing_v<ProcessorsType>,
                    "Atomic types cannot be mixed with rfl::DefaultIfMissing");

      return Parser<R, W, RemoveAtomicT, ProcessorsType>::read(_r, _var)
          .transform([](auto&& _t) {
            auto atomic_ref = Ref<T>::make();
            atomic::set_atomic(std::move(_t), &(*atomic_ref));
            return atomic_ref;
          });

    } else {
      const auto to_ref = [&](auto&& _t) {
        return Ref<T>::make(std::move(_t));
      };
      return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r,
                                                                        _var)
          .transform(to_ref);
    }
  }

  /**
   * @brief Writes a Ref to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _ref The Ref to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const Ref<T>& _ref, const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, *_ref,
                                                                _parent);
  }

  /**
   * @brief Generates the schema for the Ref.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::to_schema(
        _definitions);
  }
};

}  // namespace rfl::parsing

#endif
