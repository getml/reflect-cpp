#ifndef RFL_CLI_RESOLVE_ARGS_HPP_
#define RFL_CLI_RESOLVE_ARGS_HPP_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "../Result.hpp"
#include "../internal/is_positional.hpp"
#include "../internal/is_short.hpp"
#include "../internal/processed_t.hpp"

namespace rfl::cli {

struct ParsedArgs {
  std::map<std::string, std::string> named;
  std::map<std::string, std::string> short_args;
  std::vector<std::string> positional;
  /// Insertion order of short arg keys (for stable reclaim ordering).
  std::vector<std::string> short_order;
};

namespace detail {

template <class T, bool _is_positional, bool _is_short>
consteval bool check_no_nested_wrappers() {
  if constexpr (_is_positional) {
    return !rfl::internal::is_short_v<typename T::Type>;
  }
  else if constexpr (_is_short) {
    return !rfl::internal::is_positional_v<typename T::Type>;
  }
  else {
    return true;
  }
}

template <class NamedTupleType, size_t _i>
struct field_info {
  using FieldType = rfl::tuple_element_t<_i, typename NamedTupleType::Fields>;
  using InnerType = typename FieldType::Type;

  static constexpr bool is_positional =
      rfl::internal::is_positional_v<InnerType>;
  static constexpr bool is_short = rfl::internal::is_short_v<InnerType>;

  static_assert(
      check_no_nested_wrappers<InnerType, is_positional, is_short>(),
      "Nested wrappers (Positional<Short<...>> or Short<..., Positional<...>>) "
      "are not allowed.");

