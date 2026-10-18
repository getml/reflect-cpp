#ifndef RFL_PARSING_TABULAR_ARROWBRIDGE_HPP_
#define RFL_PARSING_TABULAR_ARROWBRIDGE_HPP_

#ifndef RFL_ARROW_MODULE_INTERFACE
#include <arrow/api.h>
#else
#include <arrow/type_fwd.h>
#endif
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "../../Ref.hpp"
#include "../../Result.hpp"
#include "../../common.hpp"

namespace rfl::parsing::tabular {

enum class SerializationType { csv, parquet };

// Owns a native builder without instantiating Arrow's implementation in
// importers.
class RFL_API ArrowBuilder {
 public:
  explicit ArrowBuilder(std::shared_ptr<arrow::ArrayBuilder> _impl);
  void append_null();
  std::shared_ptr<arrow::Array> finish();
  arrow::ArrayBuilder* get() const;

 private:
  std::shared_ptr<arrow::ArrayBuilder> impl_;
};

RFL_API std::shared_ptr<arrow::DataType> binary_data_type();
RFL_API ArrowBuilder make_binary_builder();
RFL_API void append_binary(ArrowBuilder*, const uint8_t*, size_t);
RFL_API Result<Ref<arrow::Array>> binary_array(
    const std::shared_ptr<arrow::Array>&);
RFL_API std::string_view binary_value(const Ref<arrow::Array>&, int64_t);
RFL_API std::shared_ptr<arrow::DataType> timestamp_data_type();
RFL_API ArrowBuilder make_timestamp_builder();
RFL_API void append_timestamp(ArrowBuilder*, int64_t);
RFL_API Result<Ref<arrow::Array>> timestamp_array(
    const std::shared_ptr<arrow::Array>&,
    Result<int64_t> (*)(std::string_view));
RFL_API int64_t timestamp_value(const Ref<arrow::Array>&, int64_t);
RFL_API bool array_is_null(const Ref<arrow::Array>&, int64_t);
RFL_API int64_t array_length(const Ref<arrow::Array>&);
RFL_API int num_chunks(const Ref<arrow::ChunkedArray>&);
RFL_API std::shared_ptr<arrow::Array> array_chunk(
    const Ref<arrow::ChunkedArray>&, int);
RFL_API Result<Ref<arrow::ChunkedArray>> table_column(const Ref<arrow::Table>&,
                                                      const std::string&);
RFL_API std::shared_ptr<arrow::Field> make_arrow_field(
    const std::string&, const std::shared_ptr<arrow::DataType>&);
RFL_API std::shared_ptr<arrow::Schema> make_schema(
    const std::vector<std::shared_ptr<arrow::Field>>&);
RFL_API std::shared_ptr<arrow::Table> make_table(
    const std::shared_ptr<arrow::Schema>&,
    const std::vector<std::shared_ptr<arrow::ChunkedArray>>&);
RFL_API std::shared_ptr<arrow::ChunkedArray> make_chunked_array(
    const std::vector<std::shared_ptr<arrow::Array>>&,
    const std::shared_ptr<arrow::DataType>&);
RFL_API std::string_view buffer_view(const Ref<arrow::Buffer>&);

}  // namespace rfl::parsing::tabular
#endif
