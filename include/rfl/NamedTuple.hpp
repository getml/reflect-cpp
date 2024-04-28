#ifndef RFL_NAMEDTUPLE_HPP_
#define RFL_NAMEDTUPLE_HPP_

#include <algorithm>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "Field.hpp"
#include "Literal.hpp"
#include "get.hpp"
#include "internal/StringLiteral.hpp"
#include "internal/find_index.hpp"
#include "internal/no_duplicate_field_names.hpp"

namespace rfl {

/// A named tuple behaves like std::tuple,
/// but the fields have explicit names, which
/// allows for reflection.
/// IMPORTANT: We have two template specializations. One with fields, one
/// without fields.
template <class... FieldTypes>
class NamedTuple;

// ----------------------------------------------------------------------------

template <class... FieldTypes>
class NamedTuple {
 public:
  using Fields = std::tuple<std::remove_cvref_t<FieldTypes>...>;
  using Names = Literal<std::remove_cvref_t<FieldTypes>::name_...>;
  using Values = std::tuple<typename std::remove_cvref_t<FieldTypes>::Type...>;

 public:
  /// Construct from the values.
  NamedTuple(typename std::remove_cvref<FieldTypes>::type::Type&&... _values)
      : values_(
            std::forward<typename std::remove_cvref<FieldTypes>::type::Type>(
                _values)...) {
    static_assert(no_duplicate_field_names(),
                  "Duplicate field names are not allowed");
  }

  /// Construct from the values.
  NamedTuple(
      const typename std::remove_cvref<FieldTypes>::type::Type&... _values)
      : values_(std::make_tuple(_values...)) {
    static_assert(no_duplicate_field_names(),
                  "Duplicate field names are not allowed");
  }

  /// Construct from the fields.
  NamedTuple(FieldTypes&&... _fields)
      : values_(std::make_tuple(std::move(_fields.value_)...)) {
    static_assert(no_duplicate_field_names(),
                  "Duplicate field names are not allowed");
  }

  /// Construct from the fields.
  NamedTuple(const FieldTypes&... _fields)
      : values_(std::make_tuple(_fields.value_...)) {
    static_assert(no_duplicate_field_names(),
                  "Duplicate field names are not allowed");
  }

  /// Construct from a tuple containing fields.
  NamedTuple(std::tuple<FieldTypes...>&& _tup)
      : NamedTuple(std::make_from_tuple<NamedTuple<FieldTypes...>>(
            std::forward<std::tuple<FieldTypes...>>(_tup))) {
    static_assert(no_duplicate_field_names(),
                  "Duplicate field names are not allowed");
  }

  /// Construct from a tuple containing fields.
  NamedTuple(const std::tuple<FieldTypes...>& _tup)
      : NamedTuple(std::make_from_tuple<NamedTuple<FieldTypes...>>(_tup)) {
    static_assert(no_duplicate_field_names(),
                  "Duplicate field names are not allowed");
  }

  /// Copy constructor.
  NamedTuple(const NamedTuple<FieldTypes...>& _other) = default;

  /// Move constructor.
  NamedTuple(NamedTuple<FieldTypes...>&& _other) = default;

  /// Copy constructor.
  template <class... OtherFieldTypes>
  NamedTuple(const NamedTuple<OtherFieldTypes...>& _other)
      : NamedTuple(retrieve_fields(_other.fields())) {
    static_assert(no_duplicate_field_names(),
                  "Duplicate field names are not allowed");
  }

  /// Move constructor.
  template <class... OtherFieldTypes>
  NamedTuple(NamedTuple<OtherFieldTypes...>&& _other)
      : NamedTuple(retrieve_fields(_other.fields())) {
    static_assert(no_duplicate_field_names(),
                  "Duplicate field names are not allowed");
  }

  ~NamedTuple() = default;

  /// Returns a new named tuple with additional fields.
  template <internal::StringLiteral _name, class FType, class... Tail>
  auto add(Field<_name, FType>&& _head, Tail&&... _tail) {
    using Head = Field<_name, FType>;
    if constexpr (sizeof...(Tail) > 0) {
      return NamedTuple<FieldTypes..., std::remove_cvref_t<Head>>(
                 make_fields<1, Head>(std::forward<Head>(_head)))
          .add(std::forward<Tail>(_tail)...);
    } else {
      return NamedTuple<FieldTypes..., std::remove_cvref_t<Head>>(
          make_fields<1, Head>(std::forward<Head>(_head)));
    }
  }

