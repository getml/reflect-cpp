# Atomic variables (`std::atomic` and `std::atomic_flag`)

reflect-cpp supports serializing and deserializing atomic types. The library treats atomic wrappers as containers around an underlying value and provides helpers to read plain (non-atomic) representations from input and to set atomic fields afterwards.

## Supported atomic types

- `std::atomic<T>`
- `std::atomic_flag` (serialized as a boolean)
- Arrays of atomic types (std::array<T, N> and C-style arrays)
- Aggregate types (structs/NamedTuple) containing atomic fields — each atomic field is handled independently

## Example (writing)

```cpp
struct Stats {
  std::atomic<std::uint64_t> bytes_downloaded;
  std::atomic<bool> finished;
  std::atomic_flag atomic_flag;
};

Stats stats{.bytes_downloaded = 123456789, .finished = true, .atomic_flag = ATOMIC_FLAG_INIT};
const auto json_str = rfl::json::write(stats);
// -> {"bytes_downloaded":123456789,"finished":true,"atomic_flag":false}
```

Note: the exact boolean value for `atomic_flag` depends on whether it is set or cleared.

## Example (reading)

Reading atomic variables is not quite trivial, because atomic fields cannot be copied or moved. Consider the following example:

```cpp
// const auto res = rfl::json::read<Stats>(json_str); 
// This will NOT compile because std::atomic<T> is neither copyable nor movable
```

There are two ways around this problem:

### 1. Wrap in `rfl::Ref`, `rfl::Box`, `std::shared_ptr` or `std::unique_ptr`

The easiest way to read structs with atomic fields is to wrap them in a pointer-like type such as `rfl::Ref`, `rfl::Box`, `std::shared_ptr` or `std::unique_ptr`. This works because the pointer-like types themselves are copyable/movable, even if the underlying type is not.

```cpp
const auto res = rfl::json::read<rfl::Ref<Stats>>(json_str);
```

### 2. Read into a non-atomic representation and then set atomic fields

The second way is to read into a non-atomic representation of the struct and then set the atomic fields afterwards using `rfl::atomic::set_atomic_fields`. The non-atomic representation can be obtained using `rfl::atomic::remove_atomic_t`.

```cpp
Stats stats;

const rfl::Result<rfl::Nothing> res =
    rfl::json::read<rfl::atomic::remove_atomic_t<Stats>>(json_str)
        .transform([&](auto&& non_atomic_stats) {
            return rfl::atomic::set_atomic_fields(non_atomic_stats, &stats);
        });

if (!res) {
    // handle error
    std::cerr << "Error reading JSON: " << res.error().what() << std::endl;
} 
```

## Limitations and notes

- Structs containing atomic fields must be default-constructible.
- Atomic types cannot be mixed with `rfl::DefaultVal` or the `rfl::DefaultIfMissing` processor; attempting to do so triggers a static assertion at compile-time (see parser implementations).
- The semantics used for setting atomic values use relaxed memory order (`std::memory_order_relaxed`).
- For complex aggregates, `rfl::atomic` will recurse into nested fields and arrays to set atomic members.

