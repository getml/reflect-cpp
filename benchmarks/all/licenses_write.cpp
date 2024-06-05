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

namespace licenses_write {

// ----------------------------------------------------------------------------

struct Identifier {
  std::string identifier;
  std::string scheme;
};

struct Link {
  std::string note;
  std::string url;
};

struct OtherName {
  std::string name;
  std::optional<std::string> note;
};

struct Text {
  std::string media_type;
  std::string title;
  std::string url;
};

struct License {
  std::string id;
  std::vector<Identifier> identifiers;
  std::vector<Link> links;
  std::string name;
  std::vector<OtherName> other_names;
  std::optional<std::string> superseded_by;
  std::vector<std::string> keywords;
  std::vector<Text> text;
};

struct Licenses {
  std::vector<License> licenses;
};

// ----------------------------------------------------------------------------

static Licenses load_data() {
  return rfl::json::load<Licenses>("benchmarks/data/licenses.json").value();
}

// ----------------------------------------------------------------------------

static void BM_licenses_write_reflect_cpp_bson(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::bson::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_write_reflect_cpp_bson);

static void BM_licenses_write_reflect_cpp_cbor(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::cbor::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_write_reflect_cpp_cbor);

static void BM_licenses_write_reflect_cpp_flexbuf(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::flexbuf::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_write_reflect_cpp_flexbuf);

static void BM_licenses_write_reflect_cpp_json(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::json::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_write_reflect_cpp_json);

static void BM_licenses_write_reflect_cpp_msgpack(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::msgpack::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_write_reflect_cpp_msgpack);

static void BM_licenses_write_reflect_cpp_toml(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::toml::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_write_reflect_cpp_toml);

static void BM_licenses_write_reflect_cpp_xml(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::xml::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_write_reflect_cpp_xml);

static void BM_licenses_write_reflect_cpp_yaml(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::yaml::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_write_reflect_cpp_yaml);

// ----------------------------------------------------------------------------

}  // namespace licenses_write

