#ifndef RFL_PARSING_PARSER_HPP_
#define RFL_PARSING_PARSER_HPP_

#include <array>
#include <cstddef>
#include <deque>
#include <exception>
#include <forward_list>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "rfl/Box.hpp"
#include "rfl/Literal.hpp"
#include "rfl/NamedTuple.hpp"
#include "rfl/Ref.hpp"
#include "rfl/Result.hpp"
#include "rfl/TaggedUnion.hpp"
#include "rfl/always_false.hpp"
#include "rfl/field_type.hpp"
#include "rfl/from_named_tuple.hpp"
#include "rfl/internal/StringLiteral.hpp"
#include "rfl/internal/all_fields.hpp"
#include "rfl/internal/has_reflection_method_v.hpp"
#include "rfl/internal/has_reflection_type_v.hpp"
#include "rfl/internal/no_duplicate_field_names.hpp"
#include "rfl/internal/to_ptr_named_tuple.hpp"
#include "rfl/named_tuple_t.hpp"
#include "rfl/parsing/is_forward_list.hpp"
#include "rfl/parsing/is_map_like.hpp"
#include "rfl/parsing/is_required.hpp"
#include "rfl/parsing/is_set_like.hpp"
#include "rfl/to_named_tuple.hpp"

namespace rfl {
namespace parsing {

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType, class T>
struct Parser;

// ----------------------------------------------------------------------------

/// Default case - anything that cannot be explicitly matched.
template <class ReaderType, class WriterType, class T>
struct Parser {
    using InputVarType = typename ReaderType::InputVarType;
    using OutputVarType = typename WriterType::OutputVarType;

    /// Expresses the variables as type T.
    static Result<T> read(const ReaderType& _r, InputVarType* _var) noexcept {
        if constexpr (ReaderType::template has_custom_constructor<T>) {
            return _r.template use_custom_constructor<T>(_var);
        } else {
            if constexpr (internal::has_reflection_type_v<T>) {
                using ReflectionType = std::decay_t<typename T::ReflectionType>;
                const auto wrap_in_t = [](auto _named_tuple) -> Result<T> {
                    try {
                        return T(_named_tuple);
                    } catch (std::exception& e) {
                        return Error(e.what());
                    }
                };
                return Parser<ReaderType, WriterType, ReflectionType>::read(
                           _r, _var)
                    .and_then(wrap_in_t);
            } else if constexpr (std::is_class_v<T> &&
                                 !std::is_same<T, std::string>()) {
                using NamedTupleType = named_tuple_t<T>;
                const auto to_struct = [](NamedTupleType&& _n) -> Result<T> {
                    try {
                        return from_named_tuple<T>(std::move(_n));
                    } catch (std::exception& e) {
                        return Error(e.what());
                    }
                };
                return Parser<ReaderType, WriterType, NamedTupleType>::read(
                           _r, _var)
                    .and_then(to_struct);
            } else {
                return _r.template to_basic_type<std::decay_t<T>>(_var);
            }
        }
    }

    /// Converts the variable to a JSON type.
    static auto write(const WriterType& _w, const T& _var) noexcept {
        if constexpr (internal::has_reflection_type_v<T>) {
            using ReflectionType = std::decay_t<typename T::ReflectionType>;
            if constexpr (internal::has_reflection_method_v<T>) {
                return Parser<ReaderType, WriterType, ReflectionType>::write(
                    _w, _var.reflection());
            } else {
                const auto& [r] = _var;
                return Parser<ReaderType, WriterType, ReflectionType>::write(_w,
                                                                             r);
            }
        } else if constexpr (std::is_class_v<T> &&
                             !std::is_same<T, std::string>()) {
            const auto ptr_named_tuple = internal::to_ptr_named_tuple(_var);
            using PtrNamedTupleType = std::decay_t<decltype(ptr_named_tuple)>;
            return Parser<ReaderType, WriterType, PtrNamedTupleType>::write(
                _w, ptr_named_tuple);
        } else {
            return _w.from_basic_type(_var);
        }
    }
};

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, T*> {
    using InputVarType = typename ReaderType::InputVarType;
    using OutputVarType = typename WriterType::OutputVarType;

    /// Expresses the variables as type T.
    static Result<T*> read(const ReaderType& _r, InputVarType* _var) noexcept {
        static_assert(
            always_false_v<T>,
            "Reading into raw pointers is dangerous and therefore unsupported. "
            "Please consider using std::unique_ptr, rfl::Box, std::shared_ptr, "
            "rfl::Ref or std::optional instead.");
        return Error("Unsupported.");
    }

    /// Expresses the variable a a JSON.
    static OutputVarType write(const WriterType& _w, const T* _ptr) noexcept {
        if (!_ptr) {
            return _w.empty_var();
        }
        return Parser<ReaderType, WriterType, std::decay_t<T>>::write(_w,
                                                                      *_ptr);
    }
};

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType, class T, size_t _size>
struct Parser<ReaderType, WriterType, std::array<T, _size>> {
   public:
    using InputArrayType = typename ReaderType::InputArrayType;
    using InputVarType = typename ReaderType::InputVarType;

