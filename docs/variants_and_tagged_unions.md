# `std::variant` and `rfl::TaggedUnion`

## `std::variant`

Sometimes you know that the JSON object can be one of several alternatives. For example,
you might have several shapes like `Circle`, `Rectangle` or `Square`. For these kind of 
cases, the C++ standard library contains `std::variant`:

```cpp
struct Circle {
    rfl::Field<"radius", double> radius;
};

struct Rectangle {
    rfl::Field<"height", double> height;
    rfl::Field<"width", double> width;
};

struct Square {
    rfl::Field<"width", double> width;
};

using Shapes = std::variant<Circle, Rectangle, Square>;

const Shapes r = Rectangle{.height = 10, .width = 5};

const auto json_string = rfl::json::write(r);

const auto r2 = rfl::json::read<Shapes>(json_string);
```

This code will compile just fine and work as intended. However, there are
several problems with this:

1) It is in inefficient: The parser has to read the fields for all of the different alternatives until it can't find a required field in the JSON object. It will then move on to the next alternative.
2) It leads to confusing error messages: If none of the alternatives can be matched, you will get an error message telling you why each of the alternatives couldn't be matched. Such error messages are very long-winding and hard to read.
3) It is dangerous. Imagine we had written `std::variant<Circle, Square, Rectangle>` instead of `std::variant<Circle, Rectangle, Square>`. This would mean that `Rectangle` could never be matched, because the fields in `Square` are a subset of `Rectangle`. This leads to very confusing bugs.

## Tagging `std::variant` (externally tagged)

From a functional programming point-of-view, the most straightforward way to handle these problems is to add tags. 

You can do that using `rfl::Field`:

```cpp
using TaggedVariant = std::variant<rfl::Field<"option1", Type1>, rfl::Field<"option2", Type2>, ...>;
```

The parser can now figure this out and will only try to parse the field that was indicated by the field name.
Duplicate field names will lead to compile-time errors.

We can rewrite the example from above:

```cpp
// Circle, Rectangle and Square are the same as above.

using Shapes = std::variant<rfl::Field<"circle", Circle>,
                            rfl::Field<"rectangle", Rectangle>,
                            rfl::Field<"square", Square>>;

const Shapes r =
    rfl::make_field<"rectangle">(Rectangle{.height = 10, .width = 5});

const auto json_string = rfl::json::write(r);

const auto r2 = rfl::json::read<Shapes>(json_string);
```

The resulting JSON looks like this:
```json
{"rectangle":{"height":10.0,"width":5.0}}
```

Because the tag is external, this is called *externally tagged*. It is the standard in Rust's [serde-json](https://serde.rs/enum-representations.html).

## `rfl::TaggedUnion` (internally tagged)

But sometimes you also want the tag to be inside the class itself. That is why we have provided a helper class for these purposes: `rfl::TaggedUnion`.

TaggedUnions require some kind of identifying field on your struct, which must be expressed as an `rfl::Literal`. It will then try to take that field from the JSON object, match it to the correct alternative and then only parse the correct alternative.

We will now rewrite the example from above using `rfl::TaggedUnion`:

```cpp
struct Circle {
    // All alternatives must contain a field named "shape". The type
    // of the field must be an rfl::Literal. Since that Literal usually
    // only contains one string (even though it doesn't have to), you can
    // assign it to its default value.
    rfl::Field<"shape", rfl::Literal<"Circle">> shape = rfl::default_value;
    rfl::Field<"radius", double> radius;
};

struct Rectangle {
    rfl::Field<"shape", rfl::Literal<"Rectangle">> shape =
        rfl::default_value;
    rfl::Field<"height", double> height;
    rfl::Field<"width", double> width;
};

struct Square {
    rfl::Field<"shape", rfl::Literal<"Square">> shape = rfl::default_value;
    rfl::Field<"width", double> width;
};

// Now you tell rfl::TaggedUnion that you want it to look for the field "shape".
using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

const Shapes r = Rectangle{.height = 10, .width = 5};

const auto json_string = rfl::json::write(r);

const auto r2 = rfl::json::read<Shapes>(json_string);
```

The resulting JSON looks like this:
```json
{"shape":"Rectangle","height":10.0,"width":5.0}
```

Because the tag is inside the JSON object, this is called *internally tagged*. 
It is the standard in Python's [pydantic](https://docs.pydantic.dev/latest/api/standard_library_types/#union).

## The visitor pattern

In C++, the idiomatic way to handle `std::variant` and `rfl::TaggedUnion` is the [visitor pattern](https://en.cppreference.com/w/cpp/utility/variant/visit).

For instance, the externally tagged `std::variant` from the example above could be handled like this:

```cpp
using Shapes = std::variant<rfl::Field<"circle", Circle>,
                            rfl::Field<"rectangle", Rectangle>,
                            rfl::Field<"square", Square>>;

const Shapes my_shape =
    rfl::make_field<"rectangle">(Rectangle{.height = 10, .width = 5});

const auto handle_shapes = [](const auto& field) {
  using Type = typename std::decay_t(decltype(field))::Type;
  if constexpr (std::is_same<Type, Circle>()) {
     std::cout << is circle, radius: << field.value().radius() << std::endl;
  } else if constexpr (std::is_same<Type, Rectangle>()) {
     std::cout << is rectangle, width: << field.value().width() << ", height: " << field.value().height() << std::endl;
  } else if constexpr (std::is_same<Type, Square>()) {
     std::cout << is square, width: << field.value().width() << std::endl;
  } else {
    // reflect-cpp also provides this very useful helper that ensures
    // at compile-time that you didn't forget anything.
    static_assert(rfl::always_false_v<Type>, "Not all cases were covered.");
  }
};

std::visit(handle_shapes, my_shape);
```

Likewise, `rfl::TaggedUnion` is just an `std::variant` under-the-hood. The original variant can be
retrieved using `.variant()`:

```cpp
using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

const Shapes my_shape = Rectangle{.height = 10, .width = 5};

const auto handle_shapes = [](const auto& s) {
  using Type = std::decay_t(decltype(s));
  if constexpr (std::is_same<Type, Circle>()) {
     std::cout << is circle, radius: << s.radius() << std::endl;
  } else if constexpr (std::is_same<Type, Rectangle>()) {
     std::cout << is rectangle, width: << s.width() << ", height: " << s.height() << std::endl;
  } else if constexpr (std::is_same<Type, Square>()) {
     std::cout << is square, width: << s.width() << std::endl;
  } else {
    static_assert(rfl::always_false_v<Type>, "Not all cases were covered.");
  }
};

std::visit(handle_shapes, my_shape.variant());
```


