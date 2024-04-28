#ifndef RFL_PARSING_VIEWREADER_HPP_
#define RFL_PARSING_VIEWREADER_HPP_

#include <array>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Result.hpp"
#include "../internal/is_array.hpp"

namespace rfl::parsing {

template <class R, class W, class ViewType>
class ViewReader {
 private:
  using InputVarType = typename R::InputVarType;
  static constexpr size_t size_ = ViewType::size();

 public:
  ViewReader(const R* _r, const ViewType* _view,
             std::array<bool, size_>* _found, std::array<bool, size_>* _set,
             std::vector<Error>* _errors)
      : r_(_r), view_(_view), found_(_found), set_(_set), errors_(_errors) {}

  ~ViewReader() = default;

  template <size_t _i = 0>
  void read(const std::string_view& _name, const InputVarType& _var) const {
    if constexpr (_i < size_) {
      constexpr auto current_name =
          std::tuple_element_t<_i, typename ViewType::Fields>::name();
      using CurrentType = std::remove_cvref_t<std::remove_pointer_t<
          typename std::tuple_element_t<_i, typename ViewType::Fields>::Type>>;
      if (!std::get<_i>(*found_) && _name == current_name) {
        auto res = Parser<R, W, CurrentType>::read(*r_, _var);
        if (res) {
          move_to(rfl::get<_i>(*view_), &(*res));
          std::get<_i>(*set_) = true;
        } else {
          errors_->push_back(Error("Failed to parse field '" +
                                   std::string(current_name) +
                                   "': " + res.error()->what()));
        }
        std::get<_i>(*found_) = true;
        return;
      }
      read<_i + 1>(_name, _var);
    }
  }

 private:
  template <class Target, class Source>
  void move_to(Target* _t, Source* _s) const {
    if constexpr (std::is_const_v<Target>) {
      return move_to(const_cast<std::remove_const_t<Target>*>(_t), _s);
    } else if constexpr (!internal::is_array_v<Source> &&
                         !std::is_array_v<Target>) {
      ::new (_t) Target(std::move(*_s));
    } else if constexpr (internal::is_array_v<Source>) {
      for (size_t i = 0; i < _s->arr_.size(); ++i) {
        move_to(&((*_t)[i]), &(_s->arr_[i]));
      }
    } else {
      for (size_t i = 0; i < _s->size(); ++i) {
        move_to(&((*_t)[i]), &((*_s)[i]));
      }
    }
  }

 private:
  /// The underlying reader.
  const R* r_;

  /// The underlying view.
  const ViewType* view_;

  /// Indicates that a certain field has been found.
  std::array<bool, size_>* found_;

  /// Indicates that a certain field has been successfully set - necessary,
  /// because we have to trigger the destructors manually.
  std::array<bool, size_>* set_;

  /// Collects any errors we may have come across.
  std::vector<Error>* errors_;
};

}  // namespace rfl::parsing

#endif
