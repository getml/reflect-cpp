#ifndef RFL_PARSING_PARSER_BOX_HPP_
#define RFL_PARSING_PARSER_BOX_HPP_

#include <map>
#include <type_traits>

#include "../Box.hpp"
#include "../Result.hpp"
#include "../atomic/is_atomic.hpp"
#include "../atomic/remove_atomic_t.hpp"
#include "../atomic/set_atomic.hpp"
#include "../internal/default_if_missing_v.hpp"
#include "../internal/has_default_val_v.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_box : std::false_type {};

template <class T, Copyability C>
struct is_box<Box<T, C>> : std::true_type {
  using element_type = T;
  static constexpr Copyability copyability = C;
};

template <class T>
constexpr bool is_box_v = is_box<std::remove_cvref_t<T>>::value;

template <class R, class W, class T, Copyability C, class ProcessorsType>
struct ParserBox {
  using InputVarType = typename R::InputVarType;

  /**
   * @brief Reads a Box from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed Box or an error.
   */
  static Result<Box<T, C>> read(const R& _r,
                                const InputVarType& _var) noexcept {
    if constexpr (atomic::is_atomic_v<T>) {
      using RemoveAtomicT = atomic::remove_atomic_t<T>;

      static_assert(!internal::has_default_val_v<RemoveAtomicT>,
                    "Atomic types cannot be mixed with rfl::DefaultVal");
      static_assert(!internal::default_if_missing_v<ProcessorsType>,
                    "Atomic types cannot be mixed with rfl::DefaultIfMissing");

      return Parser<R, W, RemoveAtomicT, ProcessorsType>::read(_r, _var)
          .transform([](auto&& _t) {
            auto atomic_box = Box<T>::make();
            atomic::set_atomic(std::move(_t), &(*atomic_box));
            return atomic_box;
          });

    } else {
      const auto to_box = [](auto&& _t) {
        return Box<T, C>::make(std::move(_t));
      };
      return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r,
                                                                        _var)
          .transform(to_box);
    }
  }

  /**
   * @brief Writes a Box to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _box The Box to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const Box<T, C>& _box, const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, *_box,
                                                                _parent);
  }

  /**
   * @brief Generates the schema for the Box.
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
