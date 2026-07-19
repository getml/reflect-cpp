module;

#include "rfl/boost_serialization.hpp"

export module rfl.boost_serialization;

export import rfl;

export namespace rfl::boost_serialization {
using ::rfl::boost_serialization::load;
using ::rfl::boost_serialization::read;
using ::rfl::boost_serialization::save;
using ::rfl::boost_serialization::write;
}
