module;

#include "rfl/internal/xml_module_constants.hpp"
#include "rfl/xml.hpp"

export module rfl.xml;

export import rfl;

export namespace rfl::xml {
using ::rfl::xml::load;
using ::rfl::xml::read;
using ::rfl::xml::save;
using ::rfl::xml::write;
}
