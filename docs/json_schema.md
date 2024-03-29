# JSON schema

JSON schemata are a powerful tool for expressing the expected structure of your input. You can use it to validate your input before you even send it to your C++ backend, which will result in better UX.

It can also be used for code generation. For instance, tools such as https://app.quicktype.io/ allow you to generate code in multiple programming languages from the JSON schema (even though the validations are usually omitted). 

If you are interacting with Python, we warmly recommend https://docs.pydantic.dev/latest/integrations/datamodel_code_generator/. This allows you to generate Pydantic dataclasses, including the validation, from the JSON schema.

Note that this is only supported for JSON, not for other formats.

## Basic idea

Suppose you have a struct like this:

```cpp
struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Email email;
  std::vector<Person> children;
  float salary;
};
```

You can generate a JSON schema like this:

```cpp
const std::string json_schema = rfl::json::to_schema<Person>(rfl::json::pretty);
```

You do not have to pass `rfl::json::pretty`, but for the purposes of this documentation it is better to do so.

This will result in the following JSON schema:

```json
{
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "$ref": "#/definitions/Person",
    "definitions": {
        "Person": {
            "type": "object",
            "properties": {
                "children": {
                    "type": "array",
                    "items": {
                        "$ref": "#/definitions/Person"
                    }
                },
                "email": {
                    "type": "string",
                    "pattern": "^[a-zA-Z0-9._%+\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z]{2,}$"
                },
                "first_name": {
                    "type": "string"
                },
                "last_name": {
                    "type": "string"
                },
                "salary": {
                    "type": "number"
                }
            },
            "required": [
                "children",
                "email",
                "first_name",
                "last_name",
                "salary"
            ]
        }
    }
}
```

You can insert this into the tools mentioned above and see the generated code.

## Adding descriptions

JSON schemata also often contain descriptions. reflect-cpp supports this as well.

```cpp
struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Description<"Must be a proper email in the form xxx@xxx.xxx.",
                   rfl::Email>
      email;
  rfl::Description<
      "The person's children. Pass an empty array for no children.",
      std::vector<Person>>
      children;
  float salary;
};
```

```cpp
const std::string json_schema = rfl::json::to_schema<Person>(rfl::json::pretty);
```

```json
{
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "$ref": "#/definitions/Person",
    "definitions": {
        "Person": {
            "type": "object",
            "properties": {
                "children": {
                    "type": "array",
                    "description": "The person's children. Pass an empty array for no children.",
                    "items": {
                        "$ref": "#/definitions/Person"
                    }
                },
                "email": {
                    "type": "string",
                    "description": "Must be a proper email in the form xxx@xxx.xxx.",
                    "pattern": "^[a-zA-Z0-9._%+\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z]{2,}$"
                },
                "first_name": {
                    "type": "string"
                },
                "last_name": {
                    "type": "string"
                },
                "salary": {
                    "type": "number"
                }
            },
            "required": [
                "children",
                "email",
                "first_name",
                "last_name",
                "salary"
            ]
        }
    }
}
```

You also add a description to the entire JSON schema:

```cpp
const std::string json_schema = rfl::json::to_schema<
    rfl::Description<"JSON schema that describes the required "
                      "attributes for the person class.",
                      Person>>(rfl::json::pretty);
```

```json
{
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "$ref": "#/definitions/Person",
    "description": "JSON schema that describes the required attributes for the person class.",
    "definitions": {
        "Person": {
            "type": "object",
            "properties": {
                "children": {
                    "type": "array",
                    "description": "The person's children. Pass an empty array for no children.",
                    "items": {
                        "$ref": "#/definitions/Person"
                    }
                },
                "email": {
                    "type": "string",
                    "description": "Must be a proper email in the form xxx@xxx.xxx.",
                    "pattern": "^[a-zA-Z0-9._%+\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z]{2,}$"
                },
                "first_name": {
                    "type": "string"
                },
                "last_name": {
                    "type": "string"
                },
                "salary": {
                    "type": "number"
                }
            },
            "required": [
                "children",
                "email",
                "first_name",
                "last_name",
                "salary"
            ]
        }
    }
}
```
