# Fix 30 bugs found during code review

## Summary

This MR fixes 30 bugs across core types, JSON, XML, TOML, MsgPack, BSON, Avro, Cap'n Proto, and CLI modules. Each bug has a regression test that fails before the fix and passes after.

**Stats:** 39 files changed, 1197 insertions, 52 deletions.

---

## Discussion: Breaking API Changes

The following 3 fixes change public API behavior. They are correct from a safety standpoint, but may break downstream code. Please review whether a deprecation period or alternative approach is preferred.

### 1. `Validator::get()`/`value()` return mutable reference

> **Breaking change:** Removes non-const overloads of `get()`, `value()`, `operator*()`, `operator()()`.
> Code like `validator.get() = new_value` will no longer compile.
>
> **Conflicts with API convention:** PR #613 established that wrapper types should provide `operator*`, `.value()`, `.get()`, and `operator()`. This fix removes the non-const variants to prevent mutation that bypasses validation. An alternative would be to keep non-const overloads but route them through re-validation (i.e. a validating setter).

**File:** `include/rfl/Validator.hpp:98-99, 114-115`

**Problem:** Non-const `get()`, `value()`, `operator*()`, `operator()()` return `T&`, allowing direct write that bypasses validation.

**Test:** `tests/generic/test_validator.cpp` — `validator_get_does_not_allow_invalid_mutation`. Uses `std::is_assignable_v` to verify `get()` returns `const T&`.

**Fix:** Removed all non-const overloads (`get()`, `value()`, `operator*()`, `operator()()`).

---

### 2. `Literal(const std::string&)` is implicit

> **Breaking change:** Adds `explicit` to the string constructor.
> Code like `rfl::Literal<"a","b"> lit = my_string;` will no longer compile; must use `rfl::Literal<"a","b">(my_string)` instead.

**File:** `include/rfl/Literal.hpp:46`

**Problem:** Non-explicit constructor allows implicit `std::string` → `Literal` conversion, which throws on invalid input. Surprising for users.

**Test:** `tests/generic/test_literal.cpp` — `literal_constructor_is_explicit`. Uses `std::is_convertible_v` to verify implicit conversion is disabled.

**Fix:** Added `explicit` keyword.

---

### 3. `TaggedUnion` — typo `discrimininator_`

> **Breaking change:** Renames the public static member `discrimininator_` → `discriminator_`.
> Code referencing `TaggedUnion<...>::discrimininator_` will no longer compile.
> A `[[deprecated]]` alias could be added to ease migration if desired.

**File:** `include/rfl/TaggedUnion.hpp:15`

**Problem:** Extra 'n' — `discrimininator_` instead of `discriminator_`.

**Test:** `tests/generic/test_tagged_union.cpp` — `tagged_union_discriminator_spelling`. Uses concepts to check member name existence.

**Fix:** Renamed to `discriminator_`. Also fixed the typo in the error message string literal `"discrimininator"` → `"discriminator"` in `Parser_tagged_union.hpp:151`.

---

## Bugs Fixed

### 4. `Tuple::operator<=>` always returns `equivalent`

**File:** `include/rfl/Tuple.hpp:107`

**Problem:** The condition `_ordering != equivalent` is checked on first iteration where `_ordering` is initialized to `equivalent` — the body never executes. The operator returns `equivalent` for any two tuples.

**Test:** `tests/generic/test_tuple.cpp` — `tuple_spaceship_not_always_equivalent`, `tuple_spaceship_less`. Verifies `Tuple<int>(1) <=> Tuple<int>(2)` returns `less`, not `equivalent`.

**Fix:** Changed `!=` to `==` so the comparison body executes when ordering is still undetermined.

---

### 5. `Result::operator=(const Result<U>&)` does not update `success_`

**File:** `include/rfl/Result.hpp:210-216`

**Problem:** The converting assignment copies `t_or_err_` but never updates `success_`. After assigning an error `Result<U>` into a value `Result<T>`, `success_` remains `true` — `.value()` reads garbage.