    using OutputArrayType = typename WriterType::OutputArrayType;
    using OutputVarType = typename WriterType::OutputVarType;

    static Result<std::array<T, _size>> read(const ReaderType& _r,
                                             InputVarType* _var) noexcept {
        const auto to_vec = [&](auto _arr) { return _r.to_vec(&_arr); };

        const auto check_size =
            [](auto _vec) -> Result<std::vector<InputVarType>> {
            if (_vec.size() != _size) {
                return Error("Expected " + std::to_string(_size) +
                             " fields, got " + std::to_string(_vec.size()) +
                             ".");
            }
            return std::move(_vec);
        };

        const auto extract = [&_r](auto _vec) {
            return extract_field_by_field(_r, std::move(_vec));
        };

        return _r.to_array(_var)
            .transform(to_vec)
            .and_then(check_size)
            .and_then(extract);
    }

    static OutputVarType write(const WriterType& _w,
                               const std::array<T, _size>& _arr) noexcept {
        auto arr = _w.new_array();
        for (auto it = _arr.begin(); it != _arr.end(); ++it) {
            _w.add(
                Parser<ReaderType, WriterType, std::decay_t<T>>::write(_w, *it),
                &arr);
        }
        return OutputVarType(arr);
    }

   private:
    /// Extracts values from the array, field by field.
    template <class... AlreadyExtracted>
    static Result<std::array<T, _size>> extract_field_by_field(
        const ReaderType& _r, std::vector<InputVarType> _vec,
        AlreadyExtracted&&... _already_extracted) noexcept {
        constexpr size_t i = sizeof...(AlreadyExtracted);
        if constexpr (i == _size) {
            return std::array<T, _size>({std::move(_already_extracted)...});
        } else {
            const auto extract_next = [&](auto&& new_entry) {
                return extract_field_by_field(_r, std::move(_vec),
                                              std::move(_already_extracted)...,
                                              std::move(new_entry));
            };
            return extract_single_field<i>(_r, &_vec).and_then(extract_next);
        }
    }

    /// Extracts a single field from a JSON.
    template <int _i>
    static auto extract_single_field(const ReaderType& _r,
                                     std::vector<InputVarType>* _vec) noexcept {
        return Parser<ReaderType, WriterType, T>::read(_r, &((*_vec)[_i]));
    }
};

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, Box<T>> {
    using InputVarType = typename ReaderType::InputVarType;
    using OutputVarType = typename WriterType::OutputVarType;

    /// Expresses the variables as type T.
    static Result<Box<T>> read(const ReaderType& _r,
                               InputVarType* _var) noexcept {
        const auto to_box = [](auto&& _t) {
            return Box<T>::make(std::move(_t));
        };
        return Parser<ReaderType, WriterType, std::decay_t<T>>::read(_r, _var)
            .transform(to_box);
    }

    /// Expresses the variable a a JSON.
    static OutputVarType write(const WriterType& _w,
                               const Box<T>& _box) noexcept {
        return Parser<ReaderType, WriterType, std::decay_t<T>>::write(_w,
                                                                      *_box);
    }
};

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType,
          internal::StringLiteral... _fields>
struct Parser<ReaderType, WriterType, Literal<_fields...>> {
    using InputVarType = typename ReaderType::InputVarType;
    using OutputVarType = typename WriterType::OutputVarType;

    /// Expresses the variables as type T.
    static Result<Literal<_fields...>> read(const ReaderType& _r,
                                            InputVarType* _var) noexcept {
        const auto to_type = [](const auto& _str) {
            return Literal<_fields...>::from_string(_str);
        };

        const auto embellish_error = [](const Error& _e) {
            return Error(std::string("Failed to parse Literal: ") + _e.what());
        };

        return _r.template to_basic_type<std::string>(_var)
            .and_then(to_type)
            .or_else(embellish_error);
    }

    /// Expresses the variable a a JSON.
    static OutputVarType write(const WriterType& _w,
                               const Literal<_fields...>& _literal) noexcept {
        return _w.from_basic_type(_literal.name());
    }
};

// ----------------------------------------------------------------------------

/// Used for maps for which the key type is std::string. These are represented
/// as objects.
template <class ReaderType, class WriterType, class MapType>
struct MapParser {
   public:
    using InputObjectType = typename ReaderType::InputObjectType;
    using InputVarType = typename ReaderType::InputVarType;

