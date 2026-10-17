#include <rfl/yas.hpp>

#include "parser_inlining.hpp"
namespace parser_inlining {
template <class Record>
void BM_parser_inlining_yas(benchmark::State& state) {
  run<Record>(
      state, [&](const auto& input) { return rfl::yas::write(input); },
      [&](const auto& bytes) { return rfl::yas::read<Data<Record>>(bytes); });
}
BENCHMARK_TEMPLATE(BM_parser_inlining_yas, Record2)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_parser_inlining_yas, Record16)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
}  // namespace parser_inlining
