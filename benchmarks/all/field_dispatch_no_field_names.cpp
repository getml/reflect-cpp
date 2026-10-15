#include <rfl/json.hpp>
#include <rfl/msgpack.hpp>

#include "field_dispatch.hpp"
namespace field_dispatch {
template <class Record, class... Ps>
void BM_field_dispatch_json_no_field_names(benchmark::State& state) {
  run<Record>(
      state,
      [](const auto& input) {
        return rfl::json::write<rfl::NoFieldNames, Ps...>(input);
      },
      [](const auto& bytes) {
        return rfl::json::read<Data<Record>, rfl::NoFieldNames, Ps...>(bytes);
      });
}
BENCHMARK_TEMPLATE(BM_field_dispatch_json_no_field_names, Record2)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_field_dispatch_json_no_field_names, Record2,
                   rfl::DefaultIfMissing)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_field_dispatch_json_no_field_names, Record16)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_field_dispatch_json_no_field_names, Record16,
                   rfl::DefaultIfMissing)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
template <class Record, class... Ps>
void BM_field_dispatch_msgpack_no_field_names(benchmark::State& state) {
  run<Record>(
      state,
      [](const auto& input) {
        return rfl::msgpack::write<rfl::NoFieldNames, Ps...>(input);
      },
      [](const auto& bytes) {
        return rfl::msgpack::read<Data<Record>, rfl::NoFieldNames, Ps...>(
            bytes);
      });
}
BENCHMARK_TEMPLATE(BM_field_dispatch_msgpack_no_field_names, Record2)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_field_dispatch_msgpack_no_field_names, Record2,
                   rfl::DefaultIfMissing)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_field_dispatch_msgpack_no_field_names, Record16)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_field_dispatch_msgpack_no_field_names, Record16,
                   rfl::DefaultIfMissing)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
}  // namespace field_dispatch
