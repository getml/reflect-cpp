# Parquet 

For Parquet support, you must also include the header `<rfl/parquet.hpp>` and link to the [Apache Arrow](https://arrow.apache.org/) and [Apache Parquet](https://parquet.apache.org/) libraries.
Furthermore, when compiling reflect-cpp, you need to pass `-DREFLECTCPP_PARQUET=ON` to cmake.

Parquet is a columnar storage format optimized for analytical workloads. Unlike most other formats supported by reflect-cpp, Parquet is designed for tabular data and has specific limitations regarding nested structures.

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

**Important**: Parquet is a tabular format that requires collections of records. You cannot serialize individual structs - you must use containers like `std::vector<Person>`, `std::deque<Person>`, etc.

A collection of `Person` structs can be serialized to a bytes vector like this:

```cpp
const auto people = std::vector<Person>{
    Person{.first_name = "Bart", .birthday = "1987-04-19", .age = 10, .email = "bart@simpson.com"},
    Person{.first_name = "Lisa", .birthday = "1987-04-19", .age = 8, .email = "lisa@simpson.com"}
};
const std::vector<char> bytes = rfl::parquet::write(people);
```

You can parse bytes like this:

```cpp
const rfl::Result<std::vector<Person>> result = rfl::parquet::read<std::vector<Person>>(bytes);
```

## Settings and compression

Parquet supports various compression algorithms and chunk sizes. You can configure these using the `Settings` struct:

```cpp
const auto settings = rfl::parquet::Settings{}
    .with_compression(rfl::parquet::Compression::GZIP)
    .with_chunksize(1000);

const std::vector<char> bytes = rfl::parquet::write(people, settings);
```

Available compression options include:

- `UNCOMPRESSED` - No compression, fastest read/write but largest file size
- `SNAPPY` (default) - Fast compression/decompression, good balance of speed and size
- `GZIP` - Good compression ratio, slower than Snappy but better compression
- `BROTLI` - Good compression for text data, optimized for web content
- `ZSTD` - Excellent compression ratio, modern algorithm with good speed
- `LZ4` - Very fast compression/decompression, lower compression ratio
- `LZ4_FRAME` - LZ4 with frame format, better compatibility
- `LZO` - Fast compression, older algorithm
- `BZ2` - High compression ratio, slower compression/decompression
- `LZ4_HADOOP` - LZ4 optimized for Hadoop ecosystem

```cpp
// Examples of different compression settings
const auto snappy_settings = rfl::parquet::Settings{}
    .with_compression(rfl::parquet::Compression::SNAPPY);

const auto gzip_settings = rfl::parquet::Settings{}
    .with_compression(rfl::parquet::Compression::GZIP);

const auto zstd_settings = rfl::parquet::Settings{}
    .with_compression(rfl::parquet::Compression::ZSTD);

const auto uncompressed_settings = rfl::parquet::Settings{}
    .with_compression(rfl::parquet::Compression::UNCOMPRESSED);
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<std::vector<Person>> result = rfl::parquet::load<std::vector<Person>>("/path/to/file.parquet");

const auto people = std::vector<Person>{...};
rfl::parquet::save("/path/to/file.parquet", people);
```

With custom settings:

```cpp
const auto settings = rfl::parquet::Settings{}
    .with_compression(rfl::parquet::Compression::GZIP);
rfl::parquet::save("/path/to/file.parquet", people, settings);
```

## Reading from and writing into streams

You can also read from and write into any `std::istream` and `std::ostream` respectively.

```cpp
const rfl::Result<std::vector<Person>> result = rfl::parquet::read<std::vector<Person>>(my_istream);

const auto people = std::vector<Person>{...};
rfl::parquet::write(people, my_ostream);
```

With custom settings:

```cpp
const auto settings = rfl::parquet::Settings{}
    .with_compression(rfl::parquet::Compression::GZIP);
rfl::parquet::write(people, my_ostream, settings);
```

## Field name transformations

Like other formats, Parquet supports field name transformations. You can use processors like `SnakeCaseToCamelCase`:

```cpp
const auto people = std::vector<Person>{...};
const auto result = rfl::parquet::read<std::vector<Person>, rfl::SnakeCaseToCamelCase>(bytes);
```

This will automatically convert field names from snake_case to camelCase during serialization and deserialization.

## Supported processors

The following processors are **NOT supported** and will cause compilation errors:

- `rfl::AddTagsToVariants` - Cannot be used for tabular data
- `rfl::NoOptionals` - Cannot be used for tabular data  
- `rfl::DefaultIfMissing` - Cannot be used for tabular data
- `rfl::NoExtraFields` - Cannot be used for tabular data
- `rfl::NoFieldNames` - Cannot be used for tabular data

```cpp
// ✅ This works
const auto result = rfl::parquet::read<std::vector<Person>, rfl::SnakeCaseToCamelCase>(bytes);

// ❌ This will cause compilation errors
const auto result = rfl::parquet::read<std::vector<Person>, rfl::AddTagsToVariants>(bytes);
const auto result = rfl::parquet::read<std::vector<Person>, rfl::NoOptionals>(bytes);
const auto result = rfl::parquet::read<std::vector<Person>, rfl::DefaultIfMissing>(bytes);
```

## Enums and validation

Parquet supports enums and validated types. Enums are stored as strings:

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

## No variant types

Parquet does not support variant types like `std::variant`, `rfl::Variant`, or `rfl::TaggedUnion`. These types cannot be serialized to Parquet format.

```cpp
// ❌ This will NOT work
struct Person {
    std::string first_name;
    std::variant<std::string, int> status;  // Variant - not supported
    rfl::Variant<std::string, int> type;      // rfl::Variant - not supported
    rfl::TaggedUnion<"type", std::string, int> category;  // TaggedUnion - not supported
};
```

## Limitations of tabular formats

Parquet, like other tabular formats, has specific limitations that differ from hierarchical formats like JSON or XML:

### No nested objects
Unlike JSON or XML, Parquet cannot directly represent nested objects within a single row. Each field must be a primitive type, enum, or a simple container of primitives.

```cpp
// This works fine
struct Person {
    std::string first_name;
    std::string last_name;
    unsigned int age;
};

// This would NOT work as expected - nested objects are not automatically flattened
struct Address {
    std::string street;
    std::string city;
};

struct Person {
    std::string first_name;
    std::string last_name;
    Address address;  // ❌ This will cause compilation errors
};
```

### Using rfl::Flatten for nested objects

If you need to include nested objects, you can use `rfl::Flatten` to explicitly flatten them:

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

// The resulting Parquet file will have columns: first_name, last_name, street, city
```

### Collections requirement

You must serialize collections, not individual objects:
```cpp
std::vector<Person> people = {...};  // ✅ Correct
Person person = {...};               // ❌ Wrong - must be in a container
```

### No arrays (except bytestrings)
Parquet does not support arrays of any type except for binary data (bytestrings). This includes arrays of primitive types, strings, and objects.

```cpp
// ❌ This will NOT work
struct Person {
    std::string first_name;
    std::vector<std::string> hobbies;  // Array of strings - not supported
    std::vector<int> scores;           // Array of integers - not supported
    std::vector<Address> addresses;    // Array of objects - not supported
};

// ✅ This works
struct Person {
    std::string first_name;
    std::string last_name;
    std::vector<char> binary_data;      // Binary data - supported as bytestring
};
```
### Use cases
Parquet is ideal for:
- Data warehousing and analytics
- Large datasets with repeated values
- Integration with big data tools (Spark, Hadoop, etc.)
- Simple, flat data structures with consistent types

Parquet is less suitable for:
- Complex nested data structures
- Data with arrays or variant types
- Frequent schema changes
- Row-oriented access patterns
- Small datasets where the overhead isn't justified
- Data with complex object hierarchies

