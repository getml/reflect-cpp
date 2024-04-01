#ifndef RFL_LITERAL_HPP_
#define RFL_LITERAL_HPP_

#include <cstdint>
#include <functional>
#include <limits>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>

#include "Result.hpp"
#include "internal/StringLiteral.hpp"
#include "internal/VisitTree.hpp"

namespace rfl {

template <internal::StringLiteral _field>
struct LiteralHelper {
  constexpr static internal::StringLiteral field_ = _field;
};

template <internal::StringLiteral... fields_>
class Literal {
  using FieldsType = std::tuple<LiteralHelper<fields_>...>;

 public:
  using ValueType =
      std::conditional_t<sizeof...(fields_) <=
                             std::numeric_limits<std::uint8_t>::max(),
                         std::uint8_t, std::uint16_t>;

  /// The number of different fields or different options that the literal
  /// can assume.
  static constexpr ValueType num_fields_ = sizeof...(fields_);

  using ReflectionType = std::string;

  /// Constructs a Literal from another literal.
  Literal(const Literal<fields_...>& _other) = default;

  /// Constructs a Literal from another literal.
  Literal(Literal<fields_...>&& _other) noexcept = default;

  Literal(const std::string& _str) : value_(find_value(_str).value()) {}

  /// A single-field literal is special because it
  /// can also have a default constructor.
  template <ValueType num_fields = num_fields_,
            typename = std::enable_if_t<num_fields <= 1>>
  Literal() : value_(0) {}

  ~Literal() = default;

  /// Constructs a new Literal.
  template <internal::StringLiteral _name>
  static Literal<fields_...> make() {
    return Literal(Literal<fields_...>::template value_of<_name>());
  }

  /// Constructs a new Literal, equivalent to make, for reasons of consistency.
  template <internal::StringLiteral _name>
  static Literal<fields_...> from_name() {
    return Literal<fields_...>::template make<_name>();
  }

  /// Constructs a new Literal.
  template <ValueType _value>
  static Literal<fields_...> from_value() {
    static_assert(_value < num_fields_,
                  "Value cannot exceed number of fields.");
    return Literal<fields_...>(_value);
  }

  /// Constructs a new Literal.
  static Result<Literal<fields_...>> from_value(ValueType _value) {
    if (_value >= num_fields_) {
      return Error("Value cannot exceed number of fields.");
    }
    return Literal<fields_...>(_value);
  }

  /// Determines whether the literal contains the string.
  static bool contains(const std::string& _str) {
    bool found = false;
    has_value(_str, &found);
    return found;
  }

  /// Determines whether the literal contains the string at compile time.
  template <internal::StringLiteral _name>
  static constexpr bool contains() {
    return find_value_of<_name>() != -1;
  }

  /// Determines whether the literal contains any of the strings in the other
  /// literal at compile time.
  template <class OtherLiteralType, int _i = 0>
  static constexpr bool contains_any() {
    if constexpr (_i == num_fields_) {
      return false;
    } else {
      constexpr auto name = find_name_within_own_fields<_i>();
      return OtherLiteralType::template contains<name>() ||
             contains_any<OtherLiteralType, _i + 1>();
    }
  }

  /// Determines whether the literal contains all of the strings in the other
  /// literal at compile time.
  template <class OtherLiteralType, int _i = 0, int _n_found = 0>
  static constexpr bool contains_all() {
    if constexpr (_i == num_fields_) {
      return _n_found == OtherLiteralType::num_fields_;
    } else {
      constexpr auto name = find_name_within_own_fields<_i>();
      if constexpr (OtherLiteralType::template contains<name>()) {
        return contains_all<OtherLiteralType, _i + 1, _n_found + 1>();
      } else {
        return contains_all<OtherLiteralType, _i + 1, _n_found>();
      }
    }
  }

  /// Determines whether the literal has duplicate strings at compile time.
  /// These is useful for checking collections of strings in other contexts.
  static constexpr bool has_duplicates() { return has_duplicate_strings(); }

  /// Constructs a Literal from a string. Returns an error if the string
  /// cannot be found.
  static Result<Literal> from_string(const std::string& _str) {
    const auto to_literal = [](const auto& _v) {
      return Literal<fields_...>(_v);
    };
    return find_value(_str).transform(to_literal);
  };

  /// The name defined by the Literal.
  std::string name() const { return find_name(); }

  /// Returns all possible values of the literal as a std::vector<std::string>.
  static std::vector<std::string> names() { return allowed_strings_vec(); }

  /// Helper function to retrieve a name at compile time.
  template <int _value>
  constexpr static auto name_of() {
    constexpr auto name = find_name_within_own_fields<_value>();
    return Literal<name>();
  }

  /// Assigns from another literal.
  Literal<fields_...>& operator=(const Literal<fields_...>& _other) = default;

  /// Assigns from another literal.
  Literal<fields_...>& operator=(Literal<fields_...>&& _other) noexcept =
      default;

  /// Assigns the literal from a string
  Literal<fields_...>& operator=(const std::string& _str) {
    value_ = find_value(_str);
    return *this;
  }

  /// <=> for other Literals with the same fields.
  auto operator<=>(const Literal<fields_...>& _other) const {
    return value() <=> _other.value();
  }

  /// <=> for other Literals with different fields.
  template <internal::StringLiteral... _fields>
  inline auto operator<=>(const Literal<_fields...>& _l2) const {
    return name() <=> _l2.name();
  }

  /// <=> for strings.
  inline auto operator<=>(const std::string& _str) const {
    return name() <=> _str;
  }

  /// <=> for const char*.
  template <internal::StringLiteral... other_fields>
  inline auto operator<=>(const char* _str) const {
    return name() <=> _str;
  }

