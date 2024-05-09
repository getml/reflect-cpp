#include <benchmark/benchmark.h>

#include <array>
#include <iostream>
#include <rfl/json.hpp>

namespace simple_read {

static const std::string json_string = R"(
    {
        "first_name": "Homer",
        "last_name": "Simpson",
        "age": 45
    }
)";

struct Person {
  std::string first_name;
  std::string last_name;
  int age;
};

static void read_using_yyjson_naive() {
  yyjson_doc *doc = yyjson_read(json_string.c_str(), json_string.size(), 0);
  if (!doc) {
    std::cout << "Could not parse document!" << std::endl;
    return;
  }
  yyjson_val *root = yyjson_doc_get_root(doc);
  yyjson_val *v;

  Person homer;

  v = yyjson_obj_get(root, "first_name");
  if (v == NULL) {
    std::cout << "'first_name' not found." << std::endl;
    yyjson_doc_free(doc);
    return;
  }
  auto first_name = yyjson_get_str(v);
  if (first_name == NULL) {
    std::cout << "Could not cast to string." << std::endl;
    yyjson_doc_free(doc);
    return;
  }
  homer.first_name = first_name;

  v = yyjson_obj_get(root, "last_name");
  if (v == NULL) {
    std::cout << "'last_name' not found." << std::endl;
    yyjson_doc_free(doc);
    return;
  }
  auto last_name = yyjson_get_str(v);
  if (last_name == NULL) {
    std::cout << "Could not cast to string." << std::endl;
    yyjson_doc_free(doc);
    return;
  }
  homer.last_name = last_name;

  v = yyjson_obj_get(root, "age");
  if (v == NULL) {
    std::cout << "'age' not found." << std::endl;
    yyjson_doc_free(doc);
    return;
  }
  if (!yyjson_is_int(v)) {
    std::cout << "Could not cast to int." << std::endl;
    yyjson_doc_free(doc);
    return;
  }
  homer.age = yyjson_get_int(v);

  yyjson_doc_free(doc);
}

static void read_using_yyjson_iter() {
  yyjson_doc *doc = yyjson_read(json_string.c_str(), json_string.size(), 0);
  if (!doc) {
    std::cout << "Could not parse document!" << std::endl;
    return;
  }
  yyjson_val *root = yyjson_doc_get_root(doc);

  Person homer;

  std::vector<std::string> errors;
  std::array<bool, 3> found;
  std::fill(found.begin(), found.end(), false);

  yyjson_obj_iter iter;
  yyjson_obj_iter_init(root, &iter);
  yyjson_val *key;
  while ((key = yyjson_obj_iter_next(&iter))) {
    const auto name = std::string_view(yyjson_get_str(key));
    const auto v = yyjson_obj_iter_get_val(key);
    if (name == "first_name") {
      auto first_name = yyjson_get_str(v);
      if (first_name == NULL) {
        errors.push_back(
            "Error reading 'first_name': Could not cast to string.");
      }
      homer.first_name = first_name;
      std::get<0>(found) = true;
    } else if (name == "last_name") {
      auto last_name = yyjson_get_str(v);
      if (last_name == NULL) {
        errors.push_back(
            "Error reading 'last_name': Could not cast to string.");
      }
      homer.last_name = last_name;
      std::get<1>(found) = true;
    } else if (name == "age") {
      if (!yyjson_is_int(v)) {
        errors.push_back("Error reading 'age': Could not cast to int.");
      }
      homer.age = yyjson_get_int(v);
      std::get<2>(found) = true;
    }
  }
  if (!std::get<0>(found)) {
    errors.push_back("'first_name' not found");
  }
  if (!std::get<1>(found)) {
    errors.push_back("'last_name' not found");
  }
  if (!std::get<2>(found)) {
    errors.push_back("'age' not found");
  }
  if (errors.size() != 0) {
    std::cout << "Some errors occurred:" << std::endl;
    for (const auto &err : errors) {
      std::cout << err << std::endl;
    }
  }

  yyjson_doc_free(doc);
}

static void BM_simple_read_yyjson_naive(benchmark::State &state) {
  for (auto _ : state) read_using_yyjson_naive();
}
BENCHMARK(BM_simple_read_yyjson_naive);

static void BM_simple_read_yyjson_iter(benchmark::State &state) {
  for (auto _ : state) read_using_yyjson_iter();
}
BENCHMARK(BM_simple_read_yyjson_iter);

static void BM_simple_read_reflect_cpp(benchmark::State &state) {
  for (auto _ : state) rfl::json::read<Person>(json_string);
}
BENCHMARK(BM_simple_read_reflect_cpp);

}  // namespace simple_read

BENCHMARK_MAIN();
