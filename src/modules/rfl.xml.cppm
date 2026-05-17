module;

#include "rfl/xml.hpp"

export module rfl.xml;
export import rfl;

export namespace rfl::xml {
using ::rfl::xml::InputVarType;
using ::rfl::xml::load;
using ::rfl::xml::Parser;
using ::rfl::xml::read;
using ::rfl::xml::Reader;
using ::rfl::xml::save;
using ::rfl::xml::write;
using ::rfl::xml::Writer;
}  // namespace rfl::xml
