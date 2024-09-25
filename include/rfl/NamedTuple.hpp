#ifndef RFL_NAMEDTUPLE_HPP_
#define RFL_NAMEDTUPLE_HPP_

#include <algorithm>
#include <string_view>
#include <type_traits>
#include <utility>

#include "Field.hpp"
#include "Literal.hpp"
#include "Tuple.hpp"
#include "apply.hpp"
#include "get.hpp"
#include "internal/StringLiteral.hpp"
#include "internal/find_index.hpp"
#include "internal/is_extra_fields.hpp"
#include "make_from_tuple.hpp"
#include "tuple_cat.hpp"

namespace rfl {

/// A named tuple behaves like a tuple,
/// but the fields have explicit names, which
/// allows for reflection.
/// IMPORTANT: We have two template specializations. One with fields, one
/// without fields.
template <class... FieldTypes>
class NamedTuple;

// ----------------------------------------------------------------------------

template <class... FieldTypes>
class NamedTuple {
  template <int _i>
  struct Index {};

  static constexpr auto seq_ =
      std::make_integer_sequence<int, sizeof...(FieldTypes)>();

 public:
  using Fields = rfl::Tuple<std::remove_cvref_t<FieldTypes>...>;
  using Names = Literal<std::remove_cvref_t<FieldTypes>::name_...>;
  using Values = rfl::Tuple<typename std::remove_cvref_t<FieldTypes>::Type...>;

 public:
  /// Construct from the values.
  NamedTuple(typename std::remove_cvref<FieldTypes>::type::Type&&... _values)
      : values_(
            std::forward<typename std::remove_cvref<FieldTypes>::type::Type>(
                _values)...) {}

  /// Construct from the values.
  NamedTuple(
      const typename std::remove_cvref<FieldTypes>::type::Type&... _values)
      : values_(rfl::make_tuple(_values...)) {}

  /// Construct from the fields.
  NamedTuple(FieldTypes&&... _fields)
      : values_(rfl::make_tuple(std::move(_fields.value_)...)) {}

  /// Construct from the fields.
  NamedTuple(const FieldTypes&... _fields)
      : values_(rfl::make_tuple(_fields.value_...)) {}

  /// Construct from a tuple containing fields.
  NamedTuple(rfl::Tuple<FieldTypes...>&& _tup)
      : NamedTuple(rfl::make_from_tuple<NamedTuple<FieldTypes...>>(
            std::forward<rfl::Tuple<FieldTypes...>>(_tup))) {}

  /// Construct from a tuple containing fields.
  NamedTuple(const rfl::Tuple<FieldTypes...>& _tup)
      : NamedTuple(rfl::make_from_tuple<NamedTuple<FieldTypes...>>(_tup)) {}

  /// Copy constructor.
  NamedTuple(const NamedTuple<FieldTypes...>& _other) = default;

  /// Move constructor.
  NamedTuple(NamedTuple<FieldTypes...>&& _other) = default;

  /// Copy constructor.
  template <class... OtherFieldTypes>
  NamedTuple(const NamedTuple<OtherFieldTypes...>& _other)
      : NamedTuple(retrieve_fields(_other.fields(), seq_)) {}

  /// Move constructor.
  template <class... OtherFieldTypes>
  NamedTuple(NamedTuple<OtherFieldTypes...>&& _other)
      : NamedTuple(retrieve_fields(_other.fields(), seq_)) {}

  ~NamedTuple() = default;

  /// Returns a new named tuple with additional fields.
  template <internal::StringLiteral _name, class FType, class... Tail>
  auto add(Field<_name, FType>&& _head, Tail&&... _tail) {
    using Head = Field<_name, FType>;
    if constexpr (sizeof...(Tail) > 0) {
      return NamedTuple<FieldTypes..., std::remove_cvref_t<Head>>(
                 make_fields(seq_, std::forward<Head>(_head)))
          .add(std::forward<Tail>(_tail)...);
    } else {
      return NamedTuple<FieldTypes..., std::remove_cvref_t<Head>>(
          make_fields(seq_, std::forward<Head>(_head)));
    }
  }

