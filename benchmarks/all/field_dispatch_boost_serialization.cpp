#include <rfl/boost_serialization.hpp>

#include "field_dispatch.hpp"
namespace field_dispatch {
template <class Record>
void BM_field_dispatch_boost_serialization(benchmark::State& state) {
  run<Record>(
      state,
      [&](const auto& input) { return rfl::boost_serialization::write(input); },
      [&](const auto& bytes) {
        return rfl::boost_serialization::read<Data<Record>>(bytes);
      });
}
BENCHMARK_TEMPLATE(BM_field_dispatch_boost_serialization, Record2)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_field_dispatch_boost_serialization, Record16)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
}  // namespace field_dispatch