**Test:** `tests/generic/test_result.cpp` — `result_cross_type_assign_error_clears_success`. Assigns error `Result<long>` to value `Result<int>`, checks the target is falsy.

**Fix:** Replaced raw union copy with proper `destroy()` + `success_` update + `move_from_other()`.

---

### 6. `Validator::operator=(U&&)` is `noexcept` but throws

**File:** `include/rfl/Validator.hpp:77`

**Problem:** The operator calls `.value()` which throws `std::runtime_error` on validation failure. Marked `noexcept` — any validation error calls `std::terminate`.

**Test:** `tests/generic/test_validator.cpp` — `validator_assign_rvalue_operator_is_noexcept_but_can_throw`. Uses `std::is_nothrow_assignable_v` to verify the operator is no longer `noexcept`.

**Fix:** Removed `noexcept` specifier.

---

### 7. `Generic::to_int()` silently truncates `int64_t` to `int`

**File:** `include/rfl/Generic.hpp:175-188`

**Problem:** `static_cast<int>(_v)` with no range check. Values outside `[INT_MIN, INT_MAX]` are silently truncated.

**Test:** `tests/generic/test_generic.cpp` — `generic_to_int_rejects_overflow`, `generic_to_int_rejects_underflow`. Passes `INT_MAX+1` and `INT_MIN-1`, expects failure.

**Fix:** Added range check before the cast, returning an error for out-of-range values.

---

### 8. `Box`/`Ref` `operator<=>` compares pointer addresses

**Files:** `include/rfl/Box.hpp:130`, `include/rfl/Ref.hpp:118`

**Problem:** `_b1.ptr() <=> _b2.ptr()` compares memory addresses instead of pointed-to values. Two `Box<int>(42)` with different allocations compare as unequal.

**Test:** `tests/generic/test_box.cpp` — `box_spaceship_compares_values_not_pointers`. `tests/generic/test_ref.cpp` — `ref_spaceship_compares_values_not_pointers`.

**Fix:** Changed to `*_b1 <=> *_b2` (dereference before compare).

---

### 9. `Field`/`Flatten`/`Skip` cross-type move constructor copies instead of moving

**Files:** `include/rfl/Field.hpp:37`, `include/rfl/Flatten.hpp:32`, `include/rfl/internal/Skip.hpp:43`

**Problem:** `value_(_field.get())` — `get()` returns lvalue reference, so the copy constructor is called instead of move.

**Test:** `tests/generic/test_field.cpp` — `field_cross_type_move_does_not_copy`. `tests/generic/test_flatten.cpp` — `flatten_cross_type_move_does_not_copy`. `tests/generic/test_skip.cpp` — `skip_cross_type_move_does_not_copy`. Use `MoveTracker` types that count copies vs moves.

**Fix:** Changed to `value_(std::move(_field.get()))` in all three types.

---

### 10. `Result::value_or(T&&)` returns dangling rvalue reference

**File:** `include/rfl/Result.hpp:295`

**Problem:** Returns `T&&` instead of `T`. If the default argument is a temporary, the caller gets a dangling reference.

**Test:** `tests/generic/test_result.cpp` — `result_value_or_returns_value_not_rvalue_ref`. Checks return type is `T`, not `T&&`.

**Fix:** Changed return type from `T&&` to `T`.

---

### 11. `Result::error() &&` returns `Error&` instead of `Error&&`

**File:** `include/rfl/Result.hpp:335`

**Problem:** The rvalue-qualified overload returns `Error&` but `get_err() &&` returns `Error&&`. Prevents move semantics on rvalue Results.

**Test:** `tests/generic/test_result.cpp` — `result_error_rvalue_qualified_returns_rvalue_ref`. Checks return type is `Error&&`.

**Fix:** Changed return type from `Error&` to `Error&&`.

---

### 12. `Object::difference_type` is unsigned

**File:** `include/rfl/Object.hpp:29`

