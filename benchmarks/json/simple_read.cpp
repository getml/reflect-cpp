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

static rfl::Result<Person> read_using_yyjson_comparison() {
  yyjson_doc *doc = yyjson_read(json_string.c_str(), json_string.size(), 0);
  if (!doc) {
    return rfl::Error("Could not parse document!");
  }
  yyjson_val *root = yyjson_doc_get_root(doc);

  alignas(Person) unsigned char buf[sizeof(Person)];
  auto ptr = reinterpret_cast<Person *>(buf);
  auto view = rfl::Processors<>::template process<Person>(rfl::to_view(*ptr));

  const auto reader = rfl::json::Reader();

  const auto err = rfl::parsing::Parser<
      rfl::json::Reader, rfl::json::Writer, decltype(view),
      rfl::Processors<>>::read_view(reader,
                                    rfl::json::Reader::InputVarType(root),
                                    &view);

  if (err) {
    yyjson_doc_free(doc);
    return *err;
  }

  yyjson_doc_free(doc);

  return std::move(*ptr);
}

template <class Target, class Source>
inline void move_to(Target *_t, Source *_s) {
  if constexpr (std::is_const_v<Target>) {
    return move_to(const_cast<std::remove_const_t<Target> *>(_t), _s);
  } else if constexpr (!rfl::internal::is_array_v<Source> &&
                       !std::is_array_v<Target>) {
    ::new (_t) Target(std::move(*_s));
  } else if constexpr (rfl::internal::is_array_v<Source>) {
    static_assert(std::is_array_v<Target>, "Expected target to be a c-array.");
    for (size_t i = 0; i < _s->arr_.size(); ++i) {
      move_to(&((*_t)[i]), &(_s->arr_[i]));
    }
  } else {
    for (size_t i = 0; i < _s->size(); ++i) {
      move_to(&((*_t)[i]), &((*_s)[i]));
    }
  }
}

template <class R, class W, class ProcessorsType, int i>
inline void assign_if_field_matches(const R &_r,
                                    const std::string_view &_current_name,
                                    const auto &_var, auto *_view,
                                    auto *_errors, auto *_found, auto *_set,
                                    bool *_already_assigned) {
  using ViewType = std::remove_cvref_t<std::remove_pointer_t<decltype(_view)>>;
  using FieldType = std::tuple_element_t<i, typename ViewType::Fields>;
  using OriginalType = typename FieldType::Type;
  using T =
      std::remove_cvref_t<std::remove_pointer_t<typename FieldType::Type>>;
  constexpr auto name = FieldType::name();
  if (!(*_already_assigned) && !std::get<i>(*_found) && _current_name == name) {
    std::get<i>(*_found) = true;
    *_already_assigned = true;
    auto res = rfl::parsing::Parser<R, W, T, ProcessorsType>::read(_r, _var);
    if (!res) {
      _errors->emplace_back(rfl::Error("Failed to parse field '" +
                                       std::string(name) +
                                       "': " + std::move(res.error()->what())));
      return;
    }
    if constexpr (std::is_pointer_v<OriginalType>) {
      move_to(rfl::get<i>(*_view), &(*res));
    } else {
      rfl::get<i>(*_view) = std::move(*res);
    }
    std::get<i>(*_set) = true;
  }
}

template <class R, class W, class ProcessorsType, int... is>
inline void assign_to_matching_field(const R &_r,
                                     const std::string_view &_current_name,
                                     const auto &_var, auto *_view,
                                     auto *_errors, auto *_found, auto *_set,
                                     std::integer_sequence<int, is...>) {
  bool already_assigned = false;
  (assign_if_field_matches<R, W, ProcessorsType, is>(_r, _current_name, _var,
                                                     _view, _errors, _found,
                                                     _set, &already_assigned),
   ...);
}

