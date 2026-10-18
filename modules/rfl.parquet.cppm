module;

#define RFL_ARROW_MODULE_INTERFACE
#include "rfl/parquet.hpp"
#undef RFL_ARROW_MODULE_INTERFACE

export module rfl.parquet;

export import rfl;

export namespace rfl::parquet {
using ::rfl::parquet::Compression;
using ::rfl::parquet::load;
using ::rfl::parquet::read;
using ::rfl::parquet::save;
using ::rfl::parquet::Settings;
using ::rfl::parquet::write;
}  // namespace rfl::parquet
