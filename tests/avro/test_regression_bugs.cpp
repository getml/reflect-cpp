#include <gtest/gtest.h>

#include <string>
#include <utility>

#include <rfl.hpp>
#include <rfl/avro.hpp>
#include <rfl/avro/SchemaImpl.hpp>

// avro::SchemaImpl move-assignment — double-decrement / use-after-free
// File: src/rfl/avro/SchemaImpl.cpp:34-42
// The move assignment operator copies iface_ from _other but does NOT:
// 1) Release the old this->iface_ (leak)
// 2) Set _other.iface_ = nullptr (double-decrement when _other is destroyed)
// The move constructor (line 27-32) correctly nullifies _other.iface_.
namespace test_avro_schema_move_assignment {

// Minimal valid Avro schema JSON
static const char* const kSchema1 =
    R"({"type":"record","name":"A","fields":[{"name":"x","type":"int"}]})";
static const char* const kSchema2 =
    R"({"type":"record","name":"B","fields":[{"name":"y","type":"string"}]})";

TEST(avro_regression, schema_impl_move_assignment_no_double_free) {
  // Create two SchemaImpl objects directly. Each holds an avro_value_iface_t*.
  auto impl1 = rfl::avro::SchemaImpl(kSchema1);
  auto impl2 = rfl::avro::SchemaImpl(kSchema2);

  ASSERT_NE(impl1.iface(), nullptr);
  ASSERT_NE(impl2.iface(), nullptr);

  // Move-assign impl2 into impl1.
  // Bug: impl1's old iface_ leaks, impl2.iface_ is NOT nullified.
  // When both destructors run, impl2's destructor calls decref on the
  // pointer that impl1 also holds → double-decrement / use-after-free.
  impl1 = std::move(impl2);

  EXPECT_NE(impl1.iface(), nullptr)
      << "impl1 should have impl2's iface after move assignment";

  // After a correct move, impl2.iface() should be nullptr.
  // With the bug, impl2.iface() still points to the moved-from pointer.
  EXPECT_EQ(impl2.iface(), nullptr)
      << "After move-assignment, source iface_ should be nullptr. "
         "Bug: _other.iface_ is not nullified in operator=(SchemaImpl&&), "
         "causing double-decrement when both destructors run";

  // Both impl1 and impl2 go out of scope here.
  // With the bug: double avro_value_iface_decref on same pointer → crash/UB.
}

}  // namespace test_avro_schema_move_assignment
