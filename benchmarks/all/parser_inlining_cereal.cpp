#include <rfl/FailFast.hpp>
#include <rfl/cereal.hpp>

#include "parser_inlining.hpp"
namespace parser_inlining {
template <class Record, bool FailFast = false>
void BM_parser_inlining_cereal(benchmark::State& state) {
  run<Record>(
      state, [&](const auto& input) { return rfl::cereal::write(input); },
      [&](const auto& bytes) {
        if constexpr (FailFast) {
          return rfl::cereal::read<Data<Record>, rfl::FailFast>(bytes);
        } else {
          return rfl::cereal::read<Data<Record>>(bytes);
        }
      });
}
BENCHMARK_TEMPLATE(BM_parser_inlining_cereal, Record2)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_parser_inlining_cereal, Record16)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_parser_inlining_cereal, Record2, true)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_parser_inlining_cereal, Record16, true)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
}  // namespace parser_inlining
