#include <benchmark/benchmark.h>
#include <rapidjson/document.h>
#include <simdjson.h>

#include <array>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <rfl/json.hpp>
#include <type_traits>
#include <vector>

namespace person_read {

// ----------------------------------------------------------------------------

static const std::string json_string = R"(
    {
        "first_name": "Homer",
        "last_name": "Simpson",
        "children": [
            {
                "first_name": "Bart",
                "last_name": "Simpson",
                "children": []
            },
            {
                "first_name": "Lisa",
                "last_name": "Simpson",
                "children": []
            },
            {
                "first_name": "Maggie",
                "last_name": "Simpson",
                "children": []
            }
        ]
    }
)";

struct Person {
  std::string first_name;
  std::string last_name;
  std::vector<Person> children;
};

// ----------------------------------------------------------------------------
// nlohmann/json

std::vector<Person> nlohmann_to_children(const nlohmann::json &_val);

Person nlohmann_to_person(const nlohmann::json &_val);

std::vector<Person> nlohmann_to_children(const nlohmann::json &_arr) {
  std::vector<Person> children;
  for (const auto &val : _arr) {
    children.push_back(nlohmann_to_person(val));
  }
  return children;
}

Person nlohmann_to_person(const nlohmann::json &_val) {
  Person person;
  person.first_name = _val["first_name"].template get<std::string>();
  person.last_name = _val["last_name"].template get<std::string>();
  person.children = nlohmann_to_children(_val["children"]);
  return person;
}

static rfl::Result<Person> read_using_nlohmann() {
  try {
    auto val = nlohmann::json::parse(json_string);
    return nlohmann_to_person(val);
  } catch (std::exception &e) {
    return rfl::Error(e.what());
  }
}

// ----------------------------------------------------------------------------
// rapidjson

std::vector<Person> rapidjson_to_children(const rapidjson::Value &_arr);

Person rapidjson_to_person(const rapidjson::Value &_val);

std::vector<Person> rapidjson_to_children(const rapidjson::Value &_arr) {
  std::vector<Person> children;
  for (auto it = _arr.Begin(); it != _arr.End(); ++it) {
    children.push_back(rapidjson_to_person(it->GetObject()));
  }
  return children;
}

Person rapidjson_to_person(const rapidjson::Value &_val) {
  Person person;
  person.first_name = _val["first_name"].GetString();
  person.last_name = _val["last_name"].GetString();
  person.children = rapidjson_to_children(_val["children"].GetObject());
  return person;
}

static rfl::Result<Person> read_using_rapidjson() {
  try {
    rapidjson::Document d;
    d.Parse(json_string.c_str());
    return rapidjson_to_person(d.GetObject());
  } catch (std::exception &e) {
    return rfl::Error(e.what());
  }
}

// ----------------------------------------------------------------------------
// simdjson

std::vector<Person> simdjson_to_children(simdjson::ondemand::array _arr);

Person simdjson_to_person(simdjson::ondemand::object _val);

std::vector<Person> simdjson_to_children(simdjson::ondemand::array _arr) {
  std::vector<Person> children;
  for (auto val : _arr) {
    children.push_back(simdjson_to_person(val));
  }
  return children;
}

Person simdjson_to_person(simdjson::ondemand::object _val) {
  Person person;
  person.first_name = _val["first_name"].get_string().value();
  person.last_name = _val["last_name"].get_string().value();
  person.children = simdjson_to_children(_val["children"].get_array());
  return person;
}

static rfl::Result<Person> read_using_simdjson() {
  try {
    simdjson::ondemand::parser parser;
    auto padded_str = simdjson::padded_string(json_string);
    auto doc = parser.iterate(padded_str).value();
    return simdjson_to_person(doc.get_object());
  } catch (std::exception &e) {
    return rfl::Error(e.what());
  }
}

// ----------------------------------------------------------------------------
// yyjson

rfl::Result<std::vector<Person>> yyjson_to_children(yyjson_val *_val);