static rfl::Result<Person> read_using_yyjson_comparison2() {
  yyjson_doc *doc = yyjson_read(json_string.c_str(), json_string.size(), 0);
  if (!doc) {
    return rfl::Error("Could not parse document!");
  }
  yyjson_val *root = yyjson_doc_get_root(doc);

  alignas(Person) unsigned char buf[sizeof(Person)];
  auto ptr = reinterpret_cast<Person *>(buf);

  auto view = rfl::to_view(*ptr);

  const auto reader = rfl::json::Reader();

  std::vector<rfl::Error> errors;
  std::array<bool, 3> found;
  std::fill(found.begin(), found.end(), false);
  std::array<bool, 3> set;
  std::fill(set.begin(), set.end(), false);

  yyjson_obj_iter iter;
  yyjson_obj_iter_init(root, &iter);
  yyjson_val *key;
  while ((key = yyjson_obj_iter_next(&iter))) {
    const auto name = std::string_view(yyjson_get_str(key));
    const auto v = yyjson_obj_iter_get_val(key);
    assign_to_matching_field<rfl::json::Reader, rfl::json::Writer,
                             rfl::Processors<>>(
        reader, name, rfl::json::Reader::InputVarType(v), &view, &errors,
        &found, &set, std::make_integer_sequence<int, view.size()>());
  }
  if (!std::get<0>(found)) {
    errors.push_back(rfl::Error("'first_name' not found"));
  }
  if (!std::get<1>(found)) {
    errors.push_back(rfl::Error("'last_name' not found"));
  }
  if (!std::get<2>(found)) {
    errors.push_back(rfl::Error("'age' not found"));
  }
  if (errors.size() != 0) {
    std::cout << "Some errors occurred:" << std::endl;
    for (const auto &err : errors) {
      std::cout << err.what() << std::endl;
    }
  }

  yyjson_doc_free(doc);

  return std::move(*ptr);
}

static rfl::Result<Person> read_using_yyjson_iter() {
  yyjson_doc *doc = yyjson_read(json_string.c_str(), json_string.size(), 0);
  if (!doc) {
    std::cout << "Could not parse document!" << std::endl;
    return rfl::Error("Could not parse document");
  }
  yyjson_val *root = yyjson_doc_get_root(doc);

  Person homer;

  std::vector<rfl::Error> errors;
  std::array<bool, 3> found;
  std::fill(found.begin(), found.end(), false);

  yyjson_obj_iter iter;
  yyjson_obj_iter_init(root, &iter);
  yyjson_val *key;
  while ((key = yyjson_obj_iter_next(&iter))) {
    const auto name = std::string_view(yyjson_get_str(key));
    const auto v = yyjson_obj_iter_get_val(key);
    if (!std::get<0>(found) && name == "first_name") {
      auto first_name = yyjson_get_str(v);
      if (first_name == NULL) {
        errors.push_back(rfl::Error(
            "Error reading 'first_name': Could not cast to string."));
      }
      homer.first_name = first_name;
      std::get<0>(found) = true;
    } else if (!std::get<1>(found) && name == "last_name") {
      auto last_name = yyjson_get_str(v);
      if (last_name == NULL) {
        errors.push_back(
            rfl::Error("Error reading 'last_name': Could not cast to string."));
      }
      homer.last_name = last_name;
      std::get<1>(found) = true;
    } else if (!std::get<2>(found) && name == "age") {
      auto age = yyjson_get_str(v);
      if (age == NULL) {
        errors.push_back(
            rfl::Error("Error reading 'age': Could not cast to string."));
      }
      homer.age = age;
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
    errors.push_back(rfl::Error("'age' not found"));
  }
  if (errors.size() != 0) {
    std::cout << "Some errors occurred:" << std::endl;
    for (const auto &err : errors) {
      std::cout << err.what() << std::endl;
    }
  }

  yyjson_doc_free(doc);

  return homer;
}

static rfl::Result<Person> read_using_reflect_cpp() {
  return rfl::json::read<Person>(json_string);
}

static void BM_simple_read_yyjson_iter(benchmark::State &state) {
  for (auto _ : state) {
    const auto res = read_using_yyjson_iter();
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_simple_read_yyjson_iter);

static void BM_simple_read_reflect_cpp(benchmark::State &state) {
  for (auto _ : state) {
    const auto res = read_using_reflect_cpp();
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_simple_read_reflect_cpp);

static void BM_simple_read_yyjson_comparison(benchmark::State &state) {
  for (auto _ : state) {
    const auto res = read_using_yyjson_comparison();
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_simple_read_yyjson_comparison);

static void BM_simple_read_yyjson_comparison2(benchmark::State &state) {
  for (auto _ : state) {
    const auto res = read_using_yyjson_comparison2();
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_simple_read_yyjson_comparison2);

}  // namespace simple_read

BENCHMARK_MAIN();
