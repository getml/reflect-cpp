#ifndef RFL_PARSING_TABULAR_ARROWREADER_HPP_
#define RFL_PARSING_TABULAR_ARROWREADER_HPP_

#include <arrow/api.h>

#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "../../Result.hpp"
#include "../../Tuple.hpp"
#include "../../apply.hpp"
#include "../../get.hpp"
#include "../../named_tuple_t.hpp"
#include "../../to_view.hpp"
#include "../call_destructors_where_necessary.hpp"
#include "make_chunked_array_iterators.hpp"

namespace rfl::parsing::tabular {

template <class VecType>
class ArrowReader {
 public:
  using ValueType = typename std::remove_cvref_t<typename VecType::value_type>;

  static Result<ArrowReader> make(const std::shared_ptr<arrow::Table>& _table) {
    try {
      return ArrowReader(_table);
    } catch (const std::exception& e) {
      return error(std::string("Could not create ArrowReader: ") + e.what());
    }
  }

  ~ArrowReader() = default;

  Result<VecType> read() const noexcept {
    try {
      auto chunked_array_iterators =
          make_chunked_array_iterators<named_tuple_t<ValueType>>(table_)
              .value();
      VecType result;
      while (!end(chunked_array_iterators)) {
        auto value = new_value(&chunked_array_iterators);
        if (!value) {
          return error(value.error().what());
        }
        result.emplace_back(std::move(*value));
      }
      return result;
    } catch (const std::exception& e) {
      return error(e.what());
    }
  }

 private:
  ArrowReader(const std::shared_ptr<arrow::Table>& _table)
      : table_(Ref<arrow::Table>::make(_table).value()) {}

  bool end(const auto& _chunked_array_iterators) const {
    return apply(
        [](const auto&... _its) { return (false || ... || _its.end()); },
        _chunked_array_iterators);
  }

  Result<ValueType> new_value(auto* _chunked_array_iterators) const noexcept {
    alignas(ValueType) unsigned char buf[sizeof(ValueType)]{};
    auto ptr = internal::ptr_cast<ValueType*>(&buf);
    // TODO
    // auto view = ProcessorsType::template process<T>(to_view(*ptr));
    auto view = to_view(*ptr);
    using ViewType = std::remove_cvref_t<decltype(view)>;
    auto set = std::array<bool, ViewType::size()>();
    set.fill(false);
    try {
      const auto set_one = [&]<size_t _i>(std::integral_constant<size_t, _i>) {
        using FieldType = tuple_element_t<_i, typename ViewType::Fields>;
        using T = std::remove_cvref_t<
            std::remove_pointer_t<typename FieldType::Type>>;
        ::new (view.template get<_i>()) T(
            std::move((*_chunked_array_iterators->template get<_i>()).value()));
        std::get<_i>(set) = true;
        ++_chunked_array_iterators->template get<_i>();
      };

      [&]<size_t... _is>(std::integer_sequence<size_t, _is...>) {
        (set_one(std::integral_constant<size_t, _is>{}), ...);
      }(std::make_integer_sequence<size_t, view.size()>());

      return std::move(*ptr);
    } catch (const std::exception& e) {
      call_destructors_where_necessary(set, &view);
      return error(e.what());
    }
  }

 private:
  Ref<arrow::Table> table_;
};

}  // namespace rfl::parsing::tabular

#endif
