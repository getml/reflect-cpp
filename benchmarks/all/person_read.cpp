#include <benchmark/benchmark.h>

#include <array>
#include <iostream>
#include <optional>
#include <rfl/bson.hpp>
#include <rfl/cbor.hpp>
#include <rfl/flexbuf.hpp>
#include <rfl/json.hpp>
#include <rfl/msgpack.hpp>
#include <rfl/toml.hpp>
#include <rfl/xml.hpp>
#include <rfl/yaml.hpp>
#include <type_traits>
#include <vector>

namespace person_read {

// ----------------------------------------------------------------------------

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  int age;
  std::string town = "Springfield";
  std::vector<Person> children;
};

// ----------------------------------------------------------------------------

static Person load_data() {
  const auto bart = Person{.first_name = "Bart", .age = 10};

  const auto lisa = Person{.first_name = "Lisa", .age = 8};

  const auto maggie = Person{.first_name = "Maggie", .age = 0};

  return Person{.first_name = "Homer",
                .age = 45,
                .children = std::vector<Person>({bart, lisa, maggie})};
}

// ----------------------------------------------------------------------------

static void BM_person_read_reflect_cpp_bson(benchmark::State &state) {
  const auto data = rfl::bson::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::bson::read<Person>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_reflect_cpp_bson);

static void BM_person_read_reflect_cpp_cbor(benchmark::State &state) {
  const auto data = rfl::cbor::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::cbor::read<Person>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_reflect_cpp_cbor);

static void BM_person_read_reflect_cpp_flexbuf(benchmark::State &state) {
  const auto data = rfl::flexbuf::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::flexbuf::read<Person>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_reflect_cpp_flexbuf);

static void BM_person_read_reflect_cpp_json(benchmark::State &state) {
  const auto data = rfl::json::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::json::read<Person>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_reflect_cpp_json);

static void BM_person_read_reflect_cpp_msgpack(benchmark::State &state) {
  const auto data = rfl::msgpack::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::msgpack::read<Person>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_reflect_cpp_msgpack);

static void BM_person_read_reflect_cpp_toml(benchmark::State &state) {
  const auto data = rfl::toml::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::toml::read<Person>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_reflect_cpp_toml);

static void BM_person_read_reflect_cpp_xml(benchmark::State &state) {
  const auto data = rfl::xml::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::xml::read<Person>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_reflect_cpp_xml);

static void BM_person_read_reflect_cpp_yaml(benchmark::State &state) {
  const auto data = rfl::yaml::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::yaml::read<Person>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_person_read_reflect_cpp_yaml);

// ----------------------------------------------------------------------------

}  // namespace person_read

