#include <rfl/capnproto.hpp>

#include "field_dispatch.hpp"
namespace field_dispatch {
template <class Record>
void BM_field_dispatch_capnproto(benchmark::State& state) {
  const auto schema = rfl::capnproto::to_schema<Data<Record>>();
  run<Record>(
      state,
      [&](const auto& input) { return rfl::capnproto::write(input, schema); },
      [&](const auto& bytes) {
        return rfl::capnproto::read<Data<Record>>(bytes, schema);
      });
}
BENCHMARK_TEMPLATE(BM_field_dispatch_capnproto, Record2)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_field_dispatch_capnproto, Record16)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond);
}  // namespace field_dispatch
