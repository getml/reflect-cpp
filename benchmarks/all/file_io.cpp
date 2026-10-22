#include <benchmark/benchmark.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <rfl/io/load_bytes.hpp>
#include <rfl/io/load_string.hpp>
#include <stdexcept>
#include <string>

namespace file_io {

class InputFile {
 public:
  explicit InputFile(std::size_t size) {
    const auto stamp =
        std::chrono::steady_clock::now().time_since_epoch().count();
    for (unsigned i = 0; i < 100; ++i) {
      dir_ = std::filesystem::temp_directory_path() /
             ("reflectcpp-io-bench-" + std::to_string(stamp) + "-" +
              std::to_string(i));
      if (std::filesystem::create_directory(dir_)) {
        name_ = (dir_ / "input").string();
        std::ofstream out;
        out.exceptions(std::ios::badbit | std::ios::failbit);
        out.open(name_, std::ios::binary);
        const std::string data(size, 'x');
        out.write(data.data(), static_cast<std::streamsize>(data.size()));
        out.close();
        return;
      }
    }
    throw std::runtime_error("Cannot create benchmark directory");
  }
  ~InputFile() {
    std::error_code ec;
    std::filesystem::remove_all(dir_, ec);
  }
  const std::string& name() const { return name_; }

 private:
  std::filesystem::path dir_;
  std::string name_;
};

template <bool Binary>
void BM_load_file(benchmark::State& state) {
  const auto size = static_cast<std::size_t>(state.range(0));
  const InputFile file(size);
  const auto load = [&] {
    if constexpr (Binary)
      return rfl::io::load_bytes(file.name());
    else
      return rfl::io::load_string(file.name());
  };
  {
    const auto result = load();
    if (!result || result->size() != size) {
      state.SkipWithError("File loading failed");
      return;
    }
    for (const char c : *result) {
      if (c != 'x') {
        state.SkipWithError("File contents changed");
        return;
      }
    }
  }
  for (auto _ : state) {
    auto result = load();
    benchmark::DoNotOptimize(result);
  }
  state.SetBytesProcessed(state.iterations() * state.range(0));
}

BENCHMARK_TEMPLATE(BM_load_file, true)
    ->Arg(1024)
    ->Arg(1024 * 1024)
    ->Arg(32 * 1024 * 1024);
BENCHMARK_TEMPLATE(BM_load_file, false)
    ->Arg(1024)
    ->Arg(1024 * 1024)
    ->Arg(32 * 1024 * 1024);

}  // namespace file_io
