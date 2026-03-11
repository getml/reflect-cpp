#pragma once

// Shared helper types for regression tests that need move/copy tracking.

namespace test_helpers {

// Tracks copy vs move operations.
struct MoveTracker {
  int copies = 0;
  int moves = 0;

  MoveTracker() = default;
  MoveTracker(const MoveTracker& other)
      : copies(other.copies + 1), moves(other.moves) {}
  MoveTracker(MoveTracker&& other) noexcept
      : copies(other.copies), moves(other.moves + 1) {}
  MoveTracker& operator=(const MoveTracker&) = default;
  MoveTracker& operator=(MoveTracker&&) noexcept = default;
};

// Derived type convertible to MoveTracker via slicing.
struct DerivedTracker : MoveTracker {
  DerivedTracker() = default;
  DerivedTracker(const DerivedTracker& other) : MoveTracker(other) {}
  DerivedTracker(DerivedTracker&& other) noexcept
      : MoveTracker(std::move(other)) {}
};

// Base type that records whether it was constructed from an rvalue.
struct FlatBase {
  int copies = 0;
  int moves = 0;
  bool from_rvalue = false;

  FlatBase() = default;
  FlatBase(const FlatBase& other)
      : copies(other.copies + 1), moves(other.moves) {}
  FlatBase(FlatBase&& other) noexcept
      : copies(other.copies), moves(other.moves + 1) {}
  FlatBase(const struct FlatDerived& other);
  FlatBase(struct FlatDerived&& other) noexcept;
  FlatBase& operator=(const FlatBase&) = default;
  FlatBase& operator=(FlatBase&&) noexcept = default;
};

struct FlatDerived {
  int copies = 0;
  int moves = 0;

  FlatDerived() = default;
  FlatDerived(const FlatDerived& other)
      : copies(other.copies + 1), moves(other.moves) {}
  FlatDerived(FlatDerived&& other) noexcept
      : copies(other.copies), moves(other.moves + 1) {}
};

inline FlatBase::FlatBase(const FlatDerived& other)
    : copies(other.copies), moves(other.moves), from_rvalue(false) {}
inline FlatBase::FlatBase(FlatDerived&& other) noexcept
    : copies(other.copies), moves(other.moves), from_rvalue(true) {}

// Same pattern for Skip tests (separate types to avoid ODR conflicts).
struct SkipBase {
  int copies = 0;
  int moves = 0;
  bool from_rvalue = false;

  SkipBase() = default;
  SkipBase(const SkipBase& other)
      : copies(other.copies + 1), moves(other.moves) {}
  SkipBase(SkipBase&& other) noexcept
      : copies(other.copies), moves(other.moves + 1) {}
  SkipBase(const struct SkipDerived& other);
  SkipBase(struct SkipDerived&& other) noexcept;
  SkipBase& operator=(const SkipBase&) = default;
  SkipBase& operator=(SkipBase&&) noexcept = default;
};

struct SkipDerived {
  int copies = 0;
  int moves = 0;

  SkipDerived() = default;
  SkipDerived(const SkipDerived& other)
      : copies(other.copies + 1), moves(other.moves) {}
  SkipDerived(SkipDerived&& other) noexcept
      : copies(other.copies), moves(other.moves + 1) {}
};

inline SkipBase::SkipBase(const SkipDerived& other)
    : copies(other.copies), moves(other.moves), from_rvalue(false) {}
inline SkipBase::SkipBase(SkipDerived&& other) noexcept
    : copies(other.copies), moves(other.moves), from_rvalue(true) {}

}  // namespace test_helpers
