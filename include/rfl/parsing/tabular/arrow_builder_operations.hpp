#ifndef RFL_PARSING_TABULAR_ARROW_BUILDER_OPERATIONS_HPP_
#define RFL_PARSING_TABULAR_ARROW_BUILDER_OPERATIONS_HPP_

#include <arrow/array/builder_binary.h>

#include <string_view>

#include "../../common.hpp"

namespace rfl::parsing::tabular {
// Keep Arrow's virtual Reserve()/length() calls out of the record templates.
RFL_API arrow::Status append_string(arrow::StringBuilder* _builder,
                                    std::string_view _value);
RFL_API arrow::Status append_null(arrow::ArrayBuilder* _builder);
}  // namespace rfl::parsing::tabular

#endif
