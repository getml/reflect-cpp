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
#include <rfl/ubjson.hpp>
#include <rfl/yaml.hpp>
#include <type_traits>
#include <vector>

namespace canada_write {

// ----------------------------------------------------------------------------

struct Property {
  std::string name;
};

struct Geometry {
  rfl::Literal<"Polygon"> type;
  std::vector<std::vector<std::tuple<double, double>>> coordinates;
};

struct Feature {
  rfl::Literal<"Feature"> type;
  Property properties;
  Geometry geometry;
};

struct FeatureCollection {
  rfl::Literal<"FeatureCollection"> type;
  std::vector<Feature> features;
};

// ----------------------------------------------------------------------------

static FeatureCollection load_data() {
  return rfl::json::load<FeatureCollection>("benchmarks/data/canada.json")
      .value();
}

// ----------------------------------------------------------------------------

static void BM_canada_write_reflect_cpp_bson(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::bson::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_write_reflect_cpp_bson);

static void BM_canada_write_reflect_cpp_cbor(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::cbor::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_write_reflect_cpp_cbor);

static void BM_canada_write_reflect_cpp_cbor_without_field_names(
    benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::cbor::write<rfl::NoFieldNames>(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_write_reflect_cpp_cbor_without_field_names);

static void BM_canada_write_reflect_cpp_flexbuf_without_field_names(
    benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::flexbuf::write<rfl::NoFieldNames>(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_write_reflect_cpp_flexbuf_without_field_names);

static void BM_canada_write_reflect_cpp_json(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::json::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_write_reflect_cpp_json);

static void BM_canada_write_reflect_cpp_json_without_field_names(
    benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::json::write<rfl::NoFieldNames>(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_write_reflect_cpp_json_without_field_names);

static void BM_canada_write_reflect_cpp_msgpack(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::msgpack::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_write_reflect_cpp_msgpack);

static void BM_canada_write_reflect_cpp_msgpack_without_field_names(
    benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::msgpack::write<rfl::NoFieldNames>(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_write_reflect_cpp_msgpack_without_field_names);

static void BM_canada_write_reflect_cpp_toml(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::toml::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_write_reflect_cpp_toml);

static void BM_canada_write_reflect_cpp_ubjson(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::ubjson::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_write_reflect_cpp_ubjson);

static void BM_canada_write_reflect_cpp_ubjson_without_field_names(
    benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::ubjson::write<rfl::NoFieldNames>(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_write_reflect_cpp_ubjson_without_field_names);

static void BM_canada_write_reflect_cpp_yaml(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::yaml::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_write_reflect_cpp_yaml);

// ----------------------------------------------------------------------------

}  // namespace canada_write