  /// Returns a new named tuple with additional fields.
  template <internal::StringLiteral _name, class FType, class... Tail>
  auto add(Field<_name, FType> _head, const Tail&... _tail) const {
    using Head = Field<_name, FType>;
    if constexpr (sizeof...(Tail) > 0) {
      return NamedTuple<FieldTypes..., std::remove_cvref_t<Head>>(
                 make_fields(seq_, _head))
          .add(_tail...);
    } else {
      return NamedTuple<FieldTypes..., std::remove_cvref_t<Head>>(
          make_fields(seq_, _head));
    }
  }

  /// Template specialization for rfl::Tuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto add(rfl::Tuple<TupContent...>&& _tuple, Tail&&... _tail) {
    if constexpr (sizeof...(Tail) > 0) {
      return add_tuple(std::forward<rfl::Tuple<TupContent...>>(_tuple))
          .add(std::forward<Tail>(_tail)...);
    } else {
      return add_tuple(std::forward<rfl::Tuple<TupContent...>>(_tuple));
    }
  }

  /// Template specialization for rfl::Tuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto add(rfl::Tuple<TupContent...> _tuple, const Tail&... _tail) const {
    if constexpr (sizeof...(Tail) > 0) {
      return add_tuple(std::move(_tuple)).add(_tail...);
    } else {
      return add_tuple(std::move(_tuple));
    }
  }

  /// Template specialization for NamedTuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto add(NamedTuple<TupContent...>&& _named_tuple, Tail&&... _tail) {
    return add(std::forward<rfl::Tuple<TupContent...>>(_named_tuple.fields()),
               std::forward<Tail>(_tail)...);
  }

  /// Template specialization for NamedTuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto add(NamedTuple<TupContent...> _named_tuple, const Tail&... _tail) const {
    return add(_named_tuple.fields(), _tail...);
  }

  /// Creates a new named tuple by applying the supplied function to
  /// field. The function is expected to return a named tuple itself.
  template <typename F>
  auto and_then(const F& _f) {
    const auto transform_field = [&_f](auto... _fields) {
      return rfl::tuple_cat(_f(std::move(_fields)).fields()...);
    };
    const auto to_nt = []<class... NewFields>(rfl::Tuple<NewFields...>&& _tup) {
      return NamedTuple<NewFields...>(_tup);
    };
    auto new_fields = rfl::apply(transform_field, std::move(fields()));
    return to_nt(std::move(new_fields));
  }

  /// Creates a new named tuple by applying the supplied function to
  /// field. The function is expected to return a named tuple itself.
  template <typename F>
  auto and_then(const F& _f) const {
    const auto transform_field = [&_f](auto... _fields) {
      return rfl::tuple_cat(_f(std::move(_fields)).fields()...);
    };
    const auto to_nt = []<class... NewFields>(rfl::Tuple<NewFields...>&& _tup) {
      return NamedTuple<NewFields...>(_tup);
    };
    auto new_fields = rfl::apply(transform_field, std::move(fields()));
    return to_nt(std::move(new_fields));
  }

  /// Invokes a callable object once for each field in order.
  template <typename F>
  void apply(F&& _f) {
    const auto apply_to_field =
        [&_f]<typename... AFields>(AFields&&... fields) {
          ((_f(std::forward<AFields>(fields))), ...);
        };
    rfl::apply(apply_to_field, fields());
  }

  /// Invokes a callable object once for each field in order.
  template <typename F>
  void apply(F&& _f) const {
    const auto apply_to_field = [&_f](const auto&... fields) {
      ((_f(fields)), ...);
    };
    rfl::apply(apply_to_field, fields());
  }

  /// Returns a tuple containing the fields.
  Fields fields() { return make_fields(seq_); }

  /// Returns a tuple containing the fields.
  Fields fields() const { return make_fields(seq_); }

  /// Gets a field by index.
  template <int _index>
  auto& get() {
    return rfl::get<_index>(*this);
  }

  /// Gets a field by name.
  template <internal::StringLiteral _field_name>
  auto& get() {
    return rfl::get<_field_name>(*this);
  }

  /// Gets a field by the field type.
  template <class Field>
  auto& get() {
    return rfl::get<Field>(*this);
  }

  /// Gets a field by index.
  template <int _index>
  const auto& get() const {
    return rfl::get<_index>(*this);
  }

  /// Gets a field by name.
  template <internal::StringLiteral _field_name>
  const auto& get() const {
    return rfl::get<_field_name>(*this);
  }

  /// Gets a field by the field type.
  template <class Field>
  const auto& get() const {
    return rfl::get<Field>(*this);
  }

  /// Returns the results wrapped in a field.
  template <internal::StringLiteral _field_name>
  auto get_field() const {
    return rfl::make_field<_field_name>(rfl::get<_field_name>(*this));
  }

  /// Copy assignment operator.
  NamedTuple<FieldTypes...>& operator=(
      const NamedTuple<FieldTypes...>& _other) = default;

  /// Move assignment operator.
  NamedTuple<FieldTypes...>& operator=(
      NamedTuple<FieldTypes...>&& _other) noexcept = default;

  /// Equality operator
  inline auto operator==(const rfl::NamedTuple<FieldTypes...>& _other) const {
    return values() == _other.values();
  }

  /// Three-way comparison operator.
  inline auto operator<=>(const rfl::NamedTuple<FieldTypes...>& _other) const {
    return values() <=> _other.values();
  }

  /// Returns the number of fields. Note that this is not necessary the same
  /// thing as .size(), because there might be rfl::ExtraFields, which are
  /// simply counted as one entry by .size(), but are counted by individually by
  /// .num_fields().
  size_t num_fields() const {
    if constexpr (pos_extra_fields() == -1) {
      return size();
    } else {
      return calc_num_fields<pos_extra_fields()>();
    }
  }

  /// The position of the extra fields, or -1 if there aren't any.
  constexpr static int pos_extra_fields() { return pos_extra_fields_; }

  /// Replaces one or several fields, returning a new version
  /// with the non-replaced fields left unchanged.
  template <internal::StringLiteral _name, class FType, class... OtherRFields>
  auto replace(Field<_name, FType>&& _field, OtherRFields&&... _other_fields) {
    using RField = Field<_name, FType>;
    constexpr auto num_other_fields = sizeof...(OtherRFields);
    if constexpr (num_other_fields == 0) {
      return replace_value<RField>(_field.value_);
    } else {
      return replace_value<RField>(_field.value_)
          .replace(std::forward<OtherRFields>(_other_fields)...);
    }
  }

  /// Replaces one or several fields, returning a new version
  /// with the non-replaced fields left unchanged.
  template <internal::StringLiteral _name, class FType, class... OtherRFields>
  auto replace(Field<_name, FType> _field,
               const OtherRFields&... _other_fields) const {
    using RField = Field<_name, FType>;
    constexpr auto num_other_fields = sizeof...(OtherRFields);
    if constexpr (num_other_fields == 0) {
      return replace_value<RField>(std::move(_field.value_));
    } else {
      return replace_value<RField>(std::move(_field.value_))
          .replace(_other_fields...);
    }
  }

  /// Template specialization for rfl::Tuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto replace(rfl::Tuple<TupContent...>&& _tuple, Tail&&... _tail) {
    if constexpr (sizeof...(Tail) > 0) {
      return replace_tuple(std::forward<rfl::Tuple<TupContent...>>(_tuple))
          .replace(std::forward<Tail>(_tail)...);
    } else {
      return replace_tuple(std::forward<rfl::Tuple<TupContent...>>(_tuple));
    }
  }

  /// Template specialization for rfl::Tuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto replace(rfl::Tuple<TupContent...> _tuple, const Tail&... _tail) const {
    if constexpr (sizeof...(Tail) > 0) {
      return replace_tuple(std::move(_tuple)).replace(_tail...);
    } else {
      return replace_tuple(std::move(_tuple));
    }
  }

  /// Template specialization for NamedTuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto replace(NamedTuple<TupContent...>&& _named_tuple, Tail&&... _tail) {
    return replace(
        std::forward<NamedTuple<TupContent...>>(_named_tuple).fields(),
        std::forward<Tail>(_tail)...);
  }

  /// Template specialization for NamedTuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto replace(NamedTuple<TupContent...> _named_tuple,
               const Tail&... _tail) const {
    return replace(_named_tuple.fields(), _tail...);
  }

  /// Returns the size of the named tuple
  static constexpr size_t size() { return rfl::tuple_size_v<Values>; }

  /// Creates a new named tuple by applying the supplied function to every
  /// field.
  template <typename F>
  auto transform(const F& _f) {
    const auto transform_field = [&_f](auto... fields) {
      return rfl::make_tuple(_f(std::move(fields))...);
    };
    const auto to_nt = []<class... NewFields>(rfl::Tuple<NewFields...>&& _tup) {
      return NamedTuple<NewFields...>(_tup);
    };
    auto new_fields = rfl::apply(transform_field, std::move(fields()));
    return to_nt(std::move(new_fields));
  }

  /// Creates a new named tuple by applying the supplied function to every
  /// field.
  template <typename F>
  auto transform(const F& _f) const {
    const auto transform_field = [&_f](auto... fields) {
      return rfl::make_tuple(_f(std::move(fields))...);
    };
    const auto to_nt = []<class... NewFields>(rfl::Tuple<NewFields...>&& _tup) {
      return NamedTuple<NewFields...>(_tup);
    };
    auto new_fields = rfl::apply(transform_field, std::move(fields()));
    return to_nt(std::move(new_fields));
  }

  /// Returns the underlying rfl::Tuple.
  Values& values() { return values_; }

  /// Returns the underlying rfl::Tuple.
  const Values& values() const { return values_; }

 private:
  /// Adds the elements of a tuple to a newly created named tuple,
  /// and other elements to a newly created named tuple.
  template <class... TupContent>
  constexpr auto add_tuple(rfl::Tuple<TupContent...>&& _tuple) {
    const auto a = [this](auto&&... _fields) {
      return this->add(std::forward<TupContent>(_fields)...);
    };
    return rfl::apply(a, std::forward<rfl::Tuple<TupContent...>>(_tuple));
  }

  /// Adds the elements of a tuple to a newly created named tuple,
  /// and other elements to a newly created named tuple.
  template <class... TupContent>
  constexpr auto add_tuple(rfl::Tuple<TupContent...>&& _tuple) const {
    const auto a = [this](auto&&... _fields) {
      return this->add(std::forward<TupContent>(_fields)...);
    };
    return rfl::apply(a, std::forward<rfl::Tuple<TupContent...>>(_tuple));
  }

  /// Unfortunately, MSVC forces us to do this...
  template <int _pos>
  size_t calc_num_fields() const {
    const auto& extra_fields = get<_pos>();
    if constexpr (std::is_pointer_v<
                      std::remove_cvref_t<decltype(extra_fields)>>) {
      return size() + extra_fields->size() - 1;
    } else {
      return size() + extra_fields.size() - 1;
    }
  }

  /// Finds the position of the extra fields, or -1 if there aren't any.
  template <int _i = 0, int _idx = -1>
  constexpr static int find_extra_fields() {
    if constexpr (_i == size()) {
      return _idx;
    } else {
      using FieldType = internal::nth_element_t<_i, FieldTypes...>;
      constexpr bool is_extra_fields =
          internal::is_extra_fields_v<typename FieldType::Type>;
      static_assert(_idx == -1 || !is_extra_fields,
                    "There can only be one rfl::ExtraFields in any struct or "
                    "named tuple.");
      if constexpr (is_extra_fields) {
        return find_extra_fields<_i + 1, _i>();
      } else {
        return find_extra_fields<_i + 1, _idx>();
      }
    }
  }

  /// Generates the fields.
  template <int... _is, class... AdditionalArgs>
  auto make_fields(std::integer_sequence<int, _is...>,
                   AdditionalArgs&&... _args) {
    const auto wrap = [this]<int _i>(Index<_i>) {
      using FieldType = internal::nth_element_t<_i, FieldTypes...>;
      using T = std::remove_cvref_t<typename FieldType::Type>;
      return FieldType(std::forward<T>(rfl::get<_i>(values_)));
    };
    return rfl::make_tuple(wrap(Index<_is>{})...,
                           std::forward<AdditionalArgs>(_args)...);
  }

  /// Generates the fields.
  template <int... _is, class... AdditionalArgs>
  auto make_fields(std::integer_sequence<int, _is...>,
                   AdditionalArgs... _args) const {
    const auto wrap = [this]<int _i>(Index<_i>) {
      using FieldType = internal::nth_element_t<_i, FieldTypes...>;
      return FieldType(rfl::get<_i>(values_));
    };
    return rfl::make_tuple(wrap(Index<_is>{})..., _args...);
  }

  /// Generates a new named tuple with one value replaced with a new value.
  template <int _index, class V, class T, int... _is>
  auto make_replaced(V&& _values, T&& _val,
                     std::integer_sequence<int, _is...>) const {
    const auto wrap = [&]<int _i>(Index<_i>) {
      if constexpr (_i == _index) {
        return std::forward<T>(_val);
      } else {
        using FieldType = internal::nth_element_t<_i, FieldTypes...>;
        using U = typename FieldType::Type;
        return FieldType(std::forward<U>(rfl::get<_i>(_values)));
      }
    };
    return NamedTuple<FieldTypes...>(wrap(Index<_is>{})...);
  }

  /// Replaced the field signified by the field type.
  template <class Field, class T>
  NamedTuple<FieldTypes...> replace_value(T&& _val) {
    using FieldType = std::remove_cvref_t<Field>;
    constexpr auto index = internal::find_index<FieldType::name_, Fields>();
    return make_replaced<index>(std::forward<Values>(values_),
                                std::forward<T>(_val), seq_);
  }

  /// Replaced the field signified by the field type.
  template <class Field, class T>
  NamedTuple<FieldTypes...> replace_value(T&& _val) const {
    using FieldType = std::remove_cvref_t<Field>;
    constexpr auto index = internal::find_index<FieldType::name_, Fields>();
    auto values = values_;
    return make_replaced<index>(std::move(values), std::forward<T>(_val), seq_);
  }

  /// Adds the elements of a tuple to a newly created named tuple,
  /// and other elements to a newly created named tuple.
  template <class... TupContent>
  auto replace_tuple(rfl::Tuple<TupContent...>&& _tuple) {
    const auto r = [this](auto&&... _fields) {
      return this->replace(std::forward<TupContent>(_fields)...);
    };
    return rfl::apply(r, std::forward<rfl::Tuple<TupContent...>>(_tuple));
  }

  /// Adds the elements of a tuple to a newly created named tuple,
  /// and other elements to a newly created named tuple.
  template <class... TupContent>
  auto replace_tuple(rfl::Tuple<TupContent...>&& _tuple) const {
    const auto r = [this](auto&&... _fields) {
      return this->replace(std::forward<TupContent>(_fields)...);
    };
    return rfl::apply(r, std::forward<rfl::Tuple<TupContent...>>(_tuple));
  }

  /// Retrieves the fields from another tuple.
  template <class... OtherFieldTypes, int... _is>
  constexpr static Fields retrieve_fields(
      rfl::Tuple<OtherFieldTypes...>&& _other_fields,
      std::integer_sequence<int, _is...>) {
    const auto get_field = [&]<int _i>(Index<_i>) {
      constexpr auto field_name =
          internal::nth_element_t<_i, FieldTypes...>::name_;
      constexpr auto index =
          internal::find_index<field_name, rfl::Tuple<OtherFieldTypes...>>();
      using FieldType = internal::nth_element_t<_i, FieldTypes...>;
      using T = std::remove_cvref_t<typename FieldType::Type>;
      return FieldType(std::forward<T>(rfl::get<index>(_other_fields).value_));
    };
    return rfl::make_tuple(get_field(Index<_is>{})...);
  }

 private:
  /// The values actually contained in the named tuple.
  /// As you can see, a NamedTuple is just a normal tuple under-the-hood,
  /// everything else is resolved at compile time. It should have no
  /// runtime overhead over a normal rfl::Tuple.
  Values values_;

  /// The position of rfl::ExtraFields, or -1 if there aren't any.
  constexpr static int pos_extra_fields_ = find_extra_fields();
};

