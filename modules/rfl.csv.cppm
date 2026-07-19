module;

#include "rfl/csv.hpp"

export module rfl.csv;

export import rfl;

export namespace rfl::csv {
using ::rfl::csv::Settings;
using ::rfl::csv::load;
using ::rfl::csv::read;
using ::rfl::csv::save;
using ::rfl::csv::write;
}
