module;

#include "rfl/yas.hpp"

export module rfl.yas;
export import rfl;

export namespace rfl::yas {
using ::rfl::yas::IArchive;
using ::rfl::yas::load;
using ::rfl::yas::OArchive;
using ::rfl::yas::Parser;
using ::rfl::yas::read;
using ::rfl::yas::read_from_archive;
using ::rfl::yas::Reader;
using ::rfl::yas::save;
using ::rfl::yas::write;
using ::rfl::yas::write_to_archive;
using ::rfl::yas::Writer;
}  // namespace rfl::yas
