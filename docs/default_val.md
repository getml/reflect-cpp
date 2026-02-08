# Default values (rfl::DefaultVal)

The `rfl::DefaultVal<T>` wrapper allows a struct field to have a predefined default value when serializing and deserializing. When a field is declared as `rfl::DefaultVal<T>`, the library will accept input that omits that field and will populate it with the provided default (or with a default-constructed T when no explicit default is given).

## Declaration and initialization

You can declare a default-valued field like this:

```cpp
struct Person {
  std::string first_name;                             // required
  rfl::DefaultVal<std::string> last_name = "Simpson"; // has explicit default
  rfl::DefaultVal<std::string> town;                  // default-constructed (empty string)
};
```

DefaultVal behaves like a thin wrapper around the underlying type. You can construct and assign it from the underlying type, from other DefaultVal instances (if convertible), or assign the special token `rfl::Default` to reset it to the default-constructed value (if the type is default-constructible):

```cpp
Person p;
p.last_name = "Smith";            // assign underlying value
p.town = rfl::Default{};            // reset to default (empty string)
std::string s = p.last_name.value();
```

API convenience:

- .get(), .value(), operator()(), operator*() — access the underlying value (const and non-const overloads).
- set(...) — assign underlying value.

## JSON behaviour

When writing JSON, fields that are DefaultVal are written like normal fields using their current underlying value. When reading JSON, omitted DefaultVal fields are filled with the default value (the value assigned in the declaration, or the type's default-constructed value).

Example (object fields):

```cpp
// Person from above
const auto homer = rfl::json::read<Person>(R"({"first_name":"Homer"})").value();
// homer.last_name == "Simpson" (declared default)
// homer.town == "" (default-constructed)
```

Example (no field names / positional arrays):

DefaultVal also works when using rfl::NoFieldNames (positional JSON arrays). Omitted positions that correspond to DefaultVal fields get their default values:

```cpp
const auto homer = rfl::json::read<Person, rfl::NoFieldNames>(R"(["Homer"])").value();
// homer.first_name == "Homer"
// homer.last_name == "Simpson"
// homer.town == ""
```

## When to use

Use `rfl::DefaultVal` when you want a field to be optional at the input side but still available as a value on the resulting object (no std::optional or pointer indirection). It is particularly useful for fields with sensible defaults (for example, a common last name, a default configuration value, or empty containers/strings).

## Notes

- The underlying type must be default-constructible to allow resetting via `rfl::Default` or when no explicit default is supplied.
- DefaultVal preserves normal read/write semantics; other fields that are not DefaultVal remain required unless expressed as optionals or handled by processors (e.g., `rfl::DefaultIfMissing`).

For more advanced control over when fields are considered missing and how defaults are applied, see the processors documentation (e.g., `rfl::DefaultIfMissing`).