**Problem:** `using difference_type = typename DataType::size_type` — `size_type` is unsigned, but the C++ standard requires `difference_type` to be signed.

**Test:** `tests/generic/test_object.cpp` — `object_difference_type_is_signed`. Uses `std::is_signed_v`.

**Fix:** Changed to `typename DataType::difference_type`.

---

### 13. `transform_camel_case` prepends `_` to names starting with uppercase

**File:** `include/rfl/internal/transform_case.hpp:60-73`

**Problem:** When `_i == 0` and the first character is uppercase, the function unconditionally prepends `'_'`. `"MyField"` becomes `"_my_field"` instead of `"my_field"`.

**Test:** `tests/json/test_regression_bugs.cpp` — `camel_case_to_snake_case_no_leading_underscore`. Serializes a struct with `CamelCaseToSnakeCase` and checks output.

**Fix:** Added `sizeof...(chars) > 0` guard so `'_'` is only inserted between characters, not at the beginning.

---

### 14. `Timestamp::to_time_t()` double-subtracts timezone offset

**File:** `include/rfl/Timestamp.hpp:120`

**Problem:** `timegm(&tm) - tm_.tm_gmtoff` — `timegm()` already returns UTC. Subtracting `tm_gmtoff` applies the timezone correction twice. Non-Windows only.

**Test:** `tests/json/test_regression_bugs.cpp` — `timestamp_to_time_t_no_double_timezone_correction`. Sets `tm_gmtoff = 3600` and verifies `to_time_t()` returns the correct UTC epoch.

**Fix:** Removed `- tm_.tm_gmtoff`.

---

### 15. `FieldVariantParser` — wrong error message

**File:** `include/rfl/parsing/FieldVariantParser.hpp:47-50`

**Problem:** When zero matching fields are found, the error says "found more than one" instead of "found none".

**Test:** `tests/json/test_regression_bugs.cpp` — `field_variant_empty_object_error_message`. Parses `"{}"` and checks the error message does not contain "more than one".

**Fix:** Changed message to "found none".

---

### 16. `json::Writer` reads past `string_view` bounds

**File:** `src/rfl/json/Writer.cpp:68, :91, :113`

**Problem:** `yyjson_mut_strcpy(_name.data())` reads until `\0`, but `string_view::data()` is not null-terminated. Reads past buffer for substrings.

**Test:** `tests/json/test_regression_bugs.cpp` — `json_writer_handles_non_null_terminated_field_names`. Creates a `string_view` substring and verifies the JSON key matches the view's length.

**Fix:** Changed all three call sites from `yyjson_mut_strcpy` to `yyjson_mut_strncpy` with explicit size.

---

### 17. `cli::Reader` — `stoull` accepts negative numbers for unsigned types

**File:** `include/rfl/cli/Reader.hpp:77`

**Problem:** `std::stoull("-1")` wraps to `ULLONG_MAX` without error. `static_cast<uint16_t>` wraps again to 65535.

**Test:** `tests/cli/test_regression_bugs.cpp` — `cli_rejects_negative_for_unsigned`. Passes `--port=-1` and expects failure.

**Fix:** Added explicit check for leading `-` character, returning an error for negative input.

---

### 18. `cli::Reader` — no range check on unsigned narrowing cast

**File:** `include/rfl/cli/Reader.hpp:77`

**Problem:** `static_cast<T>(stoull(str))` silently truncates. `stoull("99999")` for `uint16_t` produces 34463.

**Test:** `tests/cli/test_regression_bugs.cpp` — `cli_rejects_out_of_range_for_narrow_type`. Passes `--port=99999` for `uint16_t` and expects failure.

**Fix:** Added `std::numeric_limits<T>::max()` bounds check before the cast.

---

### 19. `cli::Reader` — no range check on signed narrowing cast

**File:** `include/rfl/cli/Reader.hpp:101-102`

**Problem:** `static_cast<T>(stoll(str))` silently truncates for `int8_t`/`int16_t`/`int32_t`. `stoll("200")` cast to `int8_t` wraps to -56.