    using OutputObjectType = typename WriterType::OutputObjectType;
    using OutputVarType = typename WriterType::OutputVarType;

    using ValueType = std::decay_t<typename MapType::value_type::second_type>;

    static Result<MapType> read(const ReaderType& _r,
                                InputVarType* _var) noexcept {
        const auto get_pair = [&](auto _pair) {
            const auto to_pair = [&](ValueType&& _val) {
                return std::make_pair(_pair.first, std::move(_val));
            };
            return Parser<ReaderType, WriterType,
                          std::decay_t<ValueType>>::read(_r, &_pair.second)
                .transform(to_pair)
                .value();
        };

        const auto to_map = [&_r, get_pair](auto _obj) -> Result<MapType> {
            auto m = _r.to_map(&_obj);
            MapType res;
            try {
                for (const auto& p : m) {
                    res.insert(get_pair(p));
                }
            } catch (std::exception& e) {
                return Error(e.what());
            }
            return res;
        };

        return _r.to_object(_var).and_then(to_map);
    }

    static OutputVarType write(const WriterType& _w,
                               const MapType& _m) noexcept {
        auto obj = _w.new_object();
        for (const auto& [k, v] : _m) {
            _w.set_field(
                k,
                Parser<ReaderType, WriterType, std::decay_t<ValueType>>::write(
                    _w, v),
                &obj);
        }
        return obj;
    }
};

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, std::map<std::string, T>>
    : public MapParser<ReaderType, WriterType, std::map<std::string, T>> {};

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, std::unordered_map<std::string, T>>
    : public MapParser<ReaderType, WriterType,
                       std::unordered_map<std::string, T>> {};

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType, class... FieldTypes>
struct Parser<ReaderType, WriterType, NamedTuple<FieldTypes...>> {
    using InputObjectType = typename ReaderType::InputObjectType;
    using InputVarType = typename ReaderType::InputVarType;

    using OutputObjectType = typename WriterType::OutputObjectType;
    using OutputVarType = typename WriterType::OutputVarType;

   public:
    /// Generates a NamedTuple from a JSON Object.
    static Result<NamedTuple<FieldTypes...>> read(const ReaderType& _r,
                                                  InputVarType* _var) noexcept {
        const auto to_fields_vec = [&](auto _obj) {
            return _r.to_fields_vec(field_indices(), &_obj);
        };
        const auto build = [&](auto _fields_vec) {
            return build_named_tuple_recursively(_r, _fields_vec);
        };
        return _r.to_object(_var).transform(to_fields_vec).and_then(build);
    }

    /// Transforms a NamedTuple into a JSON object.
    static OutputVarType write(const WriterType& _w,
                               const NamedTuple<FieldTypes...>& _tup) noexcept {
        auto obj = _w.new_object();
        build_object_recursively(_w, _tup, &obj);
        return OutputVarType(obj);
    }

   private:
    /// Builds the named tuple field by field.
    template <class... Args>
    static Result<NamedTuple<FieldTypes...>> build_named_tuple_recursively(
        const ReaderType& _r,
        const std::vector<std::optional<InputVarType>>& _fields_vec,
        Args&&... _args) noexcept {
        const auto size = sizeof...(Args);

        if constexpr (size == sizeof...(FieldTypes)) {
            return NamedTuple<FieldTypes...>(std::move(_args)...);
        } else {
            using FieldType = typename std::tuple_element<
                size, typename NamedTuple<FieldTypes...>::Fields>::type;

            using ValueType = std::decay_t<typename FieldType::Type>;

            const auto& f = _fields_vec[size];

            if (!f) {
                if constexpr (is_required<ValueType>()) {
                    const auto key = FieldType::name_.str();
                    return Error("Field named '" + key + "' not found!");
                } else {
                    return build_named_tuple_recursively(
                        _r, _fields_vec, std::move(_args)...,
                        FieldType(ValueType()));
                }
            }

            const auto build = [&](auto&& _value) {
                return build_named_tuple_recursively(
                    _r, _fields_vec, std::move(_args)...,
                    FieldType(std::move(_value)));
            };

            return get_value<FieldType>(_r, *f).and_then(build);
        }
    }

    /// Builds the object field by field.
    template <int _i = 0>
    static void build_object_recursively(const WriterType& _w,
                                         const NamedTuple<FieldTypes...>& _tup,
                                         OutputObjectType* _ptr) noexcept {
        if constexpr (_i >= sizeof...(FieldTypes)) {
            return;
        } else {
            using FieldType =
                typename std::tuple_element<_i,
                                            std::tuple<FieldTypes...>>::type;
            using ValueType = std::decay_t<typename FieldType::Type>;
            auto value = Parser<ReaderType, WriterType, ValueType>::write(
                _w, rfl::get<_i>(_tup));
            const auto name = FieldType::name_.str();
            if constexpr (!is_required<ValueType>()) {
                if (!_w.is_empty(&value)) {
                    _w.set_field(name, value, _ptr);
                }
            } else {
                _w.set_field(name, value, _ptr);
            }
            return build_object_recursively<_i + 1>(_w, _tup, _ptr);
        }
    }

