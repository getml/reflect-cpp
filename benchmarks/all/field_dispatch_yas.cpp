#include <rfl/yas.hpp>

#include "field_dispatch.hpp"
namespace field_dispatch {
template <class Record>
void BM_field_dispatch_yas(benchmark::State& state) {
  run<Record>(
      state, [&](const auto& input) { return rfl::yas::write(input); },
      [&](const auto& bytes) { return rfl::yas::read<Data<Record>>(bytes); });
}
BENCHMARK_TEMPLATE(BM_field_dispatch_yas, Record2)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_field_dispatch_yas, Record16)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
}  // namespace field_dispatch
