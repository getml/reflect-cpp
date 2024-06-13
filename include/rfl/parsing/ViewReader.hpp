#ifndef RFL_PARSING_VIEWREADER_HPP_
#define RFL_PARSING_VIEWREADER_HPP_

#include <array>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "../Result.hpp"
#include "../internal/is_array.hpp"

namespace rfl::parsing {

template <class R, class W, class ViewType, class ProcessorsType>
class ViewReader {
 private:
  using InputVarType = typename R::InputVarType;
  static constexpr size_t size_ = ViewType::size();

 public:
  ViewReader(const R* _r, ViewType* _view, std::array<bool, size_>* _found,
             std::array<bool, size_>* _set, std::vector<Error>* _errors)
      : r_(_r), view_(_view), found_(_found), set_(_set), errors_(_errors) {}

  ~ViewReader() = default;

  /// Assigns the parsed version of _var to the field signified by _name, if
  /// such a field exists in the underlying view.
  void read(const std::string_view& _name, const InputVarType& _var) const {
    assign_to_matching_field(*r_, _name, _var, view_, errors_, found_, set_,
                             std::make_integer_sequence<int, size_>());
  }

  /// Because of the way we have allocated the fields, we need to manually
  /// trigger the destructors.
  void call_destructors_where_necessary() const {
    [&]<int... is>(std::integer_sequence<int, is...>) {
      (call_destructor_on_one_if_necessary<is>(), ...);
    }
    (std::make_integer_sequence<int, size_>());
  }

 private:
  template <int i>
  static void assign_if_field_matches(const R& _r,
                                      const std::string_view& _current_name,
                                      const auto& _var, auto* _view,
                                      auto* _errors, auto* _found, auto* _set,
                                      bool* _already_assigned) {
    using FieldType = std::tuple_element_t<i, typename ViewType::Fields>;
    using OriginalType = typename FieldType::Type;
    using T =
        std::remove_cvref_t<std::remove_pointer_t<typename FieldType::Type>>;
    constexpr auto name = FieldType::name();
    if (!(*_already_assigned) && !std::get<i>(*_found) &&
        _current_name == name) {
      std::get<i>(*_found) = true;
      *_already_assigned = true;
      auto res = Parser<R, W, T, ProcessorsType>::read(_r, _var);
      if (!res) {
        _errors->emplace_back(Error("Failed to parse field '" +
                                    std::string(name) +
                                    "': " + std::move(res.error()->what())));
        return;
      }
      if constexpr (std::is_pointer_v<OriginalType>) {
        move_to(rfl::get<i>(*_view), &(*res));
      } else {
        rfl::get<i>(*_view) = std::move(*res);
      }
      std::get<i>(*_set) = true;
    }
  }

  template <int... is>
  static void assign_to_matching_field(const R& _r,
                                       const std::string_view& _current_name,
                                       const auto& _var, auto* _view,
                                       auto* _errors, auto* _found, auto* _set,
                                       std::integer_sequence<int, is...>) {
    bool already_assigned = false;
    (assign_if_field_matches<is>(_r, _current_name, _var, _view, _errors,
                                 _found, _set, &already_assigned),
     ...);
  }

  template <class T>
  static void call_destructor_on_array(const size_t _size, T* _ptr) {
    for (size_t i = 0; i < _size; ++i) {
      if constexpr (std::is_array_v<T>) {
        call_destructor_on_array(sizeof(*_ptr) / sizeof(**_ptr), *(_ptr + i));
      } else if constexpr (std::is_destructible_v<T>) {
        (_ptr + i)->~T();
      }
    }
  }

  template <int _i>
  void call_destructor_on_one_if_necessary() const {
    using FieldType = std::tuple_element_t<_i, typename ViewType::Fields>;
    using OriginalType = std::remove_cvref_t<typename FieldType::Type>;
    using ValueType =
        std::remove_cvref_t<std::remove_pointer_t<typename FieldType::Type>>;
    if constexpr (!std::is_array_v<ValueType> &&
                  std::is_pointer_v<OriginalType> &&
                  std::is_destructible_v<ValueType>) {
      if (std::get<_i>(*set_)) {
        rfl::get<_i>(*view_)->~ValueType();
      }
    } else if constexpr (std::is_array_v<ValueType>) {
      if (std::get<_i>(*set_)) {
        auto ptr = rfl::get<_i>(*view_);
        call_destructor_on_array(sizeof(*ptr) / sizeof(**ptr), *ptr);
      }
    }
  }

  template <class Target, class Source>
  static void move_to(Target* _t, Source* _s) {
    if constexpr (std::is_const_v<Target>) {
      return move_to(const_cast<std::remove_const_t<Target>*>(_t), _s);
    } else if constexpr (!rfl::internal::is_array_v<Source> &&
                         !std::is_array_v<Target>) {
      ::new (_t) Target(std::move(*_s));
    } else if constexpr (rfl::internal::is_array_v<Source>) {
      static_assert(std::is_array_v<Target>,
                    "Expected target to be a c-array.");
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
  ViewType* view_;

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
