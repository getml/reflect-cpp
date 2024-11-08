#ifndef RFL_UBJSON_READER_HPP_
#define RFL_UBJSON_READER_HPP_

#include <cstddef>
#include <exception>
#include <jsoncons_ext/ubjson/ubjson_cursor.hpp>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl::ubjson {

struct Reader {
  using CursorType = jsoncons::ubjson::ubjson_bytes_cursor;

  struct UBJSONInputArray {
    CursorType* cursor_;
  };

  struct UBJSONInputObject {
    CursorType* cursor_;
  };

  struct UBJSONInputVar {
    CursorType* cursor_;
  };

  using InputArrayType = UBJSONInputArray;
  using InputObjectType = UBJSONInputObject;
  using InputVarType = UBJSONInputVar;

  template <class T>
  static constexpr bool has_custom_constructor = (requires(InputVarType var) {
    T::from_ubjson_obj(var);
  });

  rfl::Result<InputVarType> get_field_from_array(
      const size_t _idx, const InputArrayType& _arr) const noexcept;

  rfl::Result<InputVarType> get_field_from_object(
      const std::string& _name, const InputObjectType& _obj) const noexcept;

  bool is_empty(const InputVarType& _var) const noexcept;

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    const auto& event = _var.cursor_->current();
    const auto event_type = event.event_type();
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      if (event_type != jsoncons::staj_event_type::string_value) {
        return Error("Could not cast to string.");
      }
      return std::string(event.get<std::string_view>());

      /*    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                            rfl::Bytestring>()) {
            if (!ubjson_value_is_byte_string(&_var.val_)) {
              return Error("Could not cast to bytestring.");
            }
            rfl::Bytestring bstr;
            const auto err = get_bytestring(&_var.val_, &bstr);
            if (err != CborNoError) {
              return Error(ubjson_error_string(err));
            }
            return bstr;*/
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (event_type != jsoncons::staj_event_type::bool_value) {
        return rfl::Error("Could not cast to boolean.");
      }
      return std::string(event.get<bool>());
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_integral<std::remove_cvref_t<T>>()) {
      switch (event_type) {
        case jsoncons::staj_event_type::int64_value:
          return static_cast<T>(event.get<int64_t>());
        case jsoncons::staj_event_type::uint64_value:
          return static_cast<T>(event.get<uint64_t>());
        case jsoncons::staj_event_type::double_value:
          return static_cast<T>(event.get<double>());
        default:
          return rfl::Error(
              "Could not cast to numeric value. The type must be integral, "
              "float or double.");
      }
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(const InputVarType& _var) const noexcept;

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept;

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    for (const auto& event = _arr.cursor_->current();
         event.event_type() != jsoncons::staj_event_type::end_array;
         _arr.cursor_->next()) {
      const auto err = _array_reader.read(InputVarType{_arr.cursor_});
      if (err) {
        // TODO: Go to end of cursor
        return err;
      }
    }
    _arr.cursor_->next();
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    for (const auto& event = _obj.cursor_->current();
         event.event_type() != jsoncons::staj_event_type::end_object;
         _obj.cursor_->next()) {
      if (event.event_type() != jsoncons::staj_event_type::key) {
        _obj.cursor_->reset();
        _obj.cursor_->reset();
        return Error("Expected a key.");
      }
      const auto key = event.get<std::string_view>();
      _object_reader.read(key, InputVarType{_obj.cursor_});
    }
    _obj.cursor_->next();
    return std::nullopt;
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_ubjson_obj(_var);
    } catch (std::exception& e) {
      return rfl::Error(e.what());
    }
  }
};

}  // namespace rfl::ubjson

#endif
