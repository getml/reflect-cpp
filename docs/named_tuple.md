# `rfl::NamedTuple`

`rfl::NamedTuple` is very similar to `std::tuple`, but unlike `std::tuple` the fields have names.

In other words, consider the following struct:

```cpp
struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">> birthday;
};
```

You might as well define the following `rfl::NamedTuple`:

```cpp
using Person = rfl::NamedTuple<
    rfl::Field<"firstName", std::string>,
    rfl::Field<"lastName", std::string>,
    rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">>>;
```

From the point-of-view of serialization/deserialization, the two definitions are equivalent. 
The resulting JSON strings (or any other format) will be the same.


## Structural typing

From the point-of-view of programming, there is an important difference: Structs are *nominally typed*
and named tuples are *structurally typed* (confusingly, structs are not structurally typed).

In plain language, that means that the compiler will regard this as absolutely equivalent to `Person`, the named tuple:

```cpp
using Person2 = rfl::NamedTuple<
    rfl::Field<"firstName", std::string>,
    rfl::Field<"lastName", std::string>,
    rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">>>;
```

However, this will be seen as a type that is different from `Person`, the struct:

```cpp
struct Person2 {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">> birthday;
};
```

Structural typing also means that you can declare new types on-the-fly. For instance, in order
to create a `Person` named tuple, you don't actually have to declare it at all. The following will do:

```cpp
const auto person = rfl::Field<"firstName", std::string>("Homer") *
                    rfl::Field<"firstName", std::string>("Simpson") *
                    rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">>("1987-04-19");
```

The type of `person` will now be equivalent to the definition of `Person`, the named tuple, 
regardless of whether you have actually declared it anywhere.

On the other hand, structural typing also means that recursive definitions are impossible.
For instance, consider something like this:

```cpp
struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"children", std::vector<Person>> children;
};
```

In this example, `Person` is recursively defined (because of the field `children`). 
This is impossible to accomplish using structural typing and `rfl::NamedTuple`, just like it is impossible to have a recursively defined lambda function.
