# `rfl::Commented`

The `rfl::Commented<T>` wrapper allows you to add comments to fields in your structs. These comments are then serialized in formats that support them, such as YAML and XML.

Note that `rfl::Commented` is currently unsupported by formats that do not have a standard way of representing comments, such as JSON.

## Example (YAML)

In YAML, the comments are added as line comments (`#`):

```cpp
struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Commented<std::string> town;
};

const auto homer = Person{.first_name = "Homer",
                          .last_name = "Simpson",
                          .town = rfl::Commented<std::string>(
                              "Springfield", "The town where Homer lives")};

const auto yaml_str = rfl::yaml::write(homer);
```

This will result in the following YAML:

```yaml
first_name: Homer
last_name: Simpson
town: Springfield  # The town where Homer lives
```

## Example (XML)

In XML, the comments are added as `<!-- comment -->` blocks after the field:

```cpp
struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Commented<std::string> town;
};

const auto homer = Person{.first_name = "Homer",
                          .last_name = "Simpson",
                          .town = rfl::Commented<std::string>(
                              "Springfield", "The town where Homer lives")};

const auto xml_str = rfl::xml::write(homer);
```

This will result in the following XML:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Person>
    <first_name>Homer</first_name>
    <last_name>Simpson</last_name>
    <town>Springfield</town>
    <!--The town where Homer lives-->
</Person>
```

## API convenience

`rfl::Commented` provides several ways to access and modify the underlying value and the comment:

- `.get()`, `.value()`, `operator()()` — access the underlying value (const and non-const overloads).
- `.comment()` — returns an `std::optional<std::string>` containing the comment.
- `.add_comment(std::string)` — sets or updates the comment.
- `.set(...)`, `operator=(...)` — assign the underlying value.

Example:

```cpp
Person p;
p.town = "Springfield";
p.town.add_comment("The town where Homer lives");
std::string s = p.town.value();
```
