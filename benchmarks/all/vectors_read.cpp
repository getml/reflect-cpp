#include <benchmark/benchmark.h>

#include <cstddef>
#include <cstdint>
#include <rfl/avro.hpp>
#include <rfl/boost_serialization.hpp>
#include <rfl/bson.hpp>
#include <rfl/capnproto.hpp>
#include <rfl/cbor.hpp>
#include <rfl/cereal.hpp>
#include <rfl/flexbuf.hpp>
#include <rfl/json.hpp>
#include <rfl/msgpack.hpp>
#include <rfl/toml.hpp>
#include <rfl/ubjson.hpp>
#include <rfl/xml.hpp>
#include <rfl/yaml.hpp>
#include <rfl/yas.hpp>
#include <utility>
#include <vector>

namespace vectors_read {

struct FloatVector {
  std::vector<float> values;

  bool operator==(const FloatVector&) const = default;
};

struct Data {
  std::vector<FloatVector> vectors;
};

static Data make_data(std::size_t count, std::size_t values_per_vector) {
  Data data;
  data.vectors.reserve(count);
  for (std::size_t i = 0; i < count; ++i) {
    FloatVector vector;
    vector.values.reserve(values_per_vector);
    for (std::size_t j = 0; j < values_per_vector; ++j) {
      const auto index = i * values_per_vector + j;
      const auto value = static_cast<std::int32_t>(index % 65536) - 32768;
      vector.values.push_back(static_cast<float>(value) * 0.125f);
    }
    data.vectors.push_back(std::move(vector));
  }
  return data;
}

template <class Write, class Read>
static void run(benchmark::State& state, Write write, Read read) {
  // Compare one large vector with many short vectors. The object wrappers
  // allow the same workload to be used with BSON, TOML and XML as well.
  const auto input = make_data(static_cast<std::size_t>(state.range(0)),
                               static_cast<std::size_t>(state.range(1)));
  const auto data = write(input);
  {
    const auto result = read(data);
    if (!result) {
      state.SkipWithError(result.error().what());
      return;
    }
    if (result->vectors != input.vectors) {
      state.SkipWithError("Vector values changed during round trip");
      return;
    }
  }

  for (auto _ : state) {
    auto result = read(data);
    if (!result) {
      state.SkipWithError(result.error().what());
      break;
    }
    benchmark::DoNotOptimize(result);
  }
  state.SetItemsProcessed(state.iterations() * state.range(0) * state.range(1));
  state.SetBytesProcessed(state.iterations() *
                          static_cast<std::int64_t>(data.size()));
  state.counters["serialized_bytes"] = static_cast<double>(data.size());
}

static void BM_vectors_read_reflect_cpp_avro(benchmark::State& state) {
  const auto schema = rfl::avro::to_schema<Data>();
  run(
      state,
      [&schema](const Data& value) { return rfl::avro::write(value, schema); },
      [&schema](const auto& data) {
        return rfl::avro::read<Data>(data, schema);
      });
}
BENCHMARK(BM_vectors_read_reflect_cpp_avro)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_boost_serialization(
    benchmark::State& state) {
  run(
      state,
      [](const Data& value) { return rfl::boost_serialization::write(value); },
      [](const auto& data) {
        return rfl::boost_serialization::read<Data>(data);
      });
}
BENCHMARK(BM_vectors_read_reflect_cpp_boost_serialization)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_bson(benchmark::State& state) {
  run(
      state, [](const Data& value) { return rfl::bson::write(value); },
      [](const auto& data) { return rfl::bson::read<Data>(data); });
}
BENCHMARK(BM_vectors_read_reflect_cpp_bson)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_capnproto(benchmark::State& state) {
  const auto schema = rfl::capnproto::to_schema<Data>();
  run(
      state,
      [&schema](const Data& value) {
        return rfl::capnproto::write(value, schema);
      },
      [&schema](const auto& data) {
        return rfl::capnproto::read<Data>(data, schema);
      });
}
BENCHMARK(BM_vectors_read_reflect_cpp_capnproto)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_cbor(benchmark::State& state) {
  run(
      state, [](const Data& value) { return rfl::cbor::write(value); },
      [](const auto& data) { return rfl::cbor::read<Data>(data); });
}
BENCHMARK(BM_vectors_read_reflect_cpp_cbor)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_cereal(benchmark::State& state) {
  run(
      state, [](const Data& value) { return rfl::cereal::write(value); },
      [](const auto& data) { return rfl::cereal::read<Data>(data); });
}
BENCHMARK(BM_vectors_read_reflect_cpp_cereal)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_flexbuf(benchmark::State& state) {
  run(
      state, [](const Data& value) { return rfl::flexbuf::write(value); },
      [](const auto& data) { return rfl::flexbuf::read<Data>(data); });
}
BENCHMARK(BM_vectors_read_reflect_cpp_flexbuf)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_json(benchmark::State& state) {
  run(
      state, [](const Data& value) { return rfl::json::write(value); },
      [](const auto& data) { return rfl::json::read<Data>(data); });
}
BENCHMARK(BM_vectors_read_reflect_cpp_json)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_msgpack(benchmark::State& state) {
  run(
      state, [](const Data& value) { return rfl::msgpack::write(value); },
      [](const auto& data) { return rfl::msgpack::read<Data>(data); });
}
BENCHMARK(BM_vectors_read_reflect_cpp_msgpack)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_toml(benchmark::State& state) {
  run(
      state, [](const Data& value) { return rfl::toml::write(value); },
      [](const auto& data) { return rfl::toml::read<Data>(data); });
}
BENCHMARK(BM_vectors_read_reflect_cpp_toml)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_ubjson(benchmark::State& state) {
  run(
      state, [](const Data& value) { return rfl::ubjson::write(value); },
      [](const auto& data) { return rfl::ubjson::read<Data>(data); });
}
BENCHMARK(BM_vectors_read_reflect_cpp_ubjson)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_xml(benchmark::State& state) {
  run(
      state, [](const Data& value) { return rfl::xml::write(value); },
      [](const auto& data) { return rfl::xml::read<Data>(data); });
}
BENCHMARK(BM_vectors_read_reflect_cpp_xml)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_yaml(benchmark::State& state) {
  run(
      state, [](const Data& value) { return rfl::yaml::write(value); },
      [](const auto& data) { return rfl::yaml::read<Data>(data); });
}
BENCHMARK(BM_vectors_read_reflect_cpp_yaml)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

static void BM_vectors_read_reflect_cpp_yas(benchmark::State& state) {
  run(
      state, [](const Data& value) { return rfl::yas::write(value); },
      [](const auto& data) { return rfl::yas::read<Data>(data); });
}
BENCHMARK(BM_vectors_read_reflect_cpp_yas)
    ->Args({1, 1000})
    ->Args({1, 1000000})
    ->Args({10000, 64})
    ->ArgNames({"vectors", "values"})
    ->Unit(benchmark::kMillisecond);

}  // namespace vectors_read
