# csv

For CSV support, include the header `<rfl/csv.hpp>` and link to the [Apache Arrow](https://arrow.apache.org/) library.
Furthermore, when compiling reflect-cpp, you need to pass `-DREFLECTCPP_CSV=ON` to cmake.

CSV is a tabular text format. Like other tabular formats in reflect-cpp, CSV is designed for collections of flat records and has limitations for nested or variant types.

## Reading and writing

Suppose you have a struct like this:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;
    unsigned int age;
    rfl::Email email;
};
```

Important: CSV is a tabular format that requires collections of records. You cannot serialize individual structs - you must use containers like `std::vector<Person>`, `std::deque<Person>`, etc.

Write a collection to a string (CSV bytes) like this:

```cpp
const auto people = std::vector<Person>{
    Person{.first_name = "Bart", .birthday = "1987-04-19", .age = 10, .email = "bart@simpson.com"},
    Person{.first_name = "Lisa", .birthday = "1987-04-19", .age = 8, .email = "lisa@simpson.com"}
};

const std::string csv_text = rfl::csv::write(people);
```

Parse from a string or bytes view:

```cpp
const rfl::Result<std::vector<Person>> result = rfl::csv::read<std::vector<Person>>(csv_text);
```

## Settings

CSV behavior can be configured using `rfl::csv::Settings`:

```cpp
const auto settings = rfl::csv::Settings{}
    .with_delimiter(';')
    .with_quoting(true)
    .with_quote_char('"')
    .with_null_string("n/a")
    .with_double_quote(true)
    .with_escaping(false)
    .with_escape_char('\\')
    .with_newlines_in_values(false)
    .with_ignore_empty_lines(true)
    .with_batch_size(1024);

const std::string csv_text = rfl::csv::write(people, settings);
```

Key options:
- `batch_size` - Maximum number of rows processed per batch (performance tuning)
- `delimiter` - Field delimiter character
- `quoting` - Whether to use quoting when writing
- `quote_char` - Quote character used when reading
- `null_string` - String representation for null values
- `double_quote` - Whether a quote inside a value is double-quoted (reading)
- `escaping` - Whether escaping is used (reading)
- `escape_char` - Escape character (reading)
- `newlines_in_values` - Whether CR/LF are allowed inside values (reading)
- `ignore_empty_lines` - Whether empty lines are ignored (reading)

## Loading and saving

You can load from and save to disk:

```cpp
const rfl::Result<std::vector<Person>> result = rfl::csv::load<std::vector<Person>>("/path/to/file.csv");

const auto people = std::vector<Person>{...};
rfl::csv::save("/path/to/file.csv", people);
```

With custom settings:

```cpp
const auto settings = rfl::csv::Settings{}.with_delimiter(';');
rfl::csv::save("/path/to/file.csv", people, settings);
```

## Reading from and writing into streams

You can read from any `std::istream` and write to any `std::ostream`:

```cpp
const rfl::Result<std::vector<Person>> result = rfl::csv::read<std::vector<Person>>(my_istream);

const auto people = std::vector<Person>{...};
rfl::csv::write(people, my_ostream);
```

With custom settings:

```cpp
const auto settings = rfl::csv::Settings{}.with_delimiter(';');
rfl::csv::write(people, my_ostream, settings);
```

## Field name transformations

Like other formats, CSV supports field name transformations via processors, e.g. `SnakeCaseToCamelCase`:

```cpp
const auto people = std::vector<Person>{...};
const auto result = rfl::csv::read<std::vector<Person>, rfl::SnakeCaseToCamelCase>(csv_text);
```

## Enums and validation

CSV supports enums and validated types. Enums are written/read as strings:

```cpp
enum class FirstName { Bart, Lisa, Maggie, Homer };

struct Person {
    rfl::Rename<"firstName", FirstName> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;
    rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>> age;
    rfl::Email email;
};
```

## Limitations of tabular formats

CSV, like other tabular formats, has limitations compared to hierarchical formats such as JSON or XML:

### Collections requirement
You must serialize collections, not individual objects:
```cpp
std::vector<Person> people = {...};  // ✅ Correct
Person person = {...};               // ❌ Wrong - must be in a container
```

### No nested objects
Each field must be a primitive type, enum, or a simple validated type. Nested objects are not automatically flattened:
```cpp
// This would NOT work as expected - nested objects are not automatically flattened
struct Address {
    std::string street;
    std::string city;
};

struct Person {
    std::string first_name;
    std::string last_name;
    Address address;  // ❌ Will cause compilation errors for CSV
};
```

### Using rfl::Flatten for nested objects
If you need to include nested objects, use `rfl::Flatten` to explicitly flatten them:
```cpp
struct Address {
    std::string street;
    std::string city;
};

struct Person {
    std::string first_name;
    std::string last_name;
    rfl::Flatten<Address> address;  // ✅ This will flatten the Address fields
};

// The resulting CSV will have columns: first_name, last_name, street, city
```

### No variant types
Variant types like `std::variant`, `rfl::Variant`, or `rfl::TaggedUnion` cannot be serialized to CSV as separate columns:
```cpp
// ❌ This will NOT work
struct Person {
    std::string first_name;
    std::variant<std::string, int> status;  // Variant - not supported
    rfl::Variant<std::string, int> type;    // rfl::Variant - not supported
    rfl::TaggedUnion<"type", std::string, int> category;  // TaggedUnion - not supported
};
```

### No arrays (except bytestrings)
CSV output here does not support arrays (lists) of values in a single column. The only array-like field supported is binary data represented as bytestrings:
```cpp
// ❌ This will NOT work
struct Person {
    std::string first_name;
    std::vector<std::string> hobbies;  // Array of strings - not supported
    std::vector<int> scores;           // Array of integers - not supported
    std::vector<Address> addresses;    // Array of objects - not supported
};

// ✅ This works
struct Blob {
    std::vector<char> binary_data;      // Binary data supported as bytestring
};
```

### Use cases
CSV is ideal for:
- Data exchange and interoperability
- Simple, flat data structures with consistent types
- Human-readable datasets

CSV is less suitable for:
- Complex nested data structures
- Data with arrays or variant types
- Strict schemas with evolving types
- Very large datasets where binary columnar formats are preferred