**Test:** `tests/cli/test_regression_bugs.cpp` — `cli_rejects_out_of_range_for_signed_narrow_type`, `cli_rejects_large_negative_for_signed_narrow_type`. Passes `--level=200` and `--level=-200` for `int8_t` and expects failure.

**Fix:** Added `std::numeric_limits<T>::min()`/`max()` bounds check before the cast.

---

### 20. `cli::Reader` — `std::stod` is locale-dependent

**File:** `include/rfl/cli/Reader.hpp:64`

**Problem:** `std::stod` uses the C locale. In locales with comma decimal separator (e.g. `de_DE`), `"3.14"` parses as `3.0`.

**Test:** `tests/cli/test_regression_bugs.cpp` — `cli_float_parsing_ignores_locale`. Sets `de_DE` locale, parses `--rate=3.14`, checks result is `3.14`.

**Fix:** Replaced `std::stod` with `std::from_chars` which is locale-independent.

---

### 21. `capnproto::read(istream, schema)` — infinite recursion

**File:** `include/rfl/capnproto/read.hpp:79-82`

**Problem:** The overload `read(std::istream&, const Schema<T>&)` calls `read<T, Ps...>(_stream, _schema)` — resolves to itself. Stack overflow.

**Test:** `tests/capnproto/compile_fail/capnproto_read_istream_schema.cpp` — instantiates the overload. Before the fix, GCC rejects it as "use of auto before deduction" (infinite recursion detected at compile time). After the fix, the file compiles successfully, verifying the overload resolves correctly.

**Fix:** Read bytes from stream into a vector, then call `read(bytes.data(), bytes.size(), _schema)`.

---

### 22. `avro::SchemaImpl` move-assignment — double-decrement / use-after-free

**File:** `src/rfl/avro/SchemaImpl.cpp:34-42`

