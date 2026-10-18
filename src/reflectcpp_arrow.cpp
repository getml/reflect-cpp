#include "rfl/parsing/tabular/arrow_builder_operations.hpp"

namespace rfl::parsing::tabular {
arrow::Status append_string(arrow::StringBuilder* _builder,
                            std::string_view _value) {
  return _builder->Append(_value);
}
arrow::Status append_null(arrow::ArrayBuilder* _builder) {
  return _builder->AppendNull();
}
}  // namespace rfl::parsing::tabular
