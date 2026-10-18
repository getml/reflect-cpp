#include <benchmark/benchmark.h>

#include <cstddef>
#include <cstdint>
#include <rfl/boost_serialization.hpp>
#include <rfl/cereal.hpp>
#include <rfl/yas.hpp>
#include <sstream>
#include <string>
#include <vector>

namespace bulk_numeric_vectors {

struct Point2 {
  double x;
  double y;

  bool operator==(const Point2&) const = default;
};

struct Camera {
  std::vector<Point2> keypoints;
  std::vector<std::uint64_t> landmark_ids;

  bool operator==(const Camera&) const = default;
};

struct World {
  std::vector<Camera> cameras;

  bool operator==(const World&) const = default;
};

static Point2 make_point(std::size_t index) {
  return {
      static_cast<double>(static_cast<std::int64_t>(index % 65536) - 32768) *
          0.125,
      static_cast<double>(static_cast<std::int64_t>((index * 17) % 65536) -
                          32768) *
          0.25};
}

static std::uint64_t make_id(std::size_t index) {
  return static_cast<std::uint64_t>(index) * UINT64_C(0x9e3779b97f4a7c15) +
         UINT64_C(0x123456789abcdef0);
}

static std::vector<std::uint64_t> make_ids(std::size_t count) {
  std::vector<std::uint64_t> values(count);
  for (std::size_t i = 0; i < count; ++i) values[i] = make_id(i);
  return values;
}

static std::vector<Point2> make_points(std::size_t count) {
  std::vector<Point2> values(count);
  for (std::size_t i = 0; i < count; ++i) values[i] = make_point(i);
  return values;
}

static World make_world(std::size_t cameras, std::size_t points_per_camera) {
  World world;
  world.cameras.resize(cameras);
  for (std::size_t i = 0; i < cameras; ++i) {
    auto& camera = world.cameras[i];
    camera.keypoints.resize(points_per_camera);
    camera.landmark_ids.resize(points_per_camera);
    for (std::size_t j = 0; j < points_per_camera; ++j) {
      const auto index = i * points_per_camera + j;
      camera.keypoints[j] = make_point(index);
      camera.landmark_ids[j] = make_id(index);
    }
  }
  return world;
}

enum class Format { cereal, yas, boost_serialization };

template <Format F, class T>
void write(const T& input, std::ostream& stream) {
  if constexpr (F == Format::cereal)
    rfl::cereal::write(input, stream);
  else if constexpr (F == Format::yas)
    rfl::yas::write(input, stream);
  else
    rfl::boost_serialization::write(input, stream);
}

template <Format F, class T>
auto read(const std::string& bytes) {
  if constexpr (F == Format::yas) {
    return rfl::yas::read<T>(bytes);
  } else {
    std::istringstream stream(bytes);
    if constexpr (F == Format::cereal)
      return rfl::cereal::read<T>(stream);
    else
      return rfl::boost_serialization::read<T>(stream);
  }
}

template <bool Read, Format F, class T>
static void run(benchmark::State& state, const T& input, std::int64_t items) {
  std::ostringstream output;
  write<F>(input, output);
  const auto bytes = output.str();
  {
    const auto result = read<F, T>(bytes);
    if (!result) {
      state.SkipWithError(result.error().what());
      return;
    }
    if (*result != input) {
      state.SkipWithError("Binary round trip changed the input");
      return;
    }
  }

  // Stream construction, serialization, and destruction are timed. Fixture
  // generation, output extraction, and equality checks are outside the loop.
  for (auto _ : state) {
    if constexpr (Read) {
      auto result = read<F, T>(bytes);
      if (!result) {
        state.SkipWithError(result.error().what());
        break;
      }
      benchmark::DoNotOptimize(result);
    } else {
      std::ostringstream stream;
      write<F>(input, stream);
      benchmark::DoNotOptimize(stream);
    }
  }
  state.SetItemsProcessed(state.iterations() * items);
  state.SetBytesProcessed(state.iterations() *
                          static_cast<std::int64_t>(bytes.size()));
  state.counters["serialized_bytes"] = static_cast<double>(bytes.size());
}

template <bool Read, Format F>
static void BM_bulk_numeric_vectors_ids(benchmark::State& state) {
  const auto input = make_ids(static_cast<std::size_t>(state.range(0)));
  run<Read, F>(state, input, state.range(0));
}

template <bool Read, Format F>
static void BM_bulk_numeric_vectors_points(benchmark::State& state) {
  const auto input = make_points(static_cast<std::size_t>(state.range(0)));
  run<Read, F>(state, input, state.range(0));
}

template <bool Read, Format F>
static void BM_bulk_numeric_vectors_complete(benchmark::State& state) {
  const auto input = make_world(static_cast<std::size_t>(state.range(0)),
                                static_cast<std::size_t>(state.range(1)));
  run<Read, F>(state, input, state.range(0) * state.range(1));
}

BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_ids, false, Format::cereal)
    ->Name("bulk_numeric_vectors/cereal/ids/write")
    ->Arg(1500000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_ids, true, Format::cereal)
    ->Name("bulk_numeric_vectors/cereal/ids/read")
    ->Arg(1500000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_points, false, Format::cereal)
    ->Name("bulk_numeric_vectors/cereal/points/write")
    ->Arg(1500000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_points, true, Format::cereal)
    ->Name("bulk_numeric_vectors/cereal/points/read")
    ->Arg(1500000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_complete, false, Format::cereal)
    ->Name("bulk_numeric_vectors/cereal/complete/write")
    ->Args({500, 3000})
    ->ArgNames({"cameras", "keypoints"})
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_complete, true, Format::cereal)
    ->Name("bulk_numeric_vectors/cereal/complete/read")
    ->Args({500, 3000})
    ->ArgNames({"cameras", "keypoints"})
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_ids, false, Format::yas)
    ->Name("bulk_numeric_vectors/yas/ids/write")
    ->Arg(1500000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_ids, true, Format::yas)
    ->Name("bulk_numeric_vectors/yas/ids/read")
    ->Arg(1500000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_points, false, Format::yas)
    ->Name("bulk_numeric_vectors/yas/points/write")
    ->Arg(1500000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_points, true, Format::yas)
    ->Name("bulk_numeric_vectors/yas/points/read")
    ->Arg(1500000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_complete, false, Format::yas)
    ->Name("bulk_numeric_vectors/yas/complete/write")
    ->Args({500, 3000})
    ->ArgNames({"cameras", "keypoints"})
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_complete, true, Format::yas)
    ->Name("bulk_numeric_vectors/yas/complete/read")
    ->Args({500, 3000})
    ->ArgNames({"cameras", "keypoints"})
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_ids, false,
                   Format::boost_serialization)
    ->Name("bulk_numeric_vectors/boost_serialization/ids/write")
    ->Arg(1500000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_ids, true,
                   Format::boost_serialization)
    ->Name("bulk_numeric_vectors/boost_serialization/ids/read")
    ->Arg(1500000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_points, false,
                   Format::boost_serialization)
    ->Name("bulk_numeric_vectors/boost_serialization/points/write")
    ->Arg(1500000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_points, true,
                   Format::boost_serialization)
    ->Name("bulk_numeric_vectors/boost_serialization/points/read")
    ->Arg(1500000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_complete, false,
                   Format::boost_serialization)
    ->Name("bulk_numeric_vectors/boost_serialization/complete/write")
    ->Args({500, 3000})
    ->ArgNames({"cameras", "keypoints"})
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_bulk_numeric_vectors_complete, true,
                   Format::boost_serialization)
    ->Name("bulk_numeric_vectors/boost_serialization/complete/read")
    ->Args({500, 3000})
    ->ArgNames({"cameras", "keypoints"})
    ->Unit(benchmark::kMillisecond);

}  // namespace bulk_numeric_vectors
