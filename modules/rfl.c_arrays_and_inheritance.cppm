module;

#define REFLECT_CPP_C_ARRAYS_OR_INHERITANCE
#include "rfl.hpp"
#include "rfl/json.hpp"
#include "rfl/num_fields.hpp"

export module rfl.c_arrays_and_inheritance;

export namespace rfl {
using ::rfl::make_field;
using ::rfl::named_tuple_t;
using ::rfl::num_fields;
using ::rfl::replace;
using ::rfl::to_view;
using ::rfl::tuple_element_t;
}

export namespace rfl::json {
using ::rfl::json::read;
using ::rfl::json::write;
}
