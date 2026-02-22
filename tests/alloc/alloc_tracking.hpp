#pragma once

#include <cstddef>

namespace alloc_tracking {

/// RAII guard: enables allocation tracking on construction.
/// Call leaks() to get count of un-freed new[] allocations since construction.
struct Guard {
  Guard();
  ~Guard();
  size_t leaks() const;
};

}  // namespace alloc_tracking
