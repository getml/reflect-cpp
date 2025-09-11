#ifndef RFL_PARSING_TABULAR_ARROWWRITER_HPP_
#define RFL_PARSING_TABULAR_ARROWWRITER_HPP_

#include <arrow/api.h>

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "../../Processors.hpp"
#include "../../Tuple.hpp"
#include "../../get.hpp"
#include "../../named_tuple_t.hpp"
#include "../../to_view.hpp"
#include "add_to_builder.hpp"
#include "make_arrow_builders.hpp"
#include "make_arrow_data_types.hpp"
#include "make_arrow_schema.hpp"

namespace rfl::parsing::tabular {

template <class VecType, SerializationType _s, class... Ps>
class ArrowWriter {
  static_assert(!Processors<Ps...>::add_tags_to_variants_,
                "rfl::AddTagsToVariants cannot be used for tabular data.");
  static_assert(!Processors<Ps...>::add_namespaced_tags_to_variants_,
                "rfl::AddNamespacedTagsToVariants cannot be used for tabular data.");
  static_assert(!Processors<Ps...>::all_required_,
                "rfl::NoOptionals cannot be used for tabular data.");
  static_assert(!Processors<Ps...>::default_if_missing_,
                "rfl::DefaultIfMissing cannot be used for tabular data.");
  static_assert(!Processors<Ps...>::no_extra_fields_,
                "rfl::NoExtraFields cannot be used for tabular data (neither "
                "can rfl::ExtraFields).");
  static_assert(!Processors<Ps...>::no_field_names_,
                "rfl::NoFieldNames cannot be used for tabular data.");

 public:
  using ValueType = typename std::remove_cvref_t<typename VecType::value_type>;

  ArrowWriter(const size_t _chunksize) : chunksize_(_chunksize) {}

  ~ArrowWriter() = default;

  std::shared_ptr<arrow::Table> to_table(const VecType& _data) const {
    return arrow::Table::Make(
        make_arrow_schema<named_tuple_t<ValueType, Ps...>, _s>(),
        to_chunked_arrays(_data));
  }

 private:
  std::vector<std::shared_ptr<arrow::ChunkedArray>> to_chunked_arrays(
      const VecType& _data) const;

 private:
  size_t chunksize_;
};

template <class VecType, SerializationType _s, class... Ps>
std::vector<std::shared_ptr<arrow::ChunkedArray>>
ArrowWriter<VecType, _s, Ps...>::to_chunked_arrays(const VecType& _data) const {
  using ValueType = typename VecType::value_type;

  auto builders =
      make_arrow_builders<named_tuple_t<typename VecType::value_type>, _s>();

  constexpr size_t size = tuple_size_v<decltype(builders)>;

  std::vector<std::vector<std::shared_ptr<arrow::Array>>> array_chunks(size);

  auto it = _data.begin();

  while (it != _data.end()) {
    size_t i = 0;

    for (; it != _data.end() && (i < chunksize_ || chunksize_ == 0);
         ++i, ++it) {
      const auto view = to_view(*it);

      [&]<int... _is>(const auto& _v, auto* _b,
                      std::integer_sequence<int, _is...>) {
        (add_to_builder<_s>(*get<_is>(_v), &(_b->template get<_is>())), ...);
      }(view, &builders, std::make_integer_sequence<int, size>());
    }

    if (i != 0) {
      std::vector<std::shared_ptr<arrow::Array>> chunks(size);

      const auto finish_builder = [](auto* _b, auto* _c) {
        const auto status = _b->Finish(_c);
        if (!status.ok()) {
          throw std::runtime_error(status.message());
        }
      };

      [&]<size_t... _is>(auto* _b, auto* _c,
                         std::integer_sequence<size_t, _is...>) {
        (finish_builder(&_b->template get<_is>(), &_c->at(_is)), ...);
      }(&builders, &chunks, std::make_integer_sequence<size_t, size>());

      for (size_t j = 0; j < size; ++j) {
        array_chunks.at(j).emplace_back(std::move(chunks.at(j)));
      }
    }
  }

  const auto data_types = make_arrow_data_types<ValueType, _s>();

  return [&]<size_t... _is>(std::integer_sequence<size_t, _is...>) {
    return std::vector<std::shared_ptr<arrow::ChunkedArray>>(
        {std::make_shared<arrow::ChunkedArray>(array_chunks.at(_is),
                                               std::get<_is>(data_types))...});
  }(std::make_integer_sequence<size_t, size>());
}

}  // namespace rfl::parsing::tabular

#endif