    /// Uses a memoization pattern to retrieve the field indices.
    /// There are some objects that we are likely to parse many times,
    /// so we only calculate these indices once.
    static const auto& field_indices() noexcept {
        if (field_indices_.size() == 0) {
            set_field_indices();
        }
        return Parser<ReaderType, WriterType,
                      NamedTuple<FieldTypes...>>::field_indices_;
    }

    /// Retrieves the value from the object. This is mainly needed to generate a
    /// better error message.
    template <class FieldType>
    static auto get_value(const ReaderType& _r, InputVarType _var) noexcept {
        const auto embellish_error = [&](const Error& _e) {
            const auto key = FieldType::name_.str();
            return Error("Failed to parse field '" + key + "': " + _e.what());
        };
        using ValueType = std::decay_t<typename FieldType::Type>;
        return Parser<ReaderType, WriterType, ValueType>::read(_r, &_var)
            .or_else(embellish_error);
    }

    /// Builds the object field by field.
    template <size_t _i = 0>
    static void set_field_indices() noexcept {
        if constexpr (_i >= sizeof...(FieldTypes)) {
            return;
        } else {
            using FieldType =
                typename std::tuple_element<_i,
                                            std::tuple<FieldTypes...>>::type;
            const auto name = FieldType::name_.str();
            Parser<ReaderType, WriterType,
                   NamedTuple<FieldTypes...>>::field_indices_[name] = _i;
            set_field_indices<_i + 1>();
        }
    }

   private:
    /// Maps each of the field names to an index signifying their order.
    static inline std::unordered_map<std::string, size_t> field_indices_;
};

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, std::optional<T>> {
    using InputVarType = typename ReaderType::InputVarType;
    using OutputVarType = typename WriterType::OutputVarType;

    /// Expresses the variables as type T.
    static Result<std::optional<T>> read(const ReaderType& _r,
                                         InputVarType* _var) noexcept {
        if (_r.is_empty(_var)) {
            return std::optional<T>();
        }
        const auto to_opt = [](auto&& _t) { return std::make_optional<T>(_t); };
        return Parser<ReaderType, WriterType, std::decay_t<T>>::read(_r, _var)
            .transform(to_opt);
    }

    /// Expresses the variable a a JSON.
    static OutputVarType write(const WriterType& _w,
                               const std::optional<T>& _o) noexcept {
        if (!_o) {
            return _w.empty_var();
        }
        return Parser<ReaderType, WriterType, std::decay_t<T>>::write(_w, *_o);
    }
};

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, Ref<T>> {
    using InputVarType = typename ReaderType::InputVarType;
    using OutputVarType = typename WriterType::OutputVarType;

    /// Expresses the variables as type T.
    static Result<Ref<T>> read(const ReaderType& _r,
                               InputVarType* _var) noexcept {
        const auto to_ref = [&](auto&& _t) { return Ref<T>::make(_t); };
        return Parser<ReaderType, WriterType, std::decay_t<T>>::read(_r, _var)
            .transform(to_ref);
    }

    /// Expresses the variable a a JSON.
    static OutputVarType write(const WriterType& _w,
                               const Ref<T>& _ref) noexcept {
        return Parser<ReaderType, WriterType, std::decay_t<T>>::write(_w,
                                                                      *_ref);
    }
};

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, std::shared_ptr<T>> {
    using InputVarType = typename ReaderType::InputVarType;
    using OutputVarType = typename WriterType::OutputVarType;

    /// Expresses the variables as type T.
    static Result<std::shared_ptr<T>> read(const ReaderType& _r,
                                           InputVarType* _var) noexcept {
        if (_r.is_empty(_var)) {
            return std::shared_ptr<T>();
        }
        const auto to_ptr = [](auto&& _t) { return std::make_shared<T>(_t); };
        return Parser<ReaderType, WriterType, std::decay_t<T>>::read(_r, _var)
            .transform(to_ptr);
    }

    /// Expresses the variable a a JSON.
    static OutputVarType write(const WriterType& _w,
                               const std::shared_ptr<T>& _s) noexcept {
        if (!_s) {
            return _w.empty_var();
        }
        return Parser<ReaderType, WriterType, std::decay_t<T>>::write(_w, *_s);
    }
};

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType, class FirstType, class SecondType>
struct Parser<ReaderType, WriterType, std::pair<FirstType, SecondType>> {
    using InputVarType = typename ReaderType::InputVarType;
    using OutputVarType = typename WriterType::OutputVarType;

