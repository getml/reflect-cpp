#include <benchmark/benchmark.h>

#include <array>
#include <iostream>
#include <rfl/json.hpp>
#include <type_traits>

namespace simple_read {

static const std::string json_string = R"(
    {
        "first_name": "Homer",
        "last_name": "Simpson",
        "age": "45"
    }
)";

struct Person {
  std::string first_name;
  std::string last_name;
  std::string age;
};

struct View {
  std::string *ptr0;
  std::string *ptr1;
  std::string *ptr2;
};

template <int _i>
inline auto get(auto &_view) {
  if constexpr (_i == 0) {
    return _view.ptr0;
  } else if constexpr (_i == 1) {
    return _view.ptr1;
  } else if constexpr (_i == 2) {
    return _view.ptr2;
  }
};

static void read_using_yyjson_comparison() {
  yyjson_doc *doc = yyjson_read(json_string.c_str(), json_string.size(), 0);
  if (!doc) {
    std::cout << "Could not parse document!" << std::endl;
    return;
  }
  yyjson_val *root = yyjson_doc_get_root(doc);

  Person homer;

  auto view = rfl::to_view(homer);

  const auto reader = rfl::json::Reader();

  const auto err = rfl::parsing::Parser<
      rfl::json::Reader, rfl::json::Writer, decltype(view),
      rfl::Processors<>>::read_view(reader,
                                    rfl::json::Reader::InputVarType(root),
                                    &view);

  if (err) {
    std::cout << err->what() << std::endl;
  }

  yyjson_doc_free(doc);
}

template <class ViewType, int i>
void assign_one(const rfl::json::Reader &_r,
                const std::string_view &_current_name,
                const rfl::json::Reader::InputVarType &_var, ViewType *_view,
                auto *_errors, auto *_found, bool *_ok) {
  using FieldType = std::tuple_element_t<i, typename ViewType::Fields>;
  using T = std::remove_pointer_t<typename FieldType::Type>;
  constexpr auto name = FieldType::name();
  if (!(*_ok) && _current_name == name) {
    auto r = rfl::parsing::Parser<rfl::json::Reader, rfl::json::Writer, T,
                                  rfl::Processors<>>::read(_r, _var);
    if (!r) {
      _errors->push_back(r.error()->what());
    }
    *rfl::get<i>(*_view) = std::move(*r);
    std::get<i>(*_found) = true;
  }
}

template <class ViewType, int... is>
void assign_all(const rfl::json::Reader &_r,
                const std::string_view &_current_name,
                const rfl::json::Reader::InputVarType &_var, ViewType *_view,
                auto *_errors, auto *_found, bool *_ok,
                std::integer_sequence<int, is...>) {
  (assign_one<ViewType, is>(_r, _current_name, _var, _view, _errors, _found,
                            _ok),
   ...);
}

static void read_using_yyjson_comparison2() {
  yyjson_doc *doc = yyjson_read(json_string.c_str(), json_string.size(), 0);
  if (!doc) {
    std::cout << "Could not parse document!" << std::endl;
    return;
  }
  yyjson_val *root = yyjson_doc_get_root(doc);

  Person homer;

  auto view = rfl::to_view(homer);

  const auto reader = rfl::json::Reader();

  std::vector<std::string> errors;
  std::array<bool, 3> found;
  std::fill(found.begin(), found.end(), false);

  yyjson_obj_iter iter;
  yyjson_obj_iter_init(root, &iter);
  yyjson_val *key;
  while ((key = yyjson_obj_iter_next(&iter))) {
    const auto name = std::string_view(yyjson_get_str(key));
    const auto v = yyjson_obj_iter_get_val(key);
    bool ok = false;
    assign_all<typeof(view)>(reader, name, rfl::json::Reader::InputVarType(v),
                             &view, &errors, &found, &ok,
                             std::make_integer_sequence<int, view.size()>());
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
      auto age = yyjson_get_str(v);
      if (age == NULL) {
        errors.push_back("Error reading 'age': Could not cast to string.");
      }
      homer.age = age;
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

static void BM_simple_read_yyjson_iter(benchmark::State &state) {
  for (auto _ : state) read_using_yyjson_iter();
}
BENCHMARK(BM_simple_read_yyjson_iter);

static void BM_simple_read_yyjson_comparison(benchmark::State &state) {
  for (auto _ : state) read_using_yyjson_comparison();
}
BENCHMARK(BM_simple_read_yyjson_comparison);

static void BM_simple_read_yyjson_comparison2(benchmark::State &state) {
  for (auto _ : state) read_using_yyjson_comparison2();
}
BENCHMARK(BM_simple_read_yyjson_comparison2);

static void BM_simple_read_reflect_cpp(benchmark::State &state) {
  for (auto _ : state) rfl::json::read<Person>(json_string);
}
BENCHMARK(BM_simple_read_reflect_cpp);

}  // namespace simple_read

BENCHMARK_MAIN();