  /// Equality operator.
  template <class Other>
  bool operator==(const Other& _other) const {
    return (*this <=> _other) == 0;
  }

  /// Alias for .name().
  std::string reflection() const { return name(); }

  /// Returns the number of fields in the Literal.
  static constexpr size_t size() { return num_fields_; }

  /// Alias for .name().
  std::string str() const { return name(); }

  /// Alias for .names().
  static std::vector<std::string> strings() { return allowed_strings_vec(); }

  /// Returns the value actually contained in the Literal.
  ValueType value() const { return value_; }

  /// Returns the value of the string literal in the template.
  template <internal::StringLiteral _name>
  static constexpr ValueType value_of() {
    constexpr auto value = find_value_of<_name>();
    static_assert(value >= 0, "String not supported.");
    return value;
  }

 private:
  /// Only the static methods are allowed to access this.
  Literal(const ValueType _value) : value_(_value) {}

  /// Returns all of the allowed fields.
  static std::string allowed_strings() {
    const auto vec = allowed_strings_vec();
    std::string str;
    for (size_t i = 0; i < vec.size(); ++i) {
      const auto head = "'" + vec[i] + "'";
      str += i == 0 ? head : (", " + head);
    }
    return str;
  }

  /// Returns all of the allowed fields.
  template <int _i = 0>
  static std::vector<std::string> allowed_strings_vec(
      std::vector<std::string> _values = {}) {
    using FieldType = typename std::tuple_element<_i, FieldsType>::type;
    const auto head = FieldType::field_.str();
    _values.push_back(head);
    if constexpr (_i + 1 < num_fields_) {
      return allowed_strings_vec<_i + 1>(std::move(_values));
    } else {
      return _values;
    }
  }

  /// Whether the Literal contains duplicate strings.
  template <int _i = 1>
  constexpr static bool has_duplicate_strings() {
    if constexpr (_i >= num_fields_) {
      return false;
    } else {
      return is_duplicate<_i>() || has_duplicate_strings<_i + 1>();
    }
  }

  template <int _i, int _j = _i - 1>
  constexpr static bool is_duplicate() {
    using FieldType1 = typename std::tuple_element<_i, FieldsType>::type;
    using FieldType2 = typename std::tuple_element<_j, FieldsType>::type;
    if constexpr (FieldType1::field_ == FieldType2::field_) {
      return true;
    } else if constexpr (_j > 0) {
      return is_duplicate<_i, _j - 1>();
    } else {
      return false;
    }
  }

  /// Finds the correct index associated with
  /// the string at run time.
  template <int _i = 0>
  std::string find_name() const {
    if (_i == value_) {
      using FieldType = typename std::tuple_element<_i, FieldsType>::type;
      return FieldType::field_.str();
    }
    if constexpr (_i + 1 == num_fields_) {
      return "";
    } else {
      return find_name<_i + 1>();
    }
  }

  /// Finds the correct index associated with
  /// the string at compile time within the Literal's own fields.
  template <int _i>
  constexpr static auto find_name_within_own_fields() {
    using FieldType = typename std::tuple_element<_i, FieldsType>::type;
    return FieldType::field_;
  }

  /// Finds the correct value associated with
  /// the string at run time.
  template <int _i = 0>
  static Result<int> find_value(const std::string& _str) {
    using FieldType = typename std::tuple_element<_i, FieldsType>::type;
    if (FieldType::field_.str() == _str) {
      return _i;
    }
    if constexpr (_i + 1 == num_fields_) {
      return Error(
          "Literal does not support string '" + _str +
          "'. The following strings are supported: " + allowed_strings() + ".");
    } else {
      return find_value<_i + 1>(_str);
    }
  }

  /// Finds the value of a string literal at compile time.
  template <internal::StringLiteral _name, int _i = 0>
  static constexpr int find_value_of() {
    if constexpr (_i == num_fields_) {
      return -1;
    } else {
      using FieldType = typename std::tuple_element<_i, FieldsType>::type;
      if constexpr (FieldType::field_ == _name) {
        return _i;
      } else {
        return find_value_of<_name, _i + 1>();
      }
    }
  }

  /// Whether the literal contains this string.
  template <int _i = 0>
  static void has_value(const std::string& _str, bool* _found) {
    if constexpr (_i == num_fields_) {
      *_found = false;
      return;
    } else {
      using FieldType = typename std::tuple_element<_i, FieldsType>::type;
      if (FieldType::field_.str() == _str) {
        *_found = true;
        return;
      }
      return has_value<_i + 1>(_str, _found);
    }
  }

  static_assert(sizeof...(fields_) <= std::numeric_limits<ValueType>::max(),
                "Too many fields.");

  static_assert(sizeof...(fields_) <= 1 || !has_duplicates(),
                "Duplicate strings are not allowed in a Literal.");

 private:
  /// The underlying value.
  ValueType value_;
};

/// Helper function to retrieve a name at compile time.
template <class LiteralType, int _value>
inline constexpr auto name_of() {
  return LiteralType::template name_of<_value>();
}

/// Helper function to retrieve a value at compile time.
template <class LiteralType, internal::StringLiteral _name>
inline constexpr auto value_of() {
  return LiteralType::template value_of<_name>();
}

}  // namespace rfl

namespace std {

template <rfl::internal::StringLiteral... fields>
struct hash<rfl::Literal<fields...>> {
  size_t operator()(const rfl::Literal<fields...>& _l) const {
    return hash<int>()(static_cast<int>(_l.value()));
  }
};

}  // namespace std

#endif  // RFL_LITERAL_HPP_
