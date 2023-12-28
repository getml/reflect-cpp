# `rfl::to_view`

`rfl::to_view` allows you to create views on structs using which you can access an modify the fields of the structs just like a tuple.

Under-the-hood, a view is a `rfl::NamedTuple` containing pointers to the original fields.

For example:

```cpp
auto lisa = Person{.first_name = "Lisa", .last_name = "Simpson", .age = 8};

const auto view = rfl::to_view(lisa);

// Assigns the first field, thus modifying the struct 'lisa'.
*view.get<0>() = "Maggie";

// view.values() is a std::tuple containing
// pointers to the original fields.
*std::get<1>(view.values()) = "Simpson";

// You can also access fields by their name.
// The correctness will be ensured at compile time.
*view.get<"age">() = 0;

// You can also access fields like this.
*rfl::get<0>(view) = "Maggie";

// Or like this.
*rfl::get<"first_name">(view) = "Maggie";
```