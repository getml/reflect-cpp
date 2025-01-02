#include <benchmark/benchmark.h>

#include <array>
#include <iostream>
#include <optional>
#include <rfl/avro.hpp>
#include <rfl/bson.hpp>
#include <rfl/capnproto.hpp>
#include <rfl/cbor.hpp>
#include <rfl/flexbuf.hpp>
#include <rfl/json.hpp>
#include <rfl/msgpack.hpp>
#include <rfl/toml.hpp>
#include <rfl/ubjson.hpp>
#include <rfl/yaml.hpp>
#include <type_traits>
#include <vector>

namespace canada_read {

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

static void BM_canada_read_reflect_cpp_avro(benchmark::State &state) {
  const auto schema = rfl::avro::to_schema<FeatureCollection>();
  const auto data = rfl::avro::write(load_data(), schema);
  for (auto _ : state) {
    const auto res = rfl::avro::read<FeatureCollection>(data, schema);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_avro);

static void BM_canada_read_reflect_cpp_bson(benchmark::State &state) {
  const auto data = rfl::bson::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::bson::read<FeatureCollection>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_bson);

static void BM_canada_read_reflect_cpp_capnproto(benchmark::State &state) {
  const auto schema = rfl::capnproto::to_schema<FeatureCollection>();
  const auto data = rfl::capnproto::write(load_data(), schema);
  for (auto _ : state) {
    const auto res = rfl::capnproto::read<FeatureCollection>(data, schema);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_capnproto);

static void BM_canada_read_reflect_cpp_cbor(benchmark::State &state) {
  const auto data = rfl::cbor::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::cbor::read<FeatureCollection>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_cbor);

static void BM_canada_read_reflect_cpp_cbor_without_field_names(
    benchmark::State &state) {
  const auto data = rfl::cbor::write<rfl::NoFieldNames>(load_data());
  for (auto _ : state) {
    const auto res =
        rfl::cbor::read<FeatureCollection, rfl::NoFieldNames>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_cbor_without_field_names);

static void BM_canada_read_reflect_cpp_flexbuf(benchmark::State &state) {
  const auto data = rfl::flexbuf::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::flexbuf::read<FeatureCollection>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_flexbuf);

static void BM_canada_read_reflect_cpp_flexbuf_without_field_names(
    benchmark::State &state) {
  const auto data = rfl::flexbuf::write<rfl::NoFieldNames>(load_data());
  for (auto _ : state) {
    const auto res =
        rfl::flexbuf::read<FeatureCollection, rfl::NoFieldNames>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_flexbuf_without_field_names);

static void BM_canada_read_reflect_cpp_json(benchmark::State &state) {
  const auto data = rfl::json::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::json::read<FeatureCollection>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_json);

static void BM_canada_read_reflect_cpp_json_without_field_names(
    benchmark::State &state) {
  const auto data = rfl::json::write<rfl::NoFieldNames>(load_data());
  for (auto _ : state) {
    const auto res =
        rfl::json::read<FeatureCollection, rfl::NoFieldNames>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_json_without_field_names);

static void BM_canada_read_reflect_cpp_msgpack(benchmark::State &state) {
  const auto data = rfl::msgpack::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::msgpack::read<FeatureCollection>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_msgpack);

static void BM_canada_read_reflect_cpp_msgpack_without_field_names(
    benchmark::State &state) {
  const auto data = rfl::msgpack::write<rfl::NoFieldNames>(load_data());
  for (auto _ : state) {
    const auto res =
        rfl::msgpack::read<FeatureCollection, rfl::NoFieldNames>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_msgpack_without_field_names);

static void BM_canada_read_reflect_cpp_toml(benchmark::State &state) {
  const auto data = rfl::toml::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::toml::read<FeatureCollection>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_toml);

static void BM_canada_read_reflect_cpp_ubjson(benchmark::State &state) {
  const auto data = rfl::ubjson::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::ubjson::read<FeatureCollection>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_ubjson);

static void BM_canada_read_reflect_cpp_ubjson_without_field_names(
    benchmark::State &state) {
  const auto data = rfl::ubjson::write<rfl::NoFieldNames>(load_data());
  for (auto _ : state) {
    const auto res =
        rfl::ubjson::read<FeatureCollection, rfl::NoFieldNames>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_ubjson_without_field_names);

static void BM_canada_read_reflect_cpp_yaml(benchmark::State &state) {
  const auto data = rfl::yaml::write(load_data());
  for (auto _ : state) {
    const auto res = rfl::yaml::read<FeatureCollection>(data);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_read_reflect_cpp_yaml);

// ----------------------------------------------------------------------------

}  // namespace canada_read

