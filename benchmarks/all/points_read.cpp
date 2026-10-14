#include <benchmark/benchmark.h>

#include <cstddef>
#include <cstdint>
#include <rfl/cereal.hpp>
#include <rfl/json.hpp>
#include <rfl/msgpack.hpp>
#include <rfl/yas.hpp>
#include <vector>

namespace points_read {

struct Point2d {
  float x;
  float y;

  bool operator==(const Point2d&) const = default;
};

using Points = std::vector<Point2d>;

static Points make_points(std::size_t count) {
  Points points;
  points.reserve(count);
  for (std::size_t i = 0; i < count; ++i) {
    // Exactly representable coordinates of both signs, varying across points.
    const auto x = static_cast<std::int32_t>(i % 65536) - 32768;
    const auto y = static_cast<std::int32_t>((i * 17) % 65536) - 32768;
    points.push_back(
        {static_cast<float>(x) * 0.125f, static_cast<float>(y) * 0.0625f});
  }
  return points;
}

template <class Write, class Read>
static void run(benchmark::State& state, Write write, Read read) {
  // Many small structs expose per-object parser overhead, including allocating
  // error storage on successful reads. Setup and validation are not timed.
  const auto points = make_points(static_cast<std::size_t>(state.range(0)));
  const auto data = write(points);
  {
    const auto result = read(data);
    if (!result) {
      state.SkipWithError(result.error().what());
      return;
    }
    if (*result != points) {
      state.SkipWithError("Point coordinates changed during round trip");
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

  state.SetItemsProcessed(state.iterations() * state.range(0));
  state.SetBytesProcessed(state.iterations() *
                          static_cast<std::int64_t>(data.size()));
  state.counters["serialized_bytes"] = static_cast<double>(data.size());
}

// DefaultIfMissing exercises the parser's default-value path. NoFieldNames
// separates field-name lookup overhead from positional struct parsing.
template <class... Ps>
static void BM_points_read_reflect_cpp_cereal(benchmark::State& state) {
  run(
      state,
      [](const Points& points) { return rfl::cereal::write<Ps...>(points); },
      [](const auto& data) { return rfl::cereal::read<Points, Ps...>(data); });
}
BENCHMARK_TEMPLATE(BM_points_read_reflect_cpp_cereal, rfl::Processors<>)
    ->Arg(1000)
    ->Arg(1000000)
    ->Unit(benchmark::kMillisecond);

template <class... Ps>
static void BM_points_read_reflect_cpp_json(benchmark::State& state) {
  run(
      state,
      [](const Points& points) { return rfl::json::write<Ps...>(points); },
      [](const auto& data) { return rfl::json::read<Points, Ps...>(data); });
}
BENCHMARK_TEMPLATE(BM_points_read_reflect_cpp_json, rfl::Processors<>)
    ->Arg(1000)
    ->Arg(1000000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_points_read_reflect_cpp_json, rfl::DefaultIfMissing)
    ->Arg(1000)
    ->Arg(1000000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_points_read_reflect_cpp_json, rfl::NoFieldNames)
    ->Arg(1000)
    ->Arg(1000000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_points_read_reflect_cpp_json, rfl::NoFieldNames,
                   rfl::DefaultIfMissing)
    ->Arg(1000)
    ->Arg(1000000)
    ->Unit(benchmark::kMillisecond);

template <class... Ps>
static void BM_points_read_reflect_cpp_msgpack(benchmark::State& state) {
  run(
      state,
      [](const Points& points) { return rfl::msgpack::write<Ps...>(points); },
      [](const auto& data) { return rfl::msgpack::read<Points, Ps...>(data); });
}
BENCHMARK_TEMPLATE(BM_points_read_reflect_cpp_msgpack, rfl::Processors<>)
    ->Arg(1000)
    ->Arg(1000000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_points_read_reflect_cpp_msgpack, rfl::DefaultIfMissing)
    ->Arg(1000)
    ->Arg(1000000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_points_read_reflect_cpp_msgpack, rfl::NoFieldNames)
    ->Arg(1000)
    ->Arg(1000000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_points_read_reflect_cpp_msgpack, rfl::NoFieldNames,
                   rfl::DefaultIfMissing)
    ->Arg(1000)
    ->Arg(1000000)
    ->Unit(benchmark::kMillisecond);

template <class... Ps>
static void BM_points_read_reflect_cpp_yas(benchmark::State& state) {
  run(
      state,
      [](const Points& points) { return rfl::yas::write<Ps...>(points); },
      [](const auto& data) { return rfl::yas::read<Points, Ps...>(data); });
}
BENCHMARK_TEMPLATE(BM_points_read_reflect_cpp_yas, rfl::Processors<>)
    ->Arg(1000)
    ->Arg(1000000)
    ->Unit(benchmark::kMillisecond);

}  // namespace points_read