**Problem:** Move-assignment neither releases the old `iface_` (leak) nor nullifies `_other.iface_` (double-decrement when `_other`'s destructor runs).

**Test:** `tests/avro/test_regression_bugs.cpp` — `schema_impl_move_assignment_no_double_free`. Move-assigns two `SchemaImpl` objects, verifies source `iface_` is null after move.

**Fix:** Added `avro_value_iface_decref`/`avro_schema_decref` for the old iface before reassignment, and `_other.iface_ = nullptr` after.

---

### 23. `xml::Reader` uses `stoi` for all integer types

**File:** `include/rfl/xml/Reader.hpp:99-105`

**Problem:** `std::stoi` returns `int` (32-bit). For `int64_t`/`uint64_t` fields, values beyond `INT_MAX` throw `out_of_range` or are silently truncated. Additionally, `static_cast<T>` after `stoll`/`stoull` silently truncates for narrow types like `int16_t`/`uint16_t`.

**Test:** `tests/xml/test_regression_bugs.cpp` — `read_int64_above_int32_max`, `read_int64_large_negative`, `read_uint64_large_value`, `read_rejects_out_of_range_for_int16`, `read_rejects_negative_for_uint16`, `read_rejects_large_negative_for_int16`.

**Fix:** Replaced with `std::from_chars` which parses directly into the target type, handling overflow, negative-for-unsigned, and narrowing without manual checks.

---

### 24. `toml::Writer` — `at_path()` dereferences as path, crashes on dotted keys

**File:** `src/rfl/toml/Writer.cpp:26, :40`

**Problem:** After `emplace("a.b", ...)`, `at_path("a.b")` interprets the dot as a path separator and looks for `table["a"]["b"]` which doesn't exist — returns invalid node — `as_table()`/`as_array()` returns nullptr.

**Test:** `tests/toml/test_regression_bugs.cpp` — `field_name_with_dot_does_not_crash`, `array_field_name_with_dot_does_not_crash`. Use `rfl::Rename<"a.b", ...>` to create dotted field names.

**Fix:** Replaced `at_path(_name)` with `operator[](_name)` which does literal key lookup.

---

### 25. `msgpack::read` ignores `msgpack_unpack` return value

**File:** `include/rfl/msgpack/read.hpp:35-36`

**Problem:** If `msgpack_unpack` fails, the return value is not checked. `deserialized` remains uninitialized — garbage is parsed, producing misleading downstream errors like "Could not cast to a map".

**Test:** `tests/msgpack/test_regression_bugs.cpp` — `read_single_invalid_byte_returns_meaningful_error`, `read_truncated_data_returns_meaningful_error`. Pass invalid/truncated data and verify the error doesn't mention "cast".

**Fix:** Check return value; return `"Failed to unpack msgpack data."` error on failure. Accept both `MSGPACK_UNPACK_SUCCESS` and `MSGPACK_UNPACK_EXTRA_BYTES`.

---

### 26. `msgpack::Writer` stores `uint64_t` as signed `int64`

**File:** `include/rfl/msgpack/Writer.hpp:133-134`

**Problem:** All integers are packed via `msgpack_pack_int64(static_cast<int64_t>(...))`. For `uint64_t` values above `INT64_MAX`, the cast wraps to negative — msgpack stores them as `NEGATIVE_INTEGER`.

**Test:** `tests/msgpack/test_regression_bugs.cpp` — `uint64_above_int64_max_stored_as_positive`, `uint64_max_stored_as_positive`. Verify raw msgpack wire type is `POSITIVE_INTEGER`.

**Fix:** Added a separate `std::is_unsigned` branch that calls `msgpack_pack_uint64`.

---

### 27. `bson::Reader` rejects integer BSON values for float fields

**File:** `include/rfl/bson/Reader.hpp:127-132`

**Problem:** The float branch checks `btype != BSON_TYPE_DOUBLE` and rejects everything else — even though the error message claims `int32`/`int64`/`date_time` are valid.

**Test:** `tests/bson/test_regression_bugs.cpp` — `read_int64_into_double_field`, `read_large_int64_into_double_field`. Write an int struct, read it back as a double struct.

**Fix:** Changed to a `switch` accepting `BSON_TYPE_DOUBLE`, `INT32`, `INT64`, and `DATE_TIME`.

---

### 28. `delete`/`delete[]` mismatch in test

**File:** `tests/json/test_pointer_fields.cpp`

**Problem:** A raw pointer field allocated with `new[]` was freed with `delete` instead of `delete[]`.

**Test:** Pre-existing test; fix prevents undefined behavior under sanitizers.

**Fix:** Changed `delete` to `delete[]`.

---

### 29. `Flatten(U&&)` universal reference constructor copies instead of forwarding

**File:** `include/rfl/Flatten.hpp:40`

**Problem:** `value_(_value)` — `_value` is a named parameter (lvalue), so the copy constructor is always called even when an rvalue is passed. Should be `value_(std::forward<U>(_value))`.

**Test:** `tests/generic/test_flatten.cpp` — `flatten_universal_ref_ctor_forwards_rvalue`. Constructs `Flatten<FlatBase>` from `FlatDerived&&` and verifies rvalue conversion is used.

**Fix:** Changed to `value_(std::forward<U>(_value))`.

---

### 30. `Flatten::operator=(Flatten<U>&&)` forwards `Flatten<U>` instead of `U`

**File:** `include/rfl/Flatten.hpp:103-105`

**Problem:** `std::forward<U>(_f)` forwards the `Flatten<U>` object, not `U`. Assignment `value_ = Flatten<U>` looks for implicit conversion which uses `get()` (lvalue ref) — copy instead of move. Causes hard compile error for types without `Flatten<U>` → `Type` conversion.

**Test:** `tests/generic/compile_fail/flatten_cross_type_move_assign.cpp` — compile test that instantiates the cross-type move assignment. Verifies compilation succeeds after the fix.

**Fix:** Changed to `value_ = std::move(_f.get())`.
