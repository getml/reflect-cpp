# `rfl::ExtraFields` 

In some cases, you simply do not know all of the possible field names at compile time. You might know some, but you
do not know all of them.

Note that there are many ways of dealing with these kind of uncertainties, such as optional fields or tagged unions.

But in instance where the field names are absolutely not known in advance, you might want to use `rfl::ExtraFields`:

```cpp
struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  rfl::ExtraFields<rfl::Generic> extra_fields;
};

auto homer = Person{.first_name = "Homer"};

homer.extra_fields["age"] = 45;
homer.extra_fields["email"] = "homer@simpson.com";
homer.extra_fields["town"] = "Springfield";
```

Note that `rfl::ExtraFields` inherits from `rfl::Object` and therefore behaves exactly like it. Refer
to the relevant section of the documentation on `rfl::Object`.

This will result in the following JSON string:

```json
{"first_name":"Homer","last_name":"Simpson","age":45,"email":"homer@simpson.com","town":"Springfield"}
```

Likewise, when we parse the JSON string, all of the fields that can't be directly matched to
a known field will be parsed into `extra_fields`.

Note that in this particular instance, we have set the type to `rfl::Generic`. But if we know in
advance that all of the extra fields must be strings, we can set it to `std::string` as well:

```cpp
struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  rfl::ExtraFields<std::string> extra_fields;
};

auto homer = Person{.first_name = "Homer"};

homer.extra_fields["email"] = "homer@simpson.com";
homer.extra_fields["town"] = "Springfield";
```

This will result in the following JSON string:

```json
{"first_name":"Homer","last_name":"Simpson","email":"homer@simpson.com","town":"Springfield"}
```

If we add an extra field that is not a string (like `"age":45`), this will result in a parsing error. 
