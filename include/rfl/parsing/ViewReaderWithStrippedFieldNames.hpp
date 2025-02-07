#ifndef RFL_PARSING_VIEWREADERWITHSTRIPPEDFIELDNAMES_HPP_
#define RFL_PARSING_VIEWREADERWITHSTRIPPEDFIELDNAMES_HPP_

#include <array>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "../Result.hpp"
#include "../Tuple.hpp"
#include "../internal/is_array.hpp"

namespace rfl::parsing {

template <class R, class W, class ViewType, class ProcessorsType>
class ViewReaderWithStrippedFieldNames {
 private:
  using InputVarType = typename R::InputVarType;
  static constexpr size_t size_ = ViewType::size();

 public:
  ViewReaderWithStrippedFieldNames(const R* _r, ViewType* _view,
                                   std::array<bool, size_>* _found,
                                   std::array<bool, size_>* _set,
                                   std::vector<Error>* _errors)
      : i_(0),
        r_(_r),
        view_(_view),
        found_(_found),
        set_(_set),
        errors_(_errors) {}

  ~ViewReaderWithStrippedFieldNames() = default;

  /// Assigns the parsed version of _var to the field signified by i_, to be
  /// used when the field names are stripped.
  std::optional<Error> read(const InputVarType& _var) const {
    if (i_ == size_) {
      std::stringstream stream;
      stream << "Expected a maximum of " << std::to_string(size_)
             << " fields, but got at least one more.";
      return Error(stream.str());
    }
    assign_to_field_i(*r_, _var, view_, errors_, found_, set_, i_,
                      std::make_integer_sequence<int, size_>());
    ++i_;
    return std::nullopt;
  }

 private:
  template <int i>
  static void assign_if_field_is_field_i(const R& _r, const auto& _var,
                                         auto* _view, auto* _errors,
                                         auto* _found, auto* _set, int _i) {
    using FieldType = tuple_element_t<i, typename ViewType::Fields>;
    using OriginalType = typename FieldType::Type;
    using T =
        std::remove_cvref_t<std::remove_pointer_t<typename FieldType::Type>>;
    constexpr auto name = FieldType::name();
    if (_i == i) {
      std::get<i>(*_found) = true;
      auto res = Parser<R, W, T, ProcessorsType>::read(_r, _var);
      if (!res) {
        std::stringstream stream;
        stream << "Failed to parse field '" << std::string(name)
               << "': " << res.error().what();
        _errors->emplace_back(Error(stream.str()));
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
  static void assign_to_field_i(const R& _r, const auto& _var, auto* _view,
                                auto* _errors, auto* _found, auto* _set, int _i,
                                std::integer_sequence<int, is...>) {
    (assign_if_field_is_field_i<is>(_r, _var, _view, _errors, _found, _set, _i),
     ...);
  }

  // TODO: Unnecessary code duplication.
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
  /// Indicates the current field.
  mutable int i_;

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
