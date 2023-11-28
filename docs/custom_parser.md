# Custom parsers

If you absolutely do not want to make any changes to your original classes whatsoever,
you can implement a custom parser using `rfl::parsing::CustomParser`.

In order to do so, you must do the following:

You must create a helper struct that *can* be parsed. The helper struct must fulfill the following
conditions:

1) It must contain a static method called `from_class` that takes your original class as an input and returns the helper struct. This method must not throw an exception.
2) (Optional) It must contain a method called `to_class` that transforms the helper struct into your original class. This method may throw an exception, if you want to. If you can directly construct your custom class from the field values in the order they were declared in the helper struct, you do not have to write a `to_class` method.

You can then implement a custom parser for your class like this:

```cpp
namespace rfl {
namespace parsing {

template <class ReaderType, class WriterType>
struct Parser<ReaderType, WriterType, YourOriginalClass>
    : public CustomParser<ReaderType, WriterType, YourOriginalClass,
                          YourHelperStruct> {};

}  // namespace parsing
}  // namespace rfl
```

## Example

Suppose your original class looks like this:

```cpp
struct Person {
    Person(const std::string& _first_name, const std::string& _last_name,
           const int _age)
        : first_name_(_first_name), last_name_(_last_name), age_(_age) {}

    const auto& first_name() const { return first_name_; }

    const auto& last_name() const { return last_name_; }

    auto age() const { return age_; }

   private:
    std::string first_name_;
    std::string last_name_;
    int age_;
};
```

You can then write a helper struct: 

```cpp
struct PersonImpl {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    int age;

    // 1) Static method that takes your original class as an input and
    //    returns the helper struct.
    //    MUST NOT THROW AN EXCEPTION!
    static PersonImpl from_class(const Person& _p) noexcept {
        return PersonImpl{.first_name = _p.first_name(),
                          .last_name = _p.last_name(),
                          .age = _p.age()};
    }

    // 2) Const method called `to_class` that transforms the helper struct
    //    into your original class.
    //    In this case, the `to_class` method is actually optional, because
    //    you can directly create Person from the field values.
    Person to_class() const { return Person(first_name(), last_name(), age); }
};
```

You then implement the custom parser:

```cpp
namespace rfl {
namespace parsing {

template <class ReaderType, class WriterType>
struct Parser<ReaderType, WriterType, Person>
    : public CustomParser<ReaderType, WriterType, Person, PersonImpl> {};

}  // namespace parsing
}  // namespace rfl
```

Now your custom class is fully supported by reflect-cpp. So for instance, you could parse it
inside a vector:

```cpp
const auto people = rfl::json::read<std::vector<Person>>(json_str).value();
```

As we have noted, in this particular example, the `Person` class can be constructed from the field values in
`PersonImpl` in the exact same order they were declared in `PersonImpl`. So we can drop the `.to_class` method:

```cpp
struct PersonImpl {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    int age;

    static PersonImpl from_class(const Person& _p) noexcept {
        return PersonImpl{.first_name = _p.first_name(),
                          .last_name = _p.last_name(),
                          .age = _p.age()};
    }
};
```
