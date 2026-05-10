#ifndef RFL_PARSING_TABULAR_ARROWREADER_HPP_
#define RFL_PARSING_TABULAR_ARROWREADER_HPP_

#include <arrow/api.h>

#include <array>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "../../Processors.hpp"
#include "../../Result.hpp"
#include "../../Tuple.hpp"
#include "../../apply.hpp"
#include "../../get.hpp"
#include "../../internal/add_tags_to_variants_v.hpp"
#include "../../internal/default_if_missing_v.hpp"
#include "../../internal/no_extra_fields_v.hpp"
#include "../../internal/no_field_names_v.hpp"
#include "../../internal/no_optionals_v.hpp"
#include "../../named_tuple_t.hpp"
#include "../../to_view.hpp"
#include "../../view_t.hpp"
#include "../call_destructors_where_necessary.hpp"
#include "make_chunked_array_iterators.hpp"

namespace rfl::parsing::tabular {

template <class VecType, SerializationType _s, class... Ps>
class ArrowReader {
  static_assert(!internal::add_tags_to_variants_v<Processors<Ps...>>,
                "rfl::AddTagsToVariants cannot be used for tabular data.");
  static_assert(
      !internal::add_namespaced_tags_to_variants_v<Processors<Ps...>>,
      "rfl::AddNamespacedTagsToVariants cannot be used for tabular data.");
  static_assert(!internal::no_optionals_v<Processors<Ps...>>,
                "rfl::NoOptionals cannot be used for tabular data.");
  static_assert(!internal::default_if_missing_v<Processors<Ps...>>,
                "rfl::DefaultIfMissing cannot be used for tabular data.");
  static_assert(!internal::no_extra_fields_v<Processors<Ps...>>,
                "rfl::NoExtraFields cannot be used for tabular data (neither "
                "can rfl::ExtraFields).");
  static_assert(!internal::no_field_names_v<Processors<Ps...>>,
                "rfl::NoFieldNames cannot be used for tabular data.");

 public:
  using ValueType = typename std::remove_cvref_t<typename VecType::value_type>;

  /**
   * @brief Creates a new ArrowReader.
   * @param _table The arrow table to read from.
   * @return An ArrowReader or an error.
   */
  static Result<ArrowReader> make(const std::shared_ptr<arrow::Table>& _table) {
    try {
      return ArrowReader(_table);
    } catch (const std::exception& e) {
      return error(std::string("Could not create ArrowReader: ") + e.what());
    }
  }

  ~ArrowReader() = default;

  /**
   * @brief Reads the data from the arrow table into a vector-like container.
   * @return The vector-like container or an error.
   */
  Result<VecType> read() const noexcept {
    return make_chunked_array_iterators<named_tuple_t<ValueType, Ps...>, _s>(
               table_)
        .and_then([&](auto chunked_array_iterators) -> Result<VecType> {
          VecType result;
          while (!end(chunked_array_iterators)) {
            auto value = new_value(&chunked_array_iterators);
            if (!value) {
              return error(value.error().what());
            }
            result.emplace_back(std::move(*value));
          }
          return result;
        });
  }

 private:
  /**
   * @brief Constructor.
   * @param _table The arrow table to read from.
   */
  ArrowReader(const std::shared_ptr<arrow::Table>& _table)
      : table_(Ref<arrow::Table>::make(_table).value()) {}

  /**
   * @brief Checks if the end of any chunked array iterator has been reached.
   * @param _chunked_array_iterators The iterators to check.
   * @return true if the end has been reached, false otherwise.
   */
  bool end(const auto& _chunked_array_iterators) const {
    return apply(
        [](const auto&... _its) { return (false || ... || _its.end()); },
        _chunked_array_iterators);
  }

  /**
   * @brief Creates a new value from the chunked array iterators.
   * @param _chunked_array_iterators The iterators to read from.
   * @return The new value or an error.
   */
  Result<ValueType> new_value(auto* _chunked_array_iterators) const noexcept {
    alignas(ValueType) unsigned char buf[sizeof(ValueType)]{};
    auto ptr = internal::ptr_cast<ValueType*>(&buf);

    auto view = to_view(*ptr);

    using ViewType = std::remove_cvref_t<decltype(view)>;

    try {
      const auto set_one = [&]<size_t _i>(std::integral_constant<size_t, _i>) {
        using FieldType = tuple_element_t<_i, typename ViewType::Fields>;
        using T = std::remove_cvref_t<
            std::remove_pointer_t<typename FieldType::Type>>;
        auto res = *_chunked_array_iterators->template get<_i>();
        if (!res) {
          destroy_value<_i>(&view);
          throw std::runtime_error(
              std::string("Field '") + typename FieldType::Name().str() +
              std::string("' could not be set: ") + res.error().what());
        }
        ::new (view.template get<_i>()) T(std::move(*res));
        ++_chunked_array_iterators->template get<_i>();
      };

      [&]<size_t... _is>(std::integer_sequence<size_t, _is...>) {
        (set_one(std::integral_constant<size_t, _is>{}), ...);
      }(std::make_integer_sequence<size_t, view.size()>());
    } catch (const std::exception& e) {
      return error(e.what());
    }

    return std::move(*ptr);
  }

  /**
   * @brief Destroys the values that have been set in the view so far.
   * @tparam _i The number of values that have been set.
   * @tparam ViewType The type of the view.
   * @param _view The view to destroy.
   */
  template <size_t _i, class ViewType>
  void destroy_value(ViewType* _view) const {
    static_assert(_i < ViewType::size(), "_i out of bounds.");
    auto set = std::array<bool, ViewType::size()>();
    for (size_t i = 0; i < _i; ++i) {
      set[i] = true;
    }
    for (size_t i = _i; i < ViewType::size(); ++i) {
      set[i] = false;
    }
    call_destructors_where_necessary(set, _view);
  }

 private:
  Ref<arrow::Table> table_;
};

}  // namespace rfl::parsing::tabular

#endif
