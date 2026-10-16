#include <rfl/json.hpp>

#include "parser_inlining.hpp"
namespace parser_inlining {
template <class Record>
void BM_parser_inlining_json(benchmark::State& state) {
  run<Record>(
      state, [&](const auto& input) { return rfl::json::write(input); },
      [&](const auto& bytes) { return rfl::json::read<Data<Record>>(bytes); });
}
BENCHMARK_TEMPLATE(BM_parser_inlining_json, Record2)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_parser_inlining_json, Record16)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
}  // namespace parser_inlining