    /// Expresses the variables as type T.
    static Result<std::pair<FirstType, SecondType>> read(
        const ReaderType& _r, InputVarType* _var) noexcept {
        const auto to_pair = [&](auto&& _t) {
            return std::make_pair(std::move(std::get<0>(_t)),
                                  std::move(std::get<1>(_t)));
        };
        return Parser<ReaderType, WriterType,
                      std::tuple<FirstType, SecondType>>::read(_r, _var)
            .transform(to_pair);
    }

    /// Transform a std::pair into an array
    static OutputVarType write(
        const WriterType& _w,
        const std::pair<FirstType, SecondType>& _p) noexcept {
        const auto tup = std::make_tuple(&_p.first, &_p.second);
        return Parser<
            ReaderType, WriterType,
            std::tuple<const FirstType*, const SecondType*>>::write(_w, tup);
    }
};

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType,
          internal::StringLiteral _discriminator, class... AlternativeTypes>
struct Parser<ReaderType, WriterType,
              TaggedUnion<_discriminator, AlternativeTypes...>> {
    using ResultType = Result<TaggedUnion<_discriminator, AlternativeTypes...>>;

   public:
    using InputObjectType = typename ReaderType::InputObjectType;
    using InputVarType = typename ReaderType::InputVarType;

    using OutputObjectType = typename WriterType::OutputObjectType;
    using OutputVarType = typename WriterType::OutputVarType;

    /// Expresses the variables as type T.
    static ResultType read(const ReaderType& _r, InputVarType* _var) noexcept {
        const auto get_disc = [&_r](auto _obj) {
            return get_discriminator(_r, &_obj);
        };

        const auto to_result = [&_r, _var](const std::string& _disc_value) {
            return find_matching_alternative(_r, _disc_value, _var);
        };

        return _r.to_object(_var).and_then(get_disc).and_then(to_result);
    }

    /// Expresses the variables as a JSON type.
    static OutputVarType write(
        const WriterType& _w,
        const TaggedUnion<_discriminator, AlternativeTypes...>&
            _tagged_union) noexcept {
        using VariantType =
            typename TaggedUnion<_discriminator,
                                 AlternativeTypes...>::VariantType;
        return Parser<ReaderType, WriterType, VariantType>::write(
            _w, _tagged_union.variant_);
    }

   private:
    template <int _i = 0>
    static ResultType find_matching_alternative(const ReaderType& _r,
                                                const std::string& _disc_value,
                                                InputVarType* _var) noexcept {
        if constexpr (_i == sizeof...(AlternativeTypes)) {
            return Error("Could not parse tagged union, could not match " +
                         _discriminator.str() + " '" + _disc_value + "'.");
        } else {
            using AlternativeType = std::decay_t<std::variant_alternative_t<
                _i, std::variant<AlternativeTypes...>>>;

            if (contains_disc_value<AlternativeType>(_disc_value)) {
                const auto to_tagged_union = [](auto&& _val) {
                    return TaggedUnion<_discriminator, AlternativeTypes...>(
                        std::move(_val));
                };

                const auto embellish_error = [&](Error&& _e) {
                    return Error(
                        "Could not parse tagged union with discrimininator " +
                        _discriminator.str() + " '" + _disc_value +
                        "': " + _e.what());
                };

                return Parser<ReaderType, WriterType, AlternativeType>::read(
                           _r, _var)
                    .transform(to_tagged_union)
                    .or_else(embellish_error);

            } else {
                return find_matching_alternative<_i + 1>(_r, _disc_value, _var);
            }
        }
    }

    /// Retrieves the discriminator.
    static Result<std::string> get_discriminator(
        const ReaderType& _r, InputObjectType* _obj) noexcept {
        const auto embellish_error = [](const auto& _err) {
            return Error(
                "Could not parse tagged union: Could not find field '" +
                _discriminator.str() + "' or type of field was not a string.");
        };

        const auto to_type = [&_r](auto _var) {
            return _r.template to_basic_type<std::string>(&_var);
        };

        return _r.get_field(_discriminator.str(), _obj)
            .and_then(to_type)
            .or_else(embellish_error);
    }

    /// Determines whether the discriminating literal contains the value
    /// retrieved from the object.
    template <class T>
    static inline bool contains_disc_value(
        const std::string& _disc_value) noexcept {
        if constexpr (!internal::has_reflection_type_v<T>) {
            using LiteralType = field_type_t<_discriminator, T>;
            return LiteralType::contains(_disc_value);
        } else {
            using LiteralType =
                field_type_t<_discriminator, typename T::ReflectionType>;
            return LiteralType::contains(_disc_value);
        }
    }
};

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType, class... Ts>
struct Parser<ReaderType, WriterType, std::tuple<Ts...>> {
   public:
    using InputArrayType = typename ReaderType::InputArrayType;
    using InputVarType = typename ReaderType::InputVarType;

