#include <rfl/msgpack.hpp>

#include "parser_inlining.hpp"
namespace parser_inlining {
template <class Record>
void BM_parser_inlining_msgpack(benchmark::State& state) {
  run<Record>(
      state, [&](const auto& input) { return rfl::msgpack::write(input); },
      [&](const auto& bytes) {
        return rfl::msgpack::read<Data<Record>>(bytes);
      });
}
BENCHMARK_TEMPLATE(BM_parser_inlining_msgpack, Record2)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_parser_inlining_msgpack, Record16)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
}  // namespace parser_inlining
