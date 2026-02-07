#ifndef RFL_CLI_READER_HPP_
#define RFL_CLI_READER_HPP_

#include <concepts>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl::cli {

static constexpr char path_separator = '.';
static constexpr char array_delimiter = ',';

struct CliVarType {
  const std::map<std::string, std::string>* const args = nullptr;
  const std::string path;
  const std::optional<std::string> direct_value;
};

struct CliObjectType {
  const std::map<std::string, std::string>* const args = nullptr;
  const std::string prefix;
};

struct CliArrayType {
  const std::vector<std::string> values;
};

// --- Constrained overloads for string-to-type parsing ---

template <class T> requires std::same_as<T, std::string>
rfl::Result<T> parse_value(
    const std::string& _str, const std::string&
) noexcept {
  return _str;
}

template <class T> requires std::same_as<T, bool>
rfl::Result<T> parse_value(
    const std::string& _str, const std::string& _path
) noexcept {
  if (_str.empty() || _str == "true" || _str == "1") {
    return true;
  }
  if (_str == "false" || _str == "0") {
    return false;
  }
  return error(
      "Could not cast '" + _str + "' to boolean for key '" + _path + "'.");
}

template <class T> requires (std::is_floating_point_v<T>)
rfl::Result<T> parse_value(
    const std::string& _str, const std::string& _path
) noexcept {
  try {
    return static_cast<T>(std::stod(_str));
  }
  catch (...) {
    return error(
        "Could not cast '" + _str + "' to floating point for key '" + _path + "'.");
  }
}

template <class T> requires (std::is_unsigned_v<T> && !std::same_as<T, bool>)
rfl::Result<T> parse_value(
    const std::string& _str, const std::string& _path
) noexcept {
  try {
    return static_cast<T>(std::stoull(_str));
  }
  catch (...) {
    return error(
        "Could not cast '" + _str + "' to unsigned integer for key '" + _path + "'.");
  }
}

template <class T> requires (std::is_integral_v<T> && std::is_signed_v<T>)
rfl::Result<T> parse_value(
    const std::string& _str, const std::string& _path
) noexcept {
  try {
    return static_cast<T>(std::stoll(_str));
  }
  catch (...) {
    return error(
        "Could not cast '" + _str + "' to integer for key '" + _path + "'.");
  }
}

struct Reader {
  using InputArrayType = CliArrayType;
  using InputObjectType = CliObjectType;
  using InputVarType = CliVarType;

  template <class T>
  static constexpr bool has_custom_constructor = false;

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType& _arr) const noexcept {
    if (_idx >= _arr.values.size()) {
      return error("Index " + std::to_string(_idx) + " out of bounds.");
    }
    return InputVarType{nullptr, "", _arr.values[_idx]};
  }

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType& _obj) const noexcept {
    const auto child_path = _obj.prefix.empty()
        ? _name
        : _obj.prefix + _name;
    return InputVarType{_obj.args, child_path, std::nullopt};
  }

  bool is_empty(const InputVarType& _var) const noexcept {
    if (_var.direct_value) {
      return false;
    }
    if (!_var.args) {
      return true;
    }
    if (_var.args->count(_var.path)) {
      return false;
    }
    const auto prefix = _var.path + path_separator;
    const auto it = _var.args->lower_bound(prefix);
    return it == _var.args->end()
           || it->first.substr(0, prefix.size()) != prefix;
  }

  template <class ArrayReader>
  std::optional<Error> read_array(
      const ArrayReader& _array_reader,
      const InputArrayType& _arr
  ) const noexcept {
    for (const auto& val : _arr.values) {
      const auto err = _array_reader.read(
          InputVarType{nullptr, "", val});
      if (err) {
        return err;
      }
    }
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(
      const ObjectReader& _object_reader,
      const InputObjectType& _obj
  ) const noexcept {
    std::set<std::string> seen;
    auto it = _obj.prefix.empty()
        ? _obj.args->begin()
        : _obj.args->lower_bound(_obj.prefix);
    while (it != _obj.args->end()) {
      if (!_obj.prefix.empty()
          && it->first.substr(0, _obj.prefix.size()) != _obj.prefix) {
        break;
      }
      const auto rest = std::string_view(it->first).substr(_obj.prefix.size());
      const auto separator_pos = rest.find(path_separator);
      const auto child = std::string(
          separator_pos == std::string_view::npos
              ? rest
              : rest.substr(0, separator_pos));
      if (!child.empty() && seen.insert(child).second) {
        const auto child_path = _obj.prefix + child;
        _object_reader.read(
            std::string_view(child),
            InputVarType{_obj.args, child_path, std::nullopt});
      }
      ++it;
    }
    return std::nullopt;
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    const auto str = get_value(_var);
    if (!str) {
      return error("No value for key '" + _var.path + "'.");
    }
    return parse_value<T>(*str, _var.path);
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    const auto str = get_value(_var);
    if (!str) {
      return InputArrayType{{}};
    }
    return InputArrayType{split(*str, array_delimiter)};
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    const auto prefix = _var.path.empty()
        ? std::string("")
        : _var.path + path_separator;
    return InputObjectType{_var.args, prefix};
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType&
  ) const noexcept {
    return error("Custom constructors are not supported for CLI parsing.");
  }

 private:
  static std::optional<std::string> get_value(
      const InputVarType& _var
  ) noexcept {
    if (_var.direct_value) {
      return *_var.direct_value;
    }
    if (!_var.args) {
      return std::nullopt;
    }
    const auto it = _var.args->find(_var.path);
    if (it == _var.args->end()) {
      return std::nullopt;
    }
    return it->second;
  }

  static std::vector<std::string> split(
      const std::string& _str, char _delim
  ) {
    std::vector<std::string> result;
    if (_str.empty()) {
      return result;
    }
    size_t start = 0;
    while (true) {
      const auto pos = _str.find(_delim, start);
      if (pos == std::string::npos) {
        result.emplace_back(_str.substr(start));
        break;
      }
      result.emplace_back(_str.substr(start, pos - start));
      start = pos + 1;
    }
    return result;
  }

};

}  // namespace rfl::cli

#endif