    using OutputArrayType = typename WriterType::OutputArrayType;
    using OutputVarType = typename WriterType::OutputVarType;

    static Result<std::tuple<Ts...>> read(const ReaderType& _r,
                                          InputVarType* _var) noexcept {
        const auto to_vec = [&](auto _arr) { return _r.to_vec(&_arr); };

        const auto check_size =
            [](auto _vec) -> Result<std::vector<InputVarType>> {
            if (_vec.size() != sizeof...(Ts)) {
                return Error("Expected " + std::to_string(sizeof...(Ts)) +
                             " fields, got " + std::to_string(_vec.size()) +
                             ".");
            }
            return std::move(_vec);
        };

        const auto extract = [&_r](auto _vec) {
            return extract_field_by_field(_r, std::move(_vec));
        };

        return _r.to_array(_var)
            .transform(to_vec)
            .and_then(check_size)
            .and_then(extract);
    }

    static OutputVarType write(const WriterType& _w,
                               const std::tuple<Ts...>& _tup) noexcept {
        auto arr = _w.new_array();
        to_array<0>(_w, _tup, &arr);
        return arr;
    }

   private:
    /// Extracts values from the array, field by field.
    template <class... AlreadyExtracted>
    static Result<std::tuple<Ts...>> extract_field_by_field(
        const ReaderType& _r, std::vector<InputVarType> _vec,
        AlreadyExtracted&&... _already_extracted) noexcept {
        constexpr size_t i = sizeof...(AlreadyExtracted);
        if constexpr (i == sizeof...(Ts)) {
            return std::tuple<Ts...>(
                std::make_tuple(std::move(_already_extracted)...));
        } else {
            const auto extract_next = [&](auto&& new_entry) {
                return extract_field_by_field(_r, std::move(_vec),
                                              std::move(_already_extracted)...,
                                              std::move(new_entry));
            };
            return extract_single_field<i>(_r, &_vec).and_then(extract_next);
        }
    }

    /// Extracts a single field from a JSON.
    template <int _i>
    static auto extract_single_field(const ReaderType& _r,
                                     std::vector<InputVarType>* _vec) noexcept {
        using NewFieldType = std::decay_t<
            typename std::tuple_element<_i, std::tuple<Ts...>>::type>;
        return Parser<ReaderType, WriterType, NewFieldType>::read(
            _r, &((*_vec)[_i]));
    }

    /// Transforms a tuple to an array.
    template <int _i>
    static void to_array(const WriterType& _w, const std::tuple<Ts...>& _tup,
                         OutputArrayType* _ptr) noexcept {
        if constexpr (_i < sizeof...(Ts)) {
            using NewFieldType = std::decay_t<
                typename std::tuple_element<_i, std::tuple<Ts...>>::type>;

            const auto val =
                Parser<ReaderType, WriterType, NewFieldType>::write(
                    _w, std::get<_i>(_tup));
            _w.add(val, _ptr);
            to_array<_i + 1>(_w, _tup, _ptr);
        }
    }
};

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, std::unique_ptr<T>> {
    using InputVarType = typename ReaderType::InputVarType;
    using OutputVarType = typename WriterType::OutputVarType;

    /// Expresses the variables as type T.
    static Result<std::unique_ptr<T>> read(const ReaderType& _r,
                                           InputVarType* _var) noexcept {
        if (_r.is_empty(_var)) {
            return std::unique_ptr<T>();
        }
        const auto to_ptr = [](auto&& _t) {
            return std::make_unique<T>(std::move(_t));
        };
        return Parser<ReaderType, WriterType, std::decay_t<T>>::read(_r, _var)
            .transform(to_ptr);
    }

    /// Expresses the variable a a JSON.
    static OutputVarType write(const WriterType& _w,
                               const std::unique_ptr<T>& _s) noexcept {
        if (!_s) {
            return _w.empty_var();
        }
        return Parser<ReaderType, WriterType, std::decay_t<T>>::write(_w, *_s);
    }
};

// ----------------------------------------------------------------------------

/// To be used when all options of the variants are rfl::Field. Essentially,
/// this is an externally tagged union.
template <class ReaderType, class WriterType, class... FieldTypes>
struct FieldVariantParser {
    using ResultType = Result<std::variant<FieldTypes...>>;

   public:
    using InputObjectType = typename ReaderType::InputObjectType;
    using InputVarType = typename ReaderType::InputVarType;

    using OutputObjectType = typename WriterType::OutputObjectType;
    using OutputVarType = typename WriterType::OutputVarType;

