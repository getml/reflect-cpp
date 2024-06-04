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

namespace licenses_read {

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

static void BM_licenses_read_reflect_cpp_bson(benchmark::State &state) {
  const auto data = rfl::bson::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::bson::read<Licenses>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_read_reflect_cpp_bson);

static void BM_licenses_read_reflect_cpp_cbor(benchmark::State &state) {
  const auto data = rfl::cbor::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::cbor::read<Licenses>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_read_reflect_cpp_cbor);

static void BM_licenses_read_reflect_cpp_flexbuf(benchmark::State &state) {
  const auto data = rfl::flexbuf::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::flexbuf::read<Licenses>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_read_reflect_cpp_flexbuf);

static void BM_licenses_read_reflect_cpp_json(benchmark::State &state) {
  const auto data = rfl::json::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::json::read<Licenses>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_read_reflect_cpp_json);

static void BM_licenses_read_reflect_cpp_msgpack(benchmark::State &state) {
  const auto data = rfl::msgpack::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::msgpack::read<Licenses>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_read_reflect_cpp_msgpack);

static void BM_licenses_read_reflect_cpp_xml(benchmark::State &state) {
  const auto data = rfl::xml::write<"license">(load_data());
  for (auto _ : state) {
    const auto res = rfl::xml::read<Licenses>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_read_reflect_cpp_xml);

static void BM_licenses_read_reflect_cpp_toml(benchmark::State &state) {
  const auto data = rfl::toml::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::toml::read<Licenses>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_read_reflect_cpp_toml);

static void BM_licenses_read_reflect_cpp_yaml(benchmark::State &state) {
  const auto data = rfl::yaml::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::yaml::read<Licenses>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_read_reflect_cpp_yaml);

// ----------------------------------------------------------------------------

}  // namespace licenses_read