// ----------------------------------------------------------------------------

/// We need a special template instantiation for empty named tuples.
template <>
class NamedTuple<> {
 public:
  using Fields = rfl::Tuple<>;
  using Names = Literal<>;
  using Values = rfl::Tuple<>;

  NamedTuple(){};

  ~NamedTuple() = default;

  /// Returns a new named tuple with additional fields.
  template <internal::StringLiteral _name, class FType, class... Tail>
  auto add(Field<_name, FType> _head, const Tail&... _tail) const {
    if constexpr (sizeof...(Tail) > 0) {
      return NamedTuple<Field<_name, FType>>(std::move(_head)).add(_tail...);
    } else {
      return NamedTuple<Field<_name, FType>>(std::move(_head));
    }
  }

  /// Template specialization for rfl::Tuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto add(rfl::Tuple<TupContent...> _tuple, const Tail&... _tail) const {
    if constexpr (sizeof...(Tail) > 0) {
      return NamedTuple<TupContent...>(std::move(_tuple)).add(_tail...);
    } else {
      return NamedTuple<TupContent...>(std::move(_tuple));
    }
  }

  /// Template specialization for NamedTuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto add(NamedTuple<TupContent...> _named_tuple, const Tail&... _tail) const {
    return add(_named_tuple.fields(), _tail...);
  }

  /// Returns an empty named tuple.
  template <typename F>
  auto and_then(const F&) const {
    return NamedTuple<>();
  }

  /// Does nothing at all.
  template <typename F>
  void apply(F&&) const {}

  /// Returns an empty tuple.
  auto fields() const { return rfl::Tuple(); }

  /// Must always be 0.
  size_t num_fields() const { return 0; }

  /// Must always be -1.
  constexpr static int pos_extra_fields() { return -1; }

  /// Must always be 0.
  static constexpr size_t size() { return 0; }

  /// Returns an empty named tuple.
  template <typename F>
  auto transform(const F&) const {
    return NamedTuple<>();
  }

  /// Returns an empty tuple.
  auto values() const { return rfl::Tuple(); }
};