    /// Expresses the variables as type T.
    static ResultType read(const ReaderType& _r, InputVarType* _var) noexcept {
        static_assert(
            internal::no_duplicate_field_names<std::tuple<FieldTypes...>>(),
            "Externally tagged variants cannot have duplicate field names.");

        const auto to_map = [&](auto _obj) { return _r.to_map(&_obj); };

        const auto to_result = [&](auto _map) -> ResultType {
            if (_map.size() != 1) {
                return Error(
                    "Could not parse Field: Expected the object to have "
                    "exactly one field, but found " +
                    std::to_string(_map.size()) + " fields.");
            }
            const auto it = _map.begin();
            const auto& disc_value = it->first;
            auto& var = it->second;
            return find_matching_alternative(_r, disc_value, &var);
        };

        return _r.to_object(_var).transform(to_map).and_then(to_result);
    }

    /// Expresses the variables as a JSON type.
    static OutputVarType write(const WriterType& _w,
                               const std::variant<FieldTypes...>& _v) noexcept {
        static_assert(
            internal::no_duplicate_field_names<std::tuple<FieldTypes...>>(),
            "Externally tagged variants cannot have duplicate field names.");

        const auto handle = [&](const auto& _field) {
            const auto named_tuple =
                make_named_tuple(internal::to_ptr_field(_field));
            using NamedTupleType = std::decay_t<decltype(named_tuple)>;
            return Parser<ReaderType, WriterType, NamedTupleType>::write(
                _w, named_tuple);
        };

        return std::visit(handle, _v);
    }

   private:
    template <int _i = 0>
    static ResultType find_matching_alternative(const ReaderType& _r,
                                                const std::string& _disc_value,
                                                InputVarType* _var) noexcept {
        if constexpr (_i == sizeof...(FieldTypes)) {
            return Error(
                "Could not parse std::variant, could not match field named '" +
                _disc_value + "'.");
        } else {
            using FieldType = std::decay_t<typename std::tuple_element<
                _i, std::tuple<FieldTypes...>>::type>;

            using ValueType = std::decay_t<typename FieldType::Type>;

            const auto key = FieldType::name_.str();

            if (key == _disc_value) {
                const auto to_variant = [](ValueType&& _val) {
                    return std::variant<FieldTypes...>(
                        FieldType(std::move(_val)));
                };

                const auto embellish_error = [&](const Error& _e) {
                    return Error("Could not parse std::variant with field '" +
                                 _disc_value + "': " + _e.what());
                };

                return Parser<ReaderType, WriterType, ValueType>::read(_r, _var)
                    .transform(to_variant)
                    .or_else(embellish_error);
            } else {
                return find_matching_alternative<_i + 1>(_r, _disc_value, _var);
            }
        }
    }
};

// ----------------------------------------------------------------------------

template <class ReaderType, class WriterType, class... FieldTypes>
struct Parser<ReaderType, WriterType, std::variant<FieldTypes...>> {
    using InputVarType = typename ReaderType::InputVarType;
    using OutputVarType = typename WriterType::OutputVarType;

    /// Expresses the variables as type T.
    template <int _i = 0>
    static Result<std::variant<FieldTypes...>> read(
        const ReaderType& _r, InputVarType* _var,
        const std::string _errors = "") noexcept {
        if constexpr (_i == 0 &&
                      internal::all_fields<std::tuple<FieldTypes...>>()) {
            return FieldVariantParser<ReaderType, WriterType,
                                      FieldTypes...>::read(_r, _var);
        } else if constexpr (_i == sizeof...(FieldTypes)) {
            return Error("Could not parse variant: " + _errors);
        } else {
            const auto to_variant = [](auto&& _val) {
                return std::variant<FieldTypes...>(std::move(_val));
            };

            const auto try_next = [&_r, _var, &_errors](const auto& _err) {
                auto errors = _errors;
                errors += std::string("\n -") + _err.what();
                return read<_i + 1>(_r, _var, errors);
            };

            using AltType = std::decay_t<
                std::variant_alternative_t<_i, std::variant<FieldTypes...>>>;

            return Parser<ReaderType, WriterType, AltType>::read(_r, _var)
                .transform(to_variant)
                .or_else(try_next);
        }
    }

    /// Expresses the variables as a JSON type.
    static OutputVarType write(
        const WriterType& _w,
        const std::variant<FieldTypes...>& _variant) noexcept {
        if constexpr (internal::all_fields<std::tuple<FieldTypes...>>()) {
            return FieldVariantParser<ReaderType, WriterType,
                                      FieldTypes...>::write(_w, _variant);
        } else {
            const auto handle = [&](const auto& _v) {
                using Type = std::decay_t<decltype(_v)>;
                return Parser<ReaderType, WriterType, Type>::write(_w, _v);
            };
            return std::visit(handle, _variant);
        }
    }
};

// ----------------------------------------------------------------------------

/// This can be used for data structures that would be expressed as array in
/// serialized format (std::vector, std::set, std::deque, ...),
/// but also includes map-like types, when the key is not of type std::string.
template <class ReaderType, class WriterType, class VecType>
struct VectorParser {
   public:
    using InputArrayType = typename ReaderType::InputArrayType;
    using InputVarType = typename ReaderType::InputVarType;