  /// Returns a new named tuple with additional fields.
  template <internal::StringLiteral _name, class FType, class... Tail>
  auto add(Field<_name, FType> _head, const Tail&... _tail) const {
    using Head = Field<_name, FType>;
    if constexpr (sizeof...(Tail) > 0) {
      return NamedTuple<FieldTypes..., std::remove_cvref_t<Head>>(
                 make_fields<1, Head>(_head))
          .add(_tail...);
    } else {
      return NamedTuple<FieldTypes..., std::remove_cvref_t<Head>>(
          make_fields<1, Head>(_head));
    }
  }

  /// Template specialization for std::tuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto add(std::tuple<TupContent...>&& _tuple, Tail&&... _tail) {
    if constexpr (sizeof...(Tail) > 0) {
      return add_tuple(std::forward<std::tuple<TupContent...>>(_tuple))
          .add(std::forward<Tail>(_tail)...);
    } else {
      return add_tuple(std::forward<std::tuple<TupContent...>>(_tuple));
    }
  }

  /// Template specialization for std::tuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto add(std::tuple<TupContent...> _tuple, const Tail&... _tail) const {
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
    return add(std::forward<std::tuple<TupContent...>>(_named_tuple.fields()),
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
      return std::tuple_cat(_f(std::move(_fields)).fields()...);
    };
    const auto to_nt = []<class... NewFields>(std::tuple<NewFields...>&& _tup) {
      return NamedTuple<NewFields...>(_tup);
    };
    auto new_fields = std::apply(transform_field, std::move(fields()));
    return to_nt(std::move(new_fields));
  }

  /// Creates a new named tuple by applying the supplied function to
  /// field. The function is expected to return a named tuple itself.
  template <typename F>
  auto and_then(const F& _f) const {
    const auto transform_field = [&_f](auto... _fields) {
      return std::tuple_cat(_f(std::move(_fields)).fields()...);
    };
    const auto to_nt = []<class... NewFields>(std::tuple<NewFields...>&& _tup) {
      return NamedTuple<NewFields...>(_tup);
    };
    auto new_fields = std::apply(transform_field, std::move(fields()));
    return to_nt(std::move(new_fields));
  }

  /// Invokes a callable object once for each field in order.
  template <typename F>
  void apply(F&& _f) {
    const auto apply_to_field =
        [&_f]<typename... AFields>(AFields&&... fields) {
          ((_f(std::forward<AFields>(fields))), ...);
        };
    std::apply(apply_to_field, fields());
  }

  /// Invokes a callable object once for each field in order.
  template <typename F>
  void apply(F&& _f) const {
    const auto apply_to_field = [&_f](const auto&... fields) {
      ((_f(fields)), ...);
    };
    std::apply(apply_to_field, fields());
  }

  /// Returns a tuple containing the fields.
  Fields fields() { return make_fields(); }

  /// Returns a tuple containing the fields.
  Fields fields() const { return make_fields(); }

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

  /// Inequality operator
  inline auto operator!=(const rfl::NamedTuple<FieldTypes...>& _other) const {
    return !(*this == _other);
  }
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

  /// Template specialization for std::tuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto replace(std::tuple<TupContent...>&& _tuple, Tail&&... _tail) {
    if constexpr (sizeof...(Tail) > 0) {
      return replace_tuple(std::forward<std::tuple<TupContent...>>(_tuple))
          .replace(std::forward<Tail>(_tail)...);
    } else {
      return replace_tuple(std::forward<std::tuple<TupContent...>>(_tuple));
    }
  }

  /// Template specialization for std::tuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto replace(std::tuple<TupContent...> _tuple, const Tail&... _tail) const {
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
  static constexpr size_t size() { return std::tuple_size_v<Values>; }

  /// Creates a new named tuple by applying the supplied function to every
  /// field.
  template <typename F>
  auto transform(const F& _f) {
    const auto transform_field = [&_f](auto... fields) {
      return std::make_tuple(_f(std::move(fields))...);
    };
    const auto to_nt = []<class... NewFields>(std::tuple<NewFields...>&& _tup) {
      return NamedTuple<NewFields...>(_tup);
    };
    auto new_fields = std::apply(transform_field, std::move(fields()));
    return to_nt(std::move(new_fields));
  }

  /// Creates a new named tuple by applying the supplied function to every
  /// field.
  template <typename F>
  auto transform(const F& _f) const {
    const auto transform_field = [&_f](auto... fields) {
      return std::make_tuple(_f(std::move(fields))...);
    };
    const auto to_nt = []<class... NewFields>(std::tuple<NewFields...>&& _tup) {
      return NamedTuple<NewFields...>(_tup);
    };
    auto new_fields = std::apply(transform_field, std::move(fields()));
    return to_nt(std::move(new_fields));
  }

  /// Returns the underlying std::tuple.
  Values& values() { return values_; }

  /// Returns the underlying std::tuple.
  const Values& values() const { return values_; }

 private:
  /// Adds the elements of a tuple to a newly created named tuple,
  /// and other elements to a newly created named tuple.
  template <class... TupContent>
  constexpr auto add_tuple(std::tuple<TupContent...>&& _tuple) {
    const auto a = [this](auto&&... _fields) {
      return this->add(std::forward<TupContent>(_fields)...);
    };
    return std::apply(a, std::forward<std::tuple<TupContent...>>(_tuple));
  }

  /// Adds the elements of a tuple to a newly created named tuple,
  /// and other elements to a newly created named tuple.
  template <class... TupContent>
  constexpr auto add_tuple(std::tuple<TupContent...>&& _tuple) const {
    const auto a = [this](auto&&... _fields) {
      return this->add(std::forward<TupContent>(_fields)...);
    };
    return std::apply(a, std::forward<std::tuple<TupContent...>>(_tuple));
  }

  /// Generates the fields.
  template <int num_additional_fields = 0, class... Args>
  auto make_fields(Args&&... _args) {
    constexpr auto size = sizeof...(Args) - num_additional_fields;
    constexpr auto num_fields = std::tuple_size_v<Fields>;
    constexpr auto i = num_fields - size - 1;

    constexpr bool retrieved_all_fields = size == num_fields;

    if constexpr (retrieved_all_fields) {
      return std::make_tuple(std::forward<Args>(_args)...);
    } else {
      // When we add additional fields, it is more intuitive to add
      // them to the end, that is why we do it like this.
      using FieldType = typename std::tuple_element<i, Fields>::type;
      using T = std::remove_cvref_t<typename FieldType::Type>;
      return make_fields<num_additional_fields>(
          FieldType(std::forward<T>(std::get<i>(values_))),
          std::forward<Args>(_args)...);
    }
  }

  /// Generates the fields.
  template <int num_additional_fields = 0, class... Args>
  auto make_fields(Args... _args) const {
    constexpr auto size = sizeof...(Args) - num_additional_fields;
    constexpr auto num_fields = std::tuple_size_v<Fields>;
    constexpr auto i = num_fields - size - 1;

    constexpr bool retrieved_all_fields = size == num_fields;

    if constexpr (retrieved_all_fields) {
      return std::make_tuple(std::move(_args)...);
    } else {
      // When we add additional fields, it is more intuitive to add
      // them to the end, that is why we do it like this.
      using FieldType = typename std::tuple_element<i, Fields>::type;
      return make_fields<num_additional_fields>(FieldType(std::get<i>(values_)),
                                                std::move(_args)...);
    }
  }

  /// Generates a new named tuple with one value replaced with a new value.
  template <int _index, class V, class T, class... Args>
  auto make_replaced(V&& _values, T&& _val, Args&&... _args) const {
    constexpr auto size = sizeof...(Args);

    constexpr bool retrieved_all_fields = size == std::tuple_size_v<Fields>;

    if constexpr (retrieved_all_fields) {
      return NamedTuple<FieldTypes...>(std::forward<Args>(_args)...);
    } else {
      using FieldType = typename std::tuple_element<size, Fields>::type;

      if constexpr (size == _index) {
        return make_replaced<_index, V, T>(
            std::forward<V>(_values), std::forward<T>(_val),
            std::forward<Args>(_args)..., FieldType(std::forward<T>(_val)));
      } else {
        using U = typename FieldType::Type;
        return make_replaced<_index, V, T>(
            std::forward<V>(_values), std::forward<T>(_val),
            std::forward<Args>(_args)...,
            FieldType(std::forward<U>(std::get<size>(_values))));
      }
    }
  }

  /// We cannot allow duplicate field names.
  constexpr static bool no_duplicate_field_names() {
    return internal::no_duplicate_field_names<Fields>();
  }

  /// Replaced the field signified by the field type.
  template <class Field, class T>
  NamedTuple<FieldTypes...> replace_value(T&& _val) {
    using FieldType = std::remove_cvref_t<Field>;
    constexpr auto index = internal::find_index<FieldType::name_, Fields>();
    return make_replaced<index, Values, T>(std::forward<Values>(values_),
                                           std::forward<T>(_val));
  }

  /// Replaced the field signified by the field type.
  template <class Field, class T>
  NamedTuple<FieldTypes...> replace_value(T&& _val) const {
    using FieldType = std::remove_cvref_t<Field>;
    constexpr auto index = internal::find_index<FieldType::name_, Fields>();
    auto values = values_;
    return make_replaced<index, Values, T>(std::move(values),
                                           std::forward<T>(_val));
  }

  /// Adds the elements of a tuple to a newly created named tuple,
  /// and other elements to a newly created named tuple.
  template <class... TupContent>
  auto replace_tuple(std::tuple<TupContent...>&& _tuple) {
    const auto r = [this](auto&&... _fields) {
      return this->replace(std::forward<TupContent>(_fields)...);
    };
    return std::apply(r, std::forward<std::tuple<TupContent...>>(_tuple));
  }

  /// Adds the elements of a tuple to a newly created named tuple,
  /// and other elements to a newly created named tuple.
  template <class... TupContent>
  auto replace_tuple(std::tuple<TupContent...>&& _tuple) const {
    const auto r = [this](auto&&... _fields) {
      return this->replace(std::forward<TupContent>(_fields)...);
    };
    return std::apply(r, std::forward<std::tuple<TupContent...>>(_tuple));
  }

  /// Retrieves the fields from another tuple.
  template <class... OtherFieldTypes, class... Args>
  constexpr static Fields retrieve_fields(
      std::tuple<OtherFieldTypes...>&& _other_fields, Args&&... _args) {
    constexpr auto size = sizeof...(Args);

    constexpr bool retrieved_all_fields = size == std::tuple_size_v<Fields>;

    if constexpr (retrieved_all_fields) {
      return std::make_tuple(std::forward<Args>(_args)...);
    } else {
      constexpr auto field_name = std::tuple_element<size, Fields>::type::name_;

      constexpr auto index =
          internal::find_index<field_name, std::tuple<OtherFieldTypes...>>();

      using FieldType = typename std::tuple_element<size, Fields>::type;

      using T = std::remove_cvref_t<typename FieldType::Type>;

      return retrieve_fields(
          std::forward<std::tuple<OtherFieldTypes...>>(_other_fields),
          std::forward<Args>(_args)...,
          FieldType(std::forward<T>(std::get<index>(_other_fields).value_)));
    }
  }

 private:
  /// The values actually contained in the named tuple.
  /// As you can see, a NamedTuple is just a normal tuple under-the-hood,
  /// everything else is resolved at compile time. It should have no
  /// runtime overhead over a normal std::tuple.
  Values values_;
};

// ----------------------------------------------------------------------------

/// We need a special template instantiation for empty named tuples.
template <>
class NamedTuple<> {
 public:
  using Fields = std::tuple<>;
  using Names = Literal<>;
  using Values = std::tuple<>;

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

  /// Template specialization for std::tuple, so we can pass fields from other
  /// named tuples.
  template <class... TupContent, class... Tail>
  auto add(std::tuple<TupContent...> _tuple, const Tail&... _tail) const {
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
  auto and_then(const F& _f) const {
    return NamedTuple<>();
  }

  /// Does nothing at all.
  template <typename F>
  void apply(F&& _f) const {}

  /// Returns an empty tuple.
  auto fields() const { return std::tuple(); }

  /// Must always be 0.
  static constexpr size_t size() { return 0; }

  /// Returns an empty named tuple.
  template <typename F>
  auto transform(const F& _f) const {
    return NamedTuple<>();
  }

  /// Returns an empty tuple.
  auto values() const { return std::tuple(); }
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