  static constexpr std::string_view name() { return FieldType::name(); }
};

template <class NamedTupleType, size_t _i>
constexpr auto get_short_name() {
  using Info = field_info<NamedTupleType, _i>;
  if constexpr (Info::is_short) {
    return Info::InnerType::short_name_;
  }
  else {
    return rfl::internal::StringLiteral<1>("");
  }
}

template <class NamedTupleType, size_t _i, size_t... _is>
void collect_positional_names(
    std::vector<std::string>& _names,
    std::index_sequence<_i, _is...>
) {
  using Info = field_info<NamedTupleType, _i>;
  if constexpr (Info::is_positional) {
    _names.emplace_back(Info::name());
  }
  if constexpr (sizeof...(_is) > 0) {
    collect_positional_names<NamedTupleType>(_names, std::index_sequence<_is...>{});
  }
}

template <class NamedTupleType>
void collect_positional_names(
    std::vector<std::string>&,
    std::index_sequence<>
) {}

template <class NamedTupleType, size_t _i, size_t... _is>
rfl::Result<bool> collect_short_mapping(
    std::map<std::string, std::string>& _mapping,
    std::index_sequence<_i, _is...>
) {
  using Info = field_info<NamedTupleType, _i>;
  if constexpr (Info::is_short) {
    constexpr auto short_lit = get_short_name<NamedTupleType, _i>();
    const auto short_str = std::string(short_lit.string_view());
    const auto long_str = std::string(Info::name());
    if (!_mapping.emplace(short_str, long_str).second) {
      return rfl::error(
          "Duplicate short name '-" + short_str + "' in struct definition.");
    }
  }
  if constexpr (sizeof...(_is) > 0) {
    return collect_short_mapping<NamedTupleType>(
        _mapping, std::index_sequence<_is...>{});
  }
  return true;
}

template <class NamedTupleType>
rfl::Result<bool> collect_short_mapping(
    std::map<std::string, std::string>&,
    std::index_sequence<>
) {
  return true;
}

template <class NamedTupleType, size_t _i>
consteval bool is_short_bool() {
  using Info = field_info<NamedTupleType, _i>;
  if constexpr (Info::is_short) {
    return std::is_same_v<typename Info::InnerType::Type, bool>;
  }
  else {
    return false;
  }
}

template <class NamedTupleType, size_t _i, size_t... _is>
void collect_short_bool_names(
    std::set<std::string>& _names,
    std::index_sequence<_i, _is...>
) {
  if constexpr (is_short_bool<NamedTupleType, _i>()) {
    constexpr auto short_lit = get_short_name<NamedTupleType, _i>();
    _names.emplace(short_lit.string_view());
  }
  if constexpr (sizeof...(_is) > 0) {
    collect_short_bool_names<NamedTupleType>(
        _names, std::index_sequence<_is...>{});
  }
}

template <class NamedTupleType>
void collect_short_bool_names(
    std::set<std::string>&,
    std::index_sequence<>
) {}

}  // namespace detail

/// Resolves ParsedArgs into a flat key-value map using compile-time
/// metadata from the target struct T.
template <class T, class ProcessorsType>
rfl::Result<std::map<std::string, std::string>> resolve_args(
    ParsedArgs _parsed
) {
  using NT = rfl::internal::processed_t<T, ProcessorsType>;
  constexpr auto sz = NT::size();
  using Indices = std::make_index_sequence<sz>;

  // Collect positional field names (in declaration order).
  std::vector<std::string> positional_names;
  detail::collect_positional_names<NT>(positional_names, Indices{});

  // Collect short-to-long mapping.
  std::map<std::string, std::string> short_to_long;
  const auto short_result =
      detail::collect_short_mapping<NT>(short_to_long, Indices{});
  if (!short_result) {
    return rfl::error(short_result.error().what());
  }

  // Reclaim non-boolean values from short bool flags.
  // parse_argv doesn't know types, so `-v somefile` consumes "somefile" as
  // the value of -v.  Since -v is bool, "somefile" is not a valid bool value
  // and must be returned to the positional list.
  // We iterate in insertion order (short_order) to preserve argv ordering.
  std::set<std::string> short_bool_names;
  detail::collect_short_bool_names<NT>(short_bool_names, Indices{});
  std::vector<std::string> reclaimed;
  for (const auto& short_name : _parsed.short_order) {
    if (!short_bool_names.count(short_name)) {
      continue;
    }
    auto it = _parsed.short_args.find(short_name);
    if (it == _parsed.short_args.end()) {
      continue;
    }
    auto& value = it->second;
    if (value != "true" && value != "false"
        && value != "0" && value != "1"
        && !value.empty()) {
      reclaimed.push_back(value);
      value.clear();
    }
  }
  // Insert reclaimed values before existing positional args to preserve
  // the original argv order: `-v file1 file2` → positional ["file1", "file2"].
  if (!reclaimed.empty()) {
    _parsed.positional.insert(
        _parsed.positional.begin(),
        std::make_move_iterator(reclaimed.begin()),
        std::make_move_iterator(reclaimed.end()));
  }

  auto& result = _parsed.named;

  // Merge positional args.
  if (_parsed.positional.size() > positional_names.size()) {
    auto msg = std::string("Too many positional arguments: expected at most ");
    msg += std::to_string(positional_names.size());
    msg += ", got ";
    msg += std::to_string(_parsed.positional.size());
    msg += ".";
    return rfl::error(std::move(msg));
  }
  for (size_t i = 0; i < _parsed.positional.size(); ++i) {
    const auto& long_name = positional_names[i];
    if (result.count(long_name)) {
      return rfl::error(
          "Conflict: positional argument and '--" + long_name
          + "' both provided.");
    }
    result.emplace(long_name, std::move(_parsed.positional[i]));
  }

  // Merge short args.
  for (auto& [short_name, value] : _parsed.short_args) {
    const auto it = short_to_long.find(short_name);
    if (it == short_to_long.end()) {
      return rfl::error("Unknown short argument: -" + short_name);
    }
    const auto& long_name = it->second;
    if (result.count(long_name)) {
      return rfl::error(
          "Conflict: '-" + short_name + "' and '--" + long_name
          + "' both provided.");
    }
    result.emplace(long_name, std::move(value));
  }

  return std::move(result);
}

}  // namespace rfl::cli

#endif
