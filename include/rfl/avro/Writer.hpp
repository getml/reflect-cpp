#ifndef RFL_AVRO_WRITER_HPP_
#define RFL_AVRO_WRITER_HPP_

#include <avro.h>

#include <climits>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include "../Bytestring.hpp"
#include "../Timestamp.hpp"
#include "../Vectorstring.hpp"
#include "../always_false.hpp"
#include "../common.hpp"
#include "../concepts.hpp"
#include "../internal/is_literal.hpp"
#include "../internal/is_validator.hpp"
#include "../patterns.hpp"

namespace rfl::avro {

class RFL_API Writer {
 public:
  struct AVROOutputArray {
    avro_value_t val_;
  };

  struct AVROOutputMap {
    avro_value_t val_;
  };

  struct AVROOutputObject {
    avro_value_t val_;
  };

  struct AVROOutputUnion {
    avro_value_t val_;
  };

  struct AVROOutputVar {
    avro_value_t val_;
  };

  using OutputArrayType = AVROOutputArray;
  using OutputMapType = AVROOutputMap;
  using OutputObjectType = AVROOutputObject;
  using OutputUnionType = AVROOutputUnion;
  using OutputVarType = AVROOutputVar;

  Writer(avro_value_t* _root);

  ~Writer();

  OutputArrayType array_as_root(const size_t _size) const;

  OutputMapType map_as_root(const size_t _size) const;

  OutputObjectType object_as_root(const size_t _size) const;

  OutputVarType null_as_root() const;

  OutputUnionType union_as_root() const;

  template <class T>
  OutputVarType value_as_root(const T& _var) const {
    set_value(_var, root_);
    return OutputVarType{*root_};
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const;

  OutputArrayType add_array_to_map(const std::string_view& _name,
                                   const size_t _size,
                                   OutputMapType* _parent) const;

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const;

  OutputArrayType add_array_to_union(const size_t _index, const size_t _size,
                                     OutputUnionType* _parent) const;

  OutputMapType add_map_to_array(const size_t _size,
                                 OutputArrayType* _parent) const;

  OutputMapType add_map_to_map(const std::string_view& _name,
                               const size_t _size,
                               OutputMapType* _parent) const;

  OutputMapType add_map_to_object(const std::string_view& _name,
                                  const size_t _size,
                                  OutputObjectType* _parent) const;

  OutputMapType add_map_to_union(const size_t _index, const size_t _size,
                                 OutputUnionType* _parent) const;

  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const;

  OutputObjectType add_object_to_map(const std::string_view& _name,
                                     const size_t _size,
                                     OutputMapType* _parent) const;

  OutputObjectType add_object_to_object(const std::string_view& _name,
                                        const size_t _size,
                                        OutputObjectType* _parent) const;

  OutputObjectType add_object_to_union(const size_t _index, const size_t _size,
                                       OutputUnionType* _parent) const;

  OutputUnionType add_union_to_array(OutputArrayType* _parent) const;

  OutputUnionType add_union_to_map(const std::string_view& _name,
                                   OutputMapType* _parent) const;

  OutputUnionType add_union_to_object(const std::string_view& _name,
                                      OutputObjectType* _parent) const;

  OutputUnionType add_union_to_union(const size_t _index,
                                     OutputUnionType* _parent) const;

  OutputVarType add_null_to_array(OutputArrayType* _parent) const;

  OutputVarType add_null_to_map(const std::string_view& _name,
                                OutputMapType* _parent) const;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const;

  OutputVarType add_null_to_union(const size_t _index,
                                  OutputUnionType* _parent) const;

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const {
    avro_value_t new_value;
    int result = avro_value_append(&_parent->val_, &new_value, nullptr);
    if (result != 0) {
      throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                               std::to_string(result) +
                               "): " + avro_strerror());
    }
    set_value(_var, &new_value);
    return OutputVarType{new_value};
  }