    using OutputArrayType = typename WriterType::OutputArrayType;
    using OutputVarType = typename WriterType::OutputVarType;

    using T = typename VecType::value_type;

    static Result<VecType> read(const ReaderType& _r,
                                InputVarType* _var) noexcept {
        using namespace std::ranges::views;

        const auto get_elem = [&](auto& _v) {
            return Parser<ReaderType, WriterType, std::decay_t<T>>::read(_r,
                                                                         &_v)
                .value();
        };

        const auto get_pair = [&](auto& _v) {
            if constexpr (is_map_like<VecType>()) {
                using K = std::decay_t<typename T::first_type>;
                using V = std::decay_t<typename T::second_type>;
                return Parser<ReaderType, WriterType,
                              std::decay_t<std::pair<K, V>>>::read(_r, &_v)
                    .value();
            }
        };

        const auto to_container =
            [&](InputArrayType&& _arr) -> Result<VecType> {
            auto input_vars = _r.to_vec(&_arr);
            VecType vec;
            if constexpr (is_forward_list<VecType>()) {
                for (auto it = input_vars.rbegin(); it != input_vars.rend();
                     ++it) {
                    vec.emplace_front(get_elem(*it));
                }
            } else if constexpr (is_map_like<VecType>()) {
                for (auto& v : input_vars) {
                    vec.insert(get_pair(v));
                }
            } else if constexpr (is_set_like<VecType>()) {
                for (auto& v : input_vars) {
                    vec.insert(get_elem(v));
                }
            } else {
                for (auto& v : input_vars) {
                    vec.emplace_back(get_elem(v));
                }
            }
            return vec;
        };

        const auto to_result = [&](InputArrayType&& _arr) -> Result<VecType> {
            try {
                return Result<VecType>(
                    to_container(std::forward<InputArrayType>(_arr)));
            } catch (std::exception& e) {
                return Error(e.what());
            }
        };

        return _r.to_array(_var).and_then(to_result);
    }

    static OutputVarType write(const WriterType& _w,
                               const VecType& _vec) noexcept {
        auto arr = _w.new_array();
        for (const auto& v : _vec) {
            _w.add(
                Parser<ReaderType, WriterType, std::decay_t<T>>::write(_w, v),
                &arr);
        }
        return OutputVarType(arr);
    }
};

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, std::deque<T>>
    : public VectorParser<ReaderType, WriterType, std::deque<T>> {};

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, std::forward_list<T>>
    : public VectorParser<ReaderType, WriterType, std::forward_list<T>> {};

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, std::list<T>>
    : public VectorParser<ReaderType, WriterType, std::list<T>> {};

template <class ReaderType, class WriterType, class K, class V>
struct Parser<ReaderType, WriterType, std::map<K, V>>
    : public VectorParser<ReaderType, WriterType, std::map<K, V>> {};

template <class ReaderType, class WriterType, class K, class V>
struct Parser<ReaderType, WriterType, std::multimap<K, V>>
    : public VectorParser<ReaderType, WriterType, std::multimap<K, V>> {};

template <class ReaderType, class WriterType, class K, class V>
struct Parser<ReaderType, WriterType, std::multiset<K, V>>
    : public VectorParser<ReaderType, WriterType, std::multiset<K, V>> {};

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, std::set<T>>
    : public VectorParser<ReaderType, WriterType, std::set<T>> {};

template <class ReaderType, class WriterType, class K, class V>
struct Parser<ReaderType, WriterType, std::unordered_map<K, V>>
    : public VectorParser<ReaderType, WriterType, std::unordered_map<K, V>> {};

template <class ReaderType, class WriterType, class K, class V>
struct Parser<ReaderType, WriterType, std::unordered_multiset<K, V>>
    : public VectorParser<ReaderType, WriterType,
                          std::unordered_multiset<K, V>> {};

template <class ReaderType, class WriterType, class K, class V>
struct Parser<ReaderType, WriterType, std::unordered_multimap<K, V>>
    : public VectorParser<ReaderType, WriterType,
                          std::unordered_multimap<K, V>> {};

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, std::unordered_set<T>>
    : public VectorParser<ReaderType, WriterType, std::unordered_set<T>> {};

template <class ReaderType, class WriterType, class T>
struct Parser<ReaderType, WriterType, std::vector<T>>
    : public VectorParser<ReaderType, WriterType, std::vector<T>> {};

}  // namespace parsing
}  // namespace rfl

#endif  // JSON_PARSER_HPP_
