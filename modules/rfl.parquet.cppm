module;

#include "rfl/parquet.hpp"

export module rfl.parquet;

export import rfl;

export namespace rfl::parquet {
using ::rfl::parquet::Compression;
using ::rfl::parquet::Settings;
using ::rfl::parquet::load;
using ::rfl::parquet::read;
using ::rfl::parquet::save;
using ::rfl::parquet::write;
}
