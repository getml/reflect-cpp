module;

#define RFL_ARROW_MODULE_INTERFACE
#include "rfl/csv.hpp"
#undef RFL_ARROW_MODULE_INTERFACE

export module rfl.csv;

export import rfl;

export namespace rfl::csv {
using ::rfl::csv::load;
using ::rfl::csv::read;
using ::rfl::csv::save;
using ::rfl::csv::Settings;
using ::rfl::csv::write;
}  // namespace rfl::csv
