#ifndef RFL_PARSING_PARSER_PAIR_HPP_
#define RFL_PARSING_PARSER_PAIR_HPP_

#include <map>
#include <utility>

#include "../Result.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_pair : std::false_type {};

template <class FirstType, class SecondType>
struct is_pair<std::pair<FirstType, SecondType>> : std::true_type {};

template <class T>
inline constexpr bool is_pair_v = is_pair<std::remove_cvref_t<T>>::value;

template <class R, class W, class PairType, class ProcessorsType>
struct ParserPair {
  using FirstType = typename PairType::first_type;
  using SecondType = typename PairType::second_type;
  using InputVarType = typename R::InputVarType;

  /**
   * @brief Reads a pair from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed pair or an error.
   */
  static Result<std::pair<FirstType, SecondType>> read(
      const R& _r, const InputVarType& _var) noexcept {
    const auto to_pair = [&](auto&& _t) {
      return std::make_pair(std::move(std::get<0>(_t)),
                            std::move(std::get<1>(_t)));
    };
    return Parser<R, W, std::tuple<FirstType, SecondType>,
                  ProcessorsType>::read(_r, _var)
        .transform(to_pair);
  }

  /**
   * @brief Writes a pair to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _p The pair to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const std::pair<FirstType, SecondType>& _p,
                    const P& _parent) {
    const auto tup = std::make_tuple(&_p.first, &_p.second);
    Parser<R, W, std::tuple<const FirstType*, const SecondType*>,
           ProcessorsType>::write(_w, tup, _parent);
  }

  /**
   * @brief Generates the schema for the pair.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::tuple<FirstType, SecondType>,
                  ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace rfl::parsing

#endif
