#ifndef RFL_PARSING_VECTORREADER_HPP_
#define RFL_PARSING_VECTORREADER_HPP_

#include <cstddef>
#include <exception>
#include <optional>
#include <string>
#include <type_traits>

#include "../Result.hpp"
#include "../internal/always_inline.hpp"
#include "Parser_base.hpp"
#include "is_inplace_vector.hpp"
#include "is_map_like.hpp"
#include "is_set_like.hpp"

namespace rfl::parsing {

template <class R, class W, class VecType, class ProcessorsType>
class VectorReader {
 private:
  using InputVarType = typename R::InputVarType;
  using T = typename VecType::value_type;

 public:
  /**
   * @brief Constructor.
   *
   * @param _r The reader to use.
   * @param _vec The vector to read into.
   */
  VectorReader(const R* _r, VecType* _vec) : r_(_r), vec_(_vec) {}

  ~VectorReader() = default;

  std::optional<Error> reserve(const std::size_t _size) const noexcept {
    if constexpr (requires { vec_->reserve(_size); } &&
                  !is_inplace_vector_v<VecType> && !is_set_like_v<VecType> &&
                  !is_map_like_v<VecType>) {
      try {
        vec_->reserve(_size);
      } catch (const std::exception& e) {
        return Error(e.what());
      }
    }
    return std::nullopt;
  }

  /**
   * @brief Reads a single variable from the input.
   *
   * @param _var The input variable to read from.
   * @return An optional error.
   */
  RFL_ALWAYS_INLINE std::optional<Error> read(const InputVarType& _var) const {
    const auto insert = [this](auto&& _var) -> std::optional<Error> {
      if constexpr (is_map_like_v<VecType> || is_set_like_v<VecType>) {
        vec_->insert(std::move(_var));
      } else if constexpr (is_inplace_vector_v<VecType>) {
        if (!vec_->try_emplace_back(std::move(_var))) {
          return Error("Too many elements, capacity is " +
                       std::to_string(vec_->capacity()) + ".");
        }
      } else {
        vec_->emplace_back(std::move(_var));
      }
      return std::nullopt;
    };
    auto res = parse(_var).transform(insert);
    if (res) {
      return *res;
    } else {
      return res.error();
    }
    // return parse(_var).transform(insert).error();
  }

 private:
  RFL_ALWAYS_INLINE auto parse(const InputVarType& _var) const {
    if constexpr (is_map_like_v<VecType>) {
      return get_pair(_var);
    } else {
      return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(*r_, _var);
    }
  }

  auto get_pair(const auto& _var) const {
    using K = std::remove_cvref_t<typename T::first_type>;
    using V = std::remove_cvref_t<typename T::second_type>;
    return Parser<R, W, std::remove_cvref_t<std::pair<K, V>>,
                  ProcessorsType>::read(*r_, _var);
  }

 private:
  /// The underlying reader.
  const R* r_;

  /// The underlying vector.
  VecType* vec_;
};

}  // namespace rfl::parsing

#endif