// ----------------------------------------------------------------------------

template <internal::StringLiteral _name1, class Type1,
          internal::StringLiteral _name2, class Type2>
inline auto operator*(const rfl::Field<_name1, Type1>& _f1,
                      const rfl::Field<_name2, Type2>& _f2) {
  return NamedTuple(_f1, _f2);
}

template <internal::StringLiteral _name, class Type, class... FieldTypes>
inline auto operator*(const NamedTuple<FieldTypes...>& _tup,
                      const rfl::Field<_name, Type>& _f) {
  return _tup.add(_f);
}

template <internal::StringLiteral _name, class Type, class... FieldTypes>
inline auto operator*(const rfl::Field<_name, Type>& _f,
                      const NamedTuple<FieldTypes...>& _tup) {
  return NamedTuple(_f).add(_tup);
}

template <class... FieldTypes1, class... FieldTypes2>
inline auto operator*(const NamedTuple<FieldTypes1...>& _tup1,
                      const NamedTuple<FieldTypes2...>& _tup2) {
  return _tup1.add(_tup2);
}

template <internal::StringLiteral _name1, class Type1,
          internal::StringLiteral _name2, class Type2>
inline auto operator*(rfl::Field<_name1, Type1>&& _f1,
                      rfl::Field<_name2, Type2>&& _f2) {
  return NamedTuple(std::forward<Field<_name1, Type1>>(_f1),
                    std::forward<Field<_name2, Type2>>(_f2));
}

template <internal::StringLiteral _name, class Type, class... FieldTypes>
inline auto operator*(NamedTuple<FieldTypes...>&& _tup,
                      rfl::Field<_name, Type>&& _f) {
  return _tup.add(std::forward<Field<_name, Type>>(_f));
}

template <internal::StringLiteral _name, class Type, class... FieldTypes>
inline auto operator*(rfl::Field<_name, Type>&& _f,
                      NamedTuple<FieldTypes...>&& _tup) {
  return NamedTuple(std::forward<Field<_name, Type>>(_f))
      .add(std::forward<NamedTuple<FieldTypes...>>(_tup));
}

template <class... FieldTypes1, class... FieldTypes2>
inline auto operator*(NamedTuple<FieldTypes1...>&& _tup1,
                      NamedTuple<FieldTypes2...>&& _tup2) {
  return _tup1.add(std::forward<NamedTuple<FieldTypes2...>>(_tup2));
}

}  // namespace rfl

#endif  // RFL_NAMEDTUPLE_HPP_
