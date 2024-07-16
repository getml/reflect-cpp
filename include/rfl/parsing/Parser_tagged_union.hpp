#ifndef RFL_PARSING_PARSER_TAGGED_UNION_HPP_
#define RFL_PARSING_PARSER_TAGGED_UNION_HPP_

#include <map>
#include <type_traits>

#include "../Result.hpp"
#include "../TaggedUnion.hpp"
#include "../always_false.hpp"
#include "../internal/strings/join.hpp"
#include "../named_tuple_t.hpp"
#include "Parser_base.hpp"
#include "TaggedUnionWrapper.hpp"
#include "is_tagged_union_wrapper.hpp"
#include "schema/Type.hpp"
#include "tagged_union_wrapper_no_ptr.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, internal::StringLiteral _discriminator,
          class... AlternativeTypes, class ProcessorsType>
requires AreReaderAndWriter<R, W,
                            TaggedUnion<_discriminator, AlternativeTypes...>>
struct Parser<R, W, TaggedUnion<_discriminator, AlternativeTypes...>,
              ProcessorsType> {
  using ResultType = Result<TaggedUnion<_discriminator, AlternativeTypes...>>;

 public:
  using InputObjectType = typename R::InputObjectType;
  using InputVarType = typename R::InputVarType;

  using OutputObjectType = typename W::OutputObjectType;
  using OutputVarType = typename W::OutputVarType;

  constexpr static bool no_field_names_ = ProcessorsType::no_field_names_;

  using InputObjectOrArrayType =
      std::conditional_t<no_field_names_, typename R::InputArrayType,
                         typename R::InputObjectType>;

  static ResultType read(const R& _r, const InputVarType& _var) noexcept {
    const auto get_disc =
        [&_r](InputObjectOrArrayType _obj_or_arr) -> Result<std::string> {
      return get_discriminator(_r, _obj_or_arr);
    };

    const auto to_result =
        [&_r, _var](const std::string& _disc_value) -> ResultType {
      return find_matching_alternative(
          _r, _disc_value, _var,
          std::make_integer_sequence<int, sizeof...(AlternativeTypes)>());
    };

    if constexpr (no_field_names_) {
      return _r.to_array(_var).and_then(get_disc).and_then(to_result);
    } else {
      return _r.to_object(_var).and_then(get_disc).and_then(to_result);
    }
  }

  template <class P>
  static void write(
      const W& _w,
      const TaggedUnion<_discriminator, AlternativeTypes...>& _tagged_union,
      const P& _parent) noexcept {
    const auto handle = [&](const auto& _val) {
      write_wrapped(_w, _val, _parent);
    };
    rfl::visit(handle, _tagged_union.variant_);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) noexcept {
    using VariantType = std::variant<std::invoke_result_t<
        decltype(wrap_if_necessary<AlternativeTypes>), AlternativeTypes>...>;
    return Parser<R, W, VariantType, ProcessorsType>::to_schema(_definitions);
  }

 private:
  template <int... _is>
  static ResultType find_matching_alternative(
      const R& _r, const std::string& _disc_value, const InputVarType& _var,
      std::integer_sequence<int, _is...>) noexcept {
    ResultType res = Error("");
    bool match_found = false;
    (set_if_disc_value_matches<_is>(_r, _disc_value, _var, &res, &match_found),
     ...);
    if (match_found) [[likely]] {
      return res;
    } else {
      const auto names =
          TaggedUnion<_discriminator,
                      AlternativeTypes...>::PossibleTags::names();
      return Error("Could not parse tagged union, could not match " +
                   _discriminator.str() + " '" + _disc_value +
                   "'. The following tags are allowed: " +
                   internal::strings::join(", ", names));
    }
  }

  template <int _i>
  static void set_if_disc_value_matches(const R& _r,
                                        const std::string& _disc_value,
                                        const InputVarType& _var,
                                        ResultType* _res,
                                        bool* _match_found) noexcept {
    using AlternativeType = std::remove_cvref_t<
        std::variant_alternative_t<_i, std::variant<AlternativeTypes...>>>;

    if (!*_match_found && contains_disc_value<AlternativeType>(_disc_value)) {
      const auto get_fields = [](auto&& _val) -> AlternativeType {
        if constexpr (is_tagged_union_wrapper_v<decltype(_val)>) {
          return std::move(_val.fields());
        } else {
          return std::move(_val);
        }
      };

      const auto to_tagged_union = [](auto&& _val) {
        return TaggedUnion<_discriminator, AlternativeTypes...>(
            std::move(_val));
      };

      const auto embellish_error = [&](Error&& _e) {
        return Error(
            "Could not parse tagged union with "
            "discrimininator " +
            _discriminator.str() + " '" + _disc_value + "': " + _e.what());
      };

      if constexpr (no_field_names_) {
        using T = tagged_union_wrapper_no_ptr_t<std::invoke_result_t<
            decltype(wrap_if_necessary<AlternativeType>), AlternativeType>>;
        *_res = Parser<R, W, T, ProcessorsType>::read(_r, _var)
                    .transform(get_fields)
                    .transform(to_tagged_union)
                    .or_else(embellish_error);
      } else {
        *_res = Parser<R, W, AlternativeType, ProcessorsType>::read(_r, _var)
                    .transform(to_tagged_union)
                    .or_else(embellish_error);
      }

      *_match_found = true;
    }
  }

  /// Retrieves the discriminator from an object
  static Result<std::string> get_discriminator(
      const R& _r, const InputObjectOrArrayType& _obj_or_arr) noexcept {
    const auto to_type = [&_r](auto _var) {
      return _r.template to_basic_type<std::string>(_var);
    };

    const auto embellish_error = [](const auto&) {
      return Error("Could not parse tagged union: Could not find field '" +
                   _discriminator.str() +
                   "' or type of field was not a string.");
    };

    if constexpr (no_field_names_) {
      return _r.get_field_from_array(0, _obj_or_arr)
          .and_then(to_type)
          .or_else(embellish_error);
    } else {
      return _r.get_field_from_object(_discriminator.str(), _obj_or_arr)
          .and_then(to_type)
          .or_else(embellish_error);
    }
  }

  /// Determines whether the discriminating literal contains the value
  /// retrieved from the object.
  template <class T>
  static inline bool contains_disc_value(
      const std::string& _disc_value) noexcept {
    return internal::tag_t<_discriminator, T>::contains(_disc_value);
  }

  /// Writes a wrapped version of the original object, which contains the tag.
  template <class T, class P>
  static void write_wrapped(const W& _w, const T& _val,
                            const P& _parent) noexcept {
    const auto wrapped = wrap_if_necessary<T>(_val);
    Parser<R, W, std::remove_cvref_t<decltype(wrapped)>, ProcessorsType>::write(
        _w, wrapped, _parent);
  }

  /// Generates a wrapped version of the original object, which contains the
  /// tag, if the object doesn't already contain the wrap.
  template <class T>
  static auto wrap_if_necessary(const T& _val) noexcept {
    if constexpr (named_tuple_t<T>::Names::template contains<
                      _discriminator>()) {
      return _val;
    } else {
      const auto tag = internal::make_tag<_discriminator, T>(_val);
      using TagType = std::remove_cvref_t<decltype(tag)>;
      if constexpr (internal::has_fields<std::remove_cvref_t<T>>()) {
        using WrapperType =
            TaggedUnionWrapperWithFields<T, TagType, _discriminator>;
        return WrapperType{.tag = tag, .fields = &_val};
      } else {
        using WrapperType =
            TaggedUnionWrapperNoFields<T, TagType, _discriminator>;
        return WrapperType{.tag = tag, .fields = &_val};
      }
    }
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
