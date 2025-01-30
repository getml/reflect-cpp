#ifndef RFL_PARSING_TUPLEREADER_HPP_
#define RFL_PARSING_TUPLEREADER_HPP_

#include <sstream>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>

#include "../Result.hpp"
#include "../Tuple.hpp"
#include "../internal/is_array.hpp"

namespace rfl::parsing {

template <class R, class W, class TupleType, bool _ignore_empty_containers,
          bool _all_required, class ProcessorsType>
class TupleReader {
 private:
  using InputVarType = typename R::InputVarType;
  static constexpr size_t size_ = rfl::tuple_size_v<TupleType>;

 public:
  TupleReader(const R* _r, TupleType* _tuple)
      : num_set_(0), r_(_r), tuple_(_tuple) {}

  ~TupleReader() = default;

  std::optional<Error> handle_missing_fields() const {
    std::optional<Error> err;
    if (num_set_ < size_) {
      handle_missing_fields_impl(&err);
    }
    return err;
  }

  size_t num_set() const { return num_set_; }

  std::optional<Error> read(const InputVarType& _var) const {
    std::optional<Error> err;
    read_impl(_var, &err);
    return err;
  }

 private:
  template <int _i = 0>
  void handle_missing_fields_impl(std::optional<Error>* _err) const noexcept {
    if constexpr (_i < size_) {
      if (num_set_ == _i) {
        using CurrentType =
            std::remove_cvref_t<rfl::tuple_element_t<_i, TupleType>>;

        if constexpr (_all_required ||
                      is_required<CurrentType, _ignore_empty_containers>()) {
          *_err = Error("Field " + std::to_string(_i) +
                        " was required, but missing.");
          return;
        } else {
          ::new (&(rfl::get<_i>(*tuple_))) CurrentType();
          ++num_set_;
        }
      }
      handle_missing_fields_impl<_i + 1>(_err);
    }
  }

  template <size_t _i = 0>
  void read_impl(const InputVarType& _var, std::optional<Error>* _err) const {
    if constexpr (_i < size_) {
      if (num_set_ == _i) {
        using CurrentType =
            std::remove_cvref_t<rfl::tuple_element_t<_i, TupleType>>;

        auto res = Parser<R, W, CurrentType, ProcessorsType>::read(*r_, _var);

        if (res) {
          move_to(&(rfl::get<_i>(*tuple_)), &(*res));
          ++num_set_;
        } else {
          std::stringstream stream;
          stream << "Failed to parse field " << _i << ": "
                 << res.error().what();
          *_err = Error(stream.str());
        }
        return;
      }
      read_impl<_i + 1>(_var, _err);
    } else {
      std::stringstream stream;
      stream << "Expected " << size_ << " fields, but got at least one more.";
      *_err = Error(stream.str());
      return;
    }
  }

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
  /// Indicates the last field that was set.
  mutable size_t num_set_;

  /// The underlying reader.
  const R* r_;

  /// The underlying tuple.
  TupleType* tuple_;
};

}  // namespace rfl::parsing

#endif