rfl::Result<Person> yyjson_to_person(yyjson_val *_val);

rfl::Result<std::vector<Person>> yyjson_to_children(yyjson_val *_val) {
  std::vector<Person> children;
  yyjson_val *val;
  yyjson_arr_iter iter;
  yyjson_arr_iter_init(_val, &iter);
  while ((val = yyjson_arr_iter_next(&iter))) {
    auto r = yyjson_to_person(val);
    if (!r) {
      return *r.error();
    }
    children.emplace_back(std::move(*r));
  }
  return children;
}

rfl::Result<Person> yyjson_to_person(yyjson_val *_val) {
  Person person;

  std::vector<rfl::Error> errors;
  std::array<bool, 3> found;
  std::fill(found.begin(), found.end(), false);

  yyjson_obj_iter iter;
  yyjson_obj_iter_init(_val, &iter);
  yyjson_val *key;
  while ((key = yyjson_obj_iter_next(&iter))) {
    const auto name = std::string_view(yyjson_get_str(key));
    const auto v = yyjson_obj_iter_get_val(key);
    if (!std::get<0>(found) && name == "first_name") {
      auto first_name = yyjson_get_str(v);
      if (first_name == NULL) {
        errors.push_back(rfl::Error(
            "Error reading 'first_name': Could not cast to string."));
        continue;
      }
      person.first_name = first_name;
      std::get<0>(found) = true;
    } else if (!std::get<1>(found) && name == "last_name") {
      auto last_name = yyjson_get_str(v);
      if (last_name == NULL) {
        errors.push_back(
            rfl::Error("Error reading 'last_name': Could not cast to string."));
        continue;
      }
      person.last_name = last_name;
      std::get<1>(found) = true;
    } else if (!std::get<2>(found) && name == "children") {
      auto children = yyjson_to_children(v);
      if (!children) {
        errors.push_back(rfl::Error("Error reading 'children': " +
                                    children.error()->what()));
        continue;
      }
      person.children = std::move(*children);
      std::get<2>(found) = true;
    }
  }
  if (!std::get<0>(found)) {
    errors.push_back(rfl::Error("'first_name' not found"));
  }
  if (!std::get<1>(found)) {
    errors.push_back(rfl::Error("'last_name' not found"));
  }
  if (!std::get<2>(found)) {
    errors.push_back(rfl::Error("'children' not found"));
  }
  if (errors.size() != 0) {
    std::cout << "Some errors occurred:" << std::endl;
    for (const auto &err : errors) {
      std::cout << err.what() << std::endl;
    }
    return errors[0];
  }
  return person;
}

static rfl::Result<Person> read_using_yyjson() {
  yyjson_doc *doc = yyjson_read(json_string.c_str(), json_string.size(), 0);
  if (!doc) {
    std::cout << "Could not parse document!" << std::endl;
    return rfl::Error("Could not parse document");
  }
  yyjson_val *root = yyjson_doc_get_root(doc);
  auto person = yyjson_to_person(root);
  yyjson_doc_free(doc);
  return person;
}

// ----------------------------------------------------------------------------

static void BM_person_read_nlohmann(benchmark::State &state) {
  for (auto _ : state) {
    const auto res = read_using_nlohmann();
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_nlohmann);

static void BM_person_read_rapidjson(benchmark::State &state) {
  for (auto _ : state) {
    const auto res = read_using_rapidjson();
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_rapidjson);

static void BM_person_read_simdjson(benchmark::State &state) {
  for (auto _ : state) {
    const auto res = read_using_simdjson();
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_simdjson);

static void BM_person_read_yyjson(benchmark::State &state) {
  for (auto _ : state) {
    const auto res = read_using_yyjson();
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_yyjson);

static void BM_person_read_reflect_cpp(benchmark::State &state) {
  for (auto _ : state) {
    const auto res = rfl::json::read<Person>(json_string);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_reflect_cpp);

// ----------------------------------------------------------------------------

}  // namespace person_read

