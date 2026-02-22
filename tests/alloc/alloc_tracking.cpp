// Custom operator new[]/delete[] that track allocations.
// This file MUST be in a separate executable because replacing global
// allocation functions affects the entire program.
//
// The tracking is gated by a bool flag so it only counts allocations
// during the specific test window. A thread_local reentrancy guard
// prevents infinite recursion if std::set internally calls operator new[].

#include "alloc_tracking.hpp"

#include <cstdlib>
#include <mutex>
#include <new>
#include <set>

namespace alloc_tracking {

static std::mutex g_mutex;
static bool g_active = false;
static std::set<void*>* g_live_allocs = nullptr;
static thread_local bool g_inside_tracker = false;

Guard::Guard() {
  std::lock_guard lock(g_mutex);
  static std::set<void*> allocs;
  g_live_allocs = &allocs;
  g_live_allocs->clear();
  g_active = true;
}

Guard::~Guard() {
  std::lock_guard lock(g_mutex);
  g_active = false;
}

size_t Guard::leaks() const {
  std::lock_guard lock(g_mutex);
  return g_live_allocs->size();
}

static void* record_alloc(void* p) {
  if (g_inside_tracker) return p;
  std::lock_guard lock(g_mutex);
  if (g_active && g_live_allocs) {
    g_inside_tracker = true;
    g_live_allocs->insert(p);
    g_inside_tracker = false;
  }

  return p;
}

static void record_free(void* p) {
  if (p == nullptr) return;
  // We need to free before erase: if std::set::erase internally calls
  // operator new[], reusing the same address would corrupt the tracker.
  // After free, p is used only as a numeric key in erase (not dereferenced).
  // GCC warns about this (-Wuse-after-free), Clang/MSVC do not.
  std::free(p);
  if (g_inside_tracker) return;
  std::lock_guard lock(g_mutex);
  if (g_live_allocs) {
    g_inside_tracker = true;
#if defined(__GNUC__) && !defined(__clang__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wuse-after-free"
#endif
    g_live_allocs->erase(p);
#if defined(__GNUC__) && !defined(__clang__)
#  pragma GCC diagnostic pop
#endif
    g_inside_tracker = false;
  }
}

}  // namespace alloc_tracking

// -- Global operator replacements --
// These replace the default new[]/delete[] for the entire executable.
// The tracking flag ensures we only count allocations during test windows.

void* operator new[](std::size_t sz) {
  void* p = std::malloc(sz ? sz : 1);
  if (!p) throw std::bad_alloc();
  return alloc_tracking::record_alloc(p);
}

void operator delete[](void* p) noexcept {
  alloc_tracking::record_free(p);
}

void operator delete[](void* p, std::size_t) noexcept {
  alloc_tracking::record_free(p);
}
