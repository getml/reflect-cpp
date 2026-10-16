#ifndef BENCHMARK_FIELD_DISPATCH_HPP_
#define BENCHMARK_FIELD_DISPATCH_HPP_
#include <benchmark/benchmark.h>

#include <cstdint>
#include <vector>
namespace field_dispatch {
struct Record2 {
  float field0 = 0;
  float field1 = 1;
  bool operator==(const Record2&) const = default;
};
struct Record16 {
  float field0 = 0;
  float field1 = 1;
  float field2 = 2;
  float field3 = 3;
  float field4 = 4;
  float field5 = 5;
  float field6 = 6;
  float field7 = 7;
  float field8 = 8;
  float field9 = 9;
  float field10 = 10;
  float field11 = 11;
  float field12 = 12;
  float field13 = 13;
  float field14 = 14;
  float field15 = 15;
  bool operator==(const Record16&) const = default;
};
template <class Record>
struct Data {
  std::vector<Record> records;
  bool operator==(const Data&) const = default;
};
template <class Record>
auto make_data(std::size_t size) {
  Data<Record> data;
  data.records.resize(size);
  for (std::size_t i = 0; i < size; ++i) {
    data.records[i].field0 = static_cast<float>(i) * 0.5f;
    data.records[i].field1 = -static_cast<float>(i) * 0.25f;
  }
  return data;
}
template <class Record, class Write, class Read>
void run(benchmark::State& state, Write write, Read read) {
  const auto input = make_data<Record>(state.range(0));
  const auto bytes = write(input);
  {
    const auto result = read(bytes);
    if (!result) {
      state.SkipWithError(result.error().what());
      return;
    }
    if (*result != input) {
      state.SkipWithError("Fields changed during round trip");
      return;
    }
  }
  for (auto _ : state) {
    auto result = read(bytes);
    if (!result) {
      state.SkipWithError(result.error().what());
      break;
    }
    benchmark::DoNotOptimize(result);
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
  state.SetBytesProcessed(state.iterations() *
                          static_cast<std::int64_t>(bytes.size()));
  state.counters["serialized_bytes"] = static_cast<double>(bytes.size());
}
}  // namespace field_dispatch
#endif
