#ifndef RFL_CSV_SETTINGS_HPP_
#define RFL_CSV_SETTINGS_HPP_

#include <arrow/io/api.h>

#include "../Field.hpp"
#include "../replace.hpp"

namespace rfl::csv {

struct Settings {
  /// The size of the chunks of the csv file.
  size_t chunksize = 2000;

  Settings with_chunksize(const size_t _chunksize) const noexcept {
    return replace(*this, make_field<"chunksize">(_chunksize));
  }
};

}  // namespace rfl::csv

#endif
