#ifndef RFL_PARSING_VECTORREADER_HPP_
#define RFL_PARSING_VECTORREADER_HPP_

#include <type_traits>

#include "../Result.hpp"
#include "is_map_like.hpp"
#include "is_set_like.hpp"

namespace rfl::parsing {

template <class R, class W, class VecType, class ProcessorsType>
class VectorReader {
 private:
  using InputVarType = typename R::InputVarType;
  using T = typename VecType::value_type;

 public:
  VectorReader(const R* _r, VecType* _vec) : r_(_r), vec_(_vec) {}

  ~VectorReader() = default;

  std::optional<Error> read(const InputVarType& _var) const {
    const auto parse = [this](const InputVarType& _var) {
      if constexpr (is_map_like_v<VecType>) {
        return get_pair(_var);
      } else {
        return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(*r_,
                                                                          _var);
      }
    };

    const auto insert = [this](auto&& _var) -> std::optional<Error> {
      if constexpr (is_map_like_v<VecType> || is_set_like_v<VecType>) {
        vec_->insert(std::move(_var));
      } else {
        vec_->emplace_back(std::move(_var));
      }
      return std::nullopt;
    };
    auto res = parse(_var).transform(insert);
    if (res) {
      return std::nullopt;
    } else {
      return res.error();
    }
    // return parse(_var).transform(insert).error();
  }

 private:
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
