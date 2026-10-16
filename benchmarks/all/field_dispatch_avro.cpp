#include <rfl/avro.hpp>

#include "field_dispatch.hpp"
namespace field_dispatch {
template <class Record>
void BM_field_dispatch_avro(benchmark::State& state) {
  const auto schema = rfl::avro::to_schema<Data<Record>>();
  run<Record>(
      state, [&](const auto& input) { return rfl::avro::write(input, schema); },
      [&](const auto& bytes) {
        return rfl::avro::read<Data<Record>>(bytes, schema);
      });
}
BENCHMARK_TEMPLATE(BM_field_dispatch_avro, Record2)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_field_dispatch_avro, Record16)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
}  // namespace field_dispatch