  template <class T>
  OutputVarType add_value_to_map(const std::string_view& _name, const T& _var,
                                 OutputMapType* _parent) const {
    avro_value_t new_value;
    int result = avro_value_add(&_parent->val_, _name.data(), &new_value,
                                nullptr, nullptr);
    if (result != 0) {
      throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                               std::to_string(result) +
                               "): " + avro_strerror());
    }
    set_value(_var, &new_value);
    return OutputVarType{new_value};
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const {
    avro_value_t new_value;
    int result = avro_value_get_by_name(&_parent->val_, _name.data(),
                                        &new_value, nullptr);
    if (result != 0) {
      throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                               std::to_string(result) +
                               "): " + avro_strerror());
    }
    set_value(_var, &new_value);
    return OutputVarType{new_value};
  }

  template <class T>
  OutputVarType add_value_to_union(const size_t _index, const T& _var,
                                   OutputUnionType* _parent) const {
    if (_index > static_cast<size_t>(INT_MAX)) {
      throw std::runtime_error(std::string(__FUNCTION__) + " index error");
    }
    avro_value_t new_value;
    int result = avro_value_set_branch(&_parent->val_, static_cast<int>(_index),
                                       &new_value);
    if (result != 0) {
      throw std::runtime_error("Error adding value to union: error(" +
                               std::to_string(result) +
                               "): " + avro_strerror());
    }
    set_value(_var, &new_value);
    return OutputVarType{new_value};
  }

  void end_array(OutputArrayType* /*_arr*/) const noexcept {}

  void end_map(OutputMapType* /*_obj*/) const noexcept {}

  void end_object(OutputObjectType* /*_obj*/) const noexcept {}

 private:
  template <class T>
  void set_value(const T& _var, avro_value_t* _val) const {
    using Type = std::remove_cvref_t<T>;

    if constexpr (std::is_same_v<Type, std::string>) {
      int result =
          avro_value_set_string_len(_val, _var.c_str(), _var.size() + 1);
      if (result != 0) {
        throw std::runtime_error(
            "Error setting string value: " + std::to_string(result) + ": " +
            avro_strerror());
      }

    } else if constexpr (concepts::MutableContiguousByteContainer<Type>) {
      auto var = _var;
      if (!var.data()) {
        return;
      }
      int result = avro_value_set_bytes(_val, var.data(), var.size());
      if (result != 0) {
        throw std::runtime_error(
            "Error setting bytestring value: " + std::to_string(result) + ": " +
            avro_strerror());
      }

    } else if constexpr (std::is_same_v<Type, bool>) {
      int result = avro_value_set_boolean(_val, _var);
      if (result != 0) {
        throw std::runtime_error(
            "Error setting boolean value: " + std::to_string(result) + ": " +
            avro_strerror());
      }

    } else if constexpr (std::is_same_v<Type, float>) {
      int result = avro_value_set_float(_val, static_cast<float>(_var));
      if (result != 0) {
        throw std::runtime_error(
            "Error setting float value: " + std::to_string(result) + ": " +
            avro_strerror());
      }

    } else if constexpr (std::is_floating_point_v<Type>) {
      int result = avro_value_set_double(_val, static_cast<double>(_var));
      if (result != 0) {
        throw std::runtime_error(
            "Error setting double value: " + std::to_string(result) + ": " +
            avro_strerror());
      }

    } else if constexpr (std::is_same_v<Type, std::int64_t> ||
                         std::is_same_v<Type, std::uint32_t> ||
                         std::is_same_v<Type, std::uint64_t>) {
      int result = avro_value_set_long(_val, static_cast<std::int64_t>(_var));
      if (result != 0) {
        throw std::runtime_error(
            "Error setting long value: " + std::to_string(result) + ": " +
            avro_strerror());
      }

    } else if constexpr (std::is_integral_v<Type>) {
      int result = avro_value_set_int(_val, static_cast<std::int32_t>(_var));
      if (result != 0) {
        throw std::runtime_error(
            "Error setting int value: " + std::to_string(result) + ": " +
            avro_strerror());
      }

    } else if constexpr (internal::is_literal_v<T>) {
      int result = avro_value_set_enum(_val, static_cast<int>(_var.value()));
      if (result != 0) {
        throw std::runtime_error(
            "Error setting literal value: " + std::to_string(result) + ": " +
            avro_strerror());
      }

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

 private:
  avro_value_t* root_;
};

}  // namespace rfl::avro

#endif
