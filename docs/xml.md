# XML 

For XML support, you must also include the header `<rfl/xml.hpp>` and include the pugixml library (https://github.com/zeux/pugixml) in your project.

## Simple example

Consider the following example:

```cpp
using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Email email;
  std::vector<Person> child;
};

const auto bart = Person{.first_name = "Bart",
                       .birthday = "1987-04-19",
                       .age = 10,
                       .email = "bart@simpson.com"};

const auto lisa = Person{.first_name = "Lisa",
                       .birthday = "1987-04-19",
                       .age = 8,
                       .email = "lisa@simpson.com"};

const auto maggie = Person{.first_name = "Maggie",
                         .birthday = "1987-04-19",
                         .age = 0,
                         .email = "maggie@simpson.com"};

const auto homer = Person{.first_name = "Homer",
                        .birthday = "1987-04-19",
                        .age = 45,
                        .email = "homer@simpson.com",
                        .child = std::vector<Person>({bart, lisa, maggie})};

rfl::xml::write(homer);
```

This will result in the following XML string:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Person>
    <firstName>Homer</firstName>
    <lastName>Simpson</lastName>
    <town>Springfield</town>
    <birthday>1987-04-19</birthday>
    <age>45</age>
    <email>homer@simpson.com</email>
    <child>
        <firstName>Bart</firstName>
        <lastName>Simpson</lastName>
        <town>Springfield</town>
        <birthday>1987-04-19</birthday>
        <age>10</age>
        <email>bart@simpson.com</email>
    </child>
    <child>
        <firstName>Lisa</firstName>
        <lastName>Simpson</lastName>
        <town>Springfield</town>
        <birthday>1987-04-19</birthday>
        <age>8</age>
        <email>lisa@simpson.com</email>
    </child>
    <child>
        <firstName>Maggie</firstName>
        <lastName>Simpson</lastName>
        <town>Springfield</town>
        <birthday>1987-04-19</birthday>
        <age>0</age>
        <email>maggie@simpson.com</email>
    </child>
</Person>
```

Unlike most other formats, XML distinguishes between attributes and nodes. 
If you want something to be displayed as an attribute, you must mark it as such:

```cpp
using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Rename<"firstName", rfl::Attribute<std::string>> first_name;
  rfl::Rename<"lastName", rfl::Attribute<std::string>> last_name = "Simpson";
  rfl::Attribute<std::string> town = "Springfield";
  rfl::Attribute<rfl::Timestamp<"%Y-%m-%d">> birthday;
  rfl::Attribute<Age> age;
  rfl::Attribute<rfl::Email> email;
  std::vector<Person> child;
};

const auto bart = Person{.first_name = "Bart",
                         .birthday = "1987-04-19",
                         .age = 10,
                         .email = "bart@simpson.com"};

const auto lisa = Person{.first_name = "Lisa",
                         .birthday = "1987-04-19",
                         .age = 8,
                         .email = "lisa@simpson.com"};

const auto maggie = Person{.first_name = "Maggie",
                           .birthday = "1987-04-19",
                           .age = 0,
                           .email = "maggie@simpson.com"};

const auto homer = Person{.first_name = "Homer",
                          .birthday = "1987-04-19",
                          .age = 45,
                          .email = "homer@simpson.com",
                          .child = std::vector<Person>({bart, lisa, maggie})};

rfl::xml::write(homer);
```

This will result in the following XML string:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Person firstName="Homer" lastName="Simpson" town="Springfield" birthday="1987-04-19" age="45" email="homer@simpson.com">
    <child firstName="Bart" lastName="Simpson" town="Springfield" birthday="1987-04-19" age="10" email="bart@simpson.com" />
    <child firstName="Lisa" lastName="Simpson" town="Springfield" birthday="1987-04-19" age="8" email="lisa@simpson.com" />
    <child firstName="Maggie" lastName="Simpson" town="Springfield" birthday="1987-04-19" age="0" email="maggie@simpson.com" />
</Person>
```

Note that only boolean values, string values, integral values or floating point values can be represented as attributes.

There also is a special field name called `xml_content` to be used when you want a value directly inserted into the content.
Again, only boolean values, string values, integral values or floating point values can be represented this way:

```cpp
struct Person {
  std::string xml_content;
  rfl::Attribute<std::string> town = "Springfield";
  rfl::Attribute<rfl::Timestamp<"%Y-%m-%d">> birthday;
  rfl::Attribute<rfl::Email> email;
  std::vector<Person> child;
};

const auto bart = Person{.xml_content = "Bart Simpson",
                         .birthday = "1987-04-19",
                         .email = "bart@simpson.com"};

const auto lisa = Person{.xml_content = "Lisa Simpson",
                         .birthday = "1987-04-19",
                         .email = "lisa@simpson.com"};

const auto maggie = Person{.xml_content = "Maggie Simpson",
                           .birthday = "1987-04-19",
                           .email = "maggie@simpson.com"};

const auto homer = Person{.xml_content = "Homer Simpson",
                          .birthday = "1987-04-19",
                          .email = "homer@simpson.com",
                          .child = std::vector<Person>({bart, lisa, maggie})};

rfl::xml::write(homer);
```

This will result in the following XML string:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Person town="Springfield" birthday="1987-04-19" email="homer@simpson.com">Homer Simpson<child town="Springfield" birthday="1987-04-19" email="bart@simpson.com">Bart Simpson</child>
    <child town="Springfield" birthday="1987-04-19" email="lisa@simpson.com">Lisa Simpson</child>
    <child town="Springfield" birthday="1987-04-19" email="maggie@simpson.com">Maggie Simpson</child>
</Person>
```

## Limitations of the XML format

### There must be exactly one root node

The XML format requires that there must be exactly one root node. 
Other formats, like JSON, allow you to have an array as the root element. XML disallows that.

### The root node must have a name

As you may have noticed, reflect-cpp uses the names of fields as the names of the XML nodes or attributes.
However, the root node does not have a field name associated with it. So reflect-cpp will use the name
of the struct as the name of the root node. This is a problem when the root node is a template parameter,
because the name of the root node cannot contain "<" or ">".

In this case, you can explicitly set a root node name like this:

```cpp
rfl::xml::write<"NameOfTheRootNode">(homer);
```

### No nested arrays

Unlike JSON, XML doesn't have an explicit concept of arrays. Array-like structures are represented by simply
writing the same field over and over again (i.e. Homer's children in the examples above).

This also implies that something like this cannot be properly represented in XML:

```cpp
struct Person {
  std::string name;
  std::vector<std::vector<Person>> child;
};
```

There is no way to represent a vector of vectors in XML. It's a limitation of the format.

### Integers cannot be tags

In JSON, it is possible to represent a map with integers as keys like this:

```cpp
std::map<int, std::string> homer;
homer[1] = "Homer";
homer[2] = "Simpson";

rfl::json::write(homer);
```

This will be represented as follows:

```json
{"1":"Homer","2":"Simpson"}
```

In XML, this is impossible. The XML standard requires that the tag must not start with an integer.

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::xml::load<Person>("/path/to/file.xml");

const auto person = Person{...};
rfl::xml::save("/path/to/file.xml", person);
```

