# Local Linux CI

`scripts/ci_matrix.py` expands the axes and exclusions in
`.github/workflows/linux.yaml`. Each job has its own build directory and log under
`build/local-ci`; the last invocation writes `results.json`. Failed jobs do not
stop the remaining jobs. The runner returns a nonzero exit code on any failure.

Install Python 3 and PyYAML on the host. List jobs without building:

```sh
python3 scripts/ci_matrix.py --list
python3 scripts/ci_matrix.py --list --modules ON
```

## Ubuntu 24.04 container

Build once with Podman (or replace `podman` with `docker`):

```sh
podman build -f scripts/ci/Containerfile -t reflectcpp-ci .
git submodule update --init --recursive
```

The image provides GCC 11–16, Clang 16–19, their dependency scanners, Ninja,
CMake 3.31.6, and several system dependencies. By default the runner uses the
repository's pinned vcpkg submodule, like CI. The source tree and build cache
remain on the host. GCC 16 comes from the Ubuntu toolchain test PPA.
First-time vcpkg dependency builds require network access.
CI also pins CMake 3.31.6 for benchmarks: the pinned vcpkg benchmark port passes
an unrecognized warning category that CMake 4.4 rejects during configuration.

```sh
python3 scripts/ci_matrix.py --container reflectcpp-ci \
  --compiler gcc-16 --compiler llvm-19 --format JSON --cxx 20 --modules ON

# Original header compatibility matrix, followed by module-capable jobs:
python3 scripts/ci_matrix.py --container reflectcpp-ci
python3 scripts/ci_matrix.py --container reflectcpp-ci --modules ON
```

Use `--runtime docker` for Docker. Builds use four parallel tasks by default;
use `--jobs 2` on machines with limited memory. The matrix runs jobs sequentially
to bound memory usage and share dependency caches.

## Native compiler or system dependencies

```sh
python3 scripts/ci_matrix.py --compiler llvm-19 --format YAML --cxx 20 \
  --modules ON --system-deps
```

For testing a newer native compiler against a selected CI configuration:

```sh
python3 scripts/ci_matrix.py --compiler llvm-19 --cxx-executable clang++ \
  --format JSON --cxx 20 --modules ON --system-deps
```

The log and JSON report record the actual compiler. Such overrides and system
packages are useful for debugging, but do not reproduce the original compiler
and dependency versions. Missing compilers fail by default; `--allow-missing`
records them as skipped instead. Windows/MSVC and AppleClang jobs need their
respective operating systems and are not emulated by this Linux runner.

## Module support

Enable module interfaces with `-DREFLECTCPP_BUILD_MODULES=ON`. Header builds are
the default. CMake generates test adapters under `<build>/tests/test_rfl` containing either
imports or includes, so both modes exercise the same test bodies without test
preprocessor switches. C arrays/inheritance use
their own module because their macro changes the compiled declarations.

CMake's scanning floor is GCC 14, Clang 16, or MSVC 14.34, with Ninja 1.11+ or a
supported Visual Studio generator. These are build-system requirements, not a
guarantee that a compiler implements every module feature or can compile this
library and its dependencies. In particular, `import std` has different
requirements and is not used by these module interfaces.

This project's module builds require GCC 16 or Clang 19 or newer. GCC 15 and
older remain supported through headers. GCC 14's CTRE module importer fails with
`Bad file data`; GCC 15's native selective exports can reject valid regex matches
at runtime. The private `rfl.ctre` dependency uses CTRE's native `CTRE_IN_A_MODULE`,
`CTLL_IN_A_MODULE`, and `UNICODE_DB_IN_A_MODULE` export macros, with no GCC-specific
export branch. CTRE is not re-exported by `rfl`.

The failure is in recursive `ctre::evaluate` overload lookup, not regex parsing:
an overload defined earlier calls `evaluate` with a dependent stack type, so
later overloads must be found by ADL. GCC 15.2 and 15.3 omit non-exported
module-linkage candidates from that instantiation context and select CTRE's
ellipsis fallback, which returns `not_matched`. A reduced module without CTRE
reproduces this at `-O0`; an earlier declaration or an explicit export restores
the intended overload. GCC 16 and Clang 19 pass the same reduced case. This
matches GCC [PR117658's ADL fix](https://gcc.gnu.org/pipermail/gcc-patches/2025-September/694072.html).
Exporting only the evaluator overloads in a temporary CTRE copy also fixes the
isolated matching tests. The supported GCC floor is now 16, so neither CTRE
patches nor the former full-header export workaround are needed.

Clang 18 and older remain in the header-build compatibility matrix. Clang 18 module importers fail
with PCH enabled, so that combination is no longer supported. Library and test
PCH remain enabled by default; `-DCMAKE_DISABLE_PRECOMPILE_HEADERS=ON` can disable
them explicitly. Module interface sources keep `SKIP_PRECOMPILE_HEADERS` because
their global module fragments must precede declarations.

With native CTRE exports and PCH enabled, the Ubuntu toolchain PPA's GCC 16.0.1
snapshot passed the C++20 JSON, CLI, CBOR, UBJSON, Cereal, CSV, Parquet and
Cap'n Proto module suites: 574 tests passed, one locale test was skipped, and
none failed. Focused GCC 16.2.1 importer checks also passed valid and invalid
custom regex and email cases in C++20 and C++23.

The following debugging results include historical GCC 15 module configurations
using the former export workaround; those configurations are no longer supported.

With PCH enabled, GCC 15.2 (Ubuntu 24.04, C++20), GCC 15.3 (C++20 and C++23),
and GCC 16.2.1 (C++20) passed
the full JSON/CLI module suite: 342 tests passed and one locale test was skipped.

GCC 15.2 also passed the MessagePack, XML (pugixml 1.15), and YAML module
suites in C++20 and C++23. Their combined shared-library header build passed
200 tests, with one locale test skipped. Clang 19 passed the Parquet module
suite with Arrow 21.0.0 (61 tests passed, one locale test skipped).

Benchmark PCH contains standard headers in module mode. `rfl.hpp` stays in
the PCH only in header mode, because CMake does not scan the PCH itself for
module imports. JSON benchmarks compiled and the reflect-cpp person benchmark
ran with PCH enabled on both GCC 15 and Clang 19.

The CBOR and UBJSON adapters now keep jsoncons 1.4.0 decoding, primitive
access, and encoder ownership in ordinary implementation files. Module
interfaces retain only forward declarations of the pointer types. Cereal 1.3.2
uses the same boundary for archive ownership and primitive operations, retaining
its native portable binary format and existing archive-reference overloads.
Regression tests cover those overloads against a separate native Cereal TU,
byte compatibility, stream reads/writes, and malformed input.

CSV and Parquet module interfaces now include Arrow forward declarations.
Native builders, array conversions and file I/O live in ordinary implementation
files. `to_buffer` retains its Arrow-buffer return type. String and null appends
are also isolated from Arrow's complete builder hierarchy to avoid GCC 16's
incorrect speculative devirtualization. No compiler warning or optimization is
disabled. The private native conversion code preserves numerical casts, nulls,
binary data and timestamp units.

Cap'n Proto 1.2.0 uses opaque native-view handles and ordinary packed-message
owners. Native calls leave the exported templates, and the schema cache has
inline linkage. The byte/stream read/write entry points and schema API remain;
low-level `val_` fields now hold handles, with `.as<T>()` for custom constructors
and `.native()` for direct integration. Handle ownership adds allocations; these
checks establish functional compatibility, not performance equivalence.

Separate native Arrow/Parquet and Cap'n Proto translation units verify both
read directions. Cap'n Proto checks exact bytes; Parquet checks numerical
conversion, nanosecond timestamps, binary data, nulls, builder growth and multiple
row groups. All local runs below keep PCH enabled and the usual optimization
and warning flags. A locale test is skipped because its locale is unavailable.

| Compiler | Standard and mode | Formats | Passed | Skipped |
| --- | --- | --- | --- | --- |
| GCC 15.2 | C++20 modules | CBOR, UBJSON, Cereal, CSV, Parquet, Cap'n Proto | 278 | 1 |
| GCC 15.2 | C++23 modules | CSV, Parquet, Cap'n Proto | 130 | 1 |
| GCC 16.2.1 | C++20 modules | Parquet | 63 | 1 |
| GCC 16.2.1 | C++20 modules | Cap'n Proto | 98 | 1 |
| Clang 19 | C++20 modules | All six formats above | 278 | 1 |
| GCC 11 | C++20 headers, shared library | All six formats above | 278 | 1 |

GCC 15.2 and GCC 16.2.1 also passed the CBOR, UBJSON and Cereal module suites
in C++23. An installed GCC 11 shared-library consumer exercises all six formats
through `find_package(reflectcpp)`; the local PyArrow fixture needs its library
directory in `LD_LIBRARY_PATH`.

Sources: [CMake 3.31](https://cmake.org/cmake/help/v3.31/manual/cmake-cxxmodules.7.html),
[GCC 14](https://gcc.gnu.org/onlinedocs/gcc-14.3.0/gcc/C_002b_002b-Modules.html),
[Clang status](https://clang.llvm.org/cxx_status.html).

## Compare PCH and modules

Build the optional Clang 20–22 toolchains alongside Clang 19, then run the same JSON module importer
with PCH explicitly enabled and disabled for each compiler:

```sh
podman build -f scripts/ci/Containerfile --build-arg 'EXTRA_CLANG_VERSIONS=20 21 22' -t reflectcpp-pch-ci .
podman run --rm --userns=keep-id -v "$PWD:/work" -w /work reflectcpp-pch-ci \
  python3 scripts/check_pch_modules.py
```

Repeat `--compiler clang++-19` (or another supported installed executable) to select
versions. Results, compiler versions, compile commands, and individual logs are
written to `build/pch-matrix`. This is a focused compilation check for the PCH
failure, not the full test matrix; add `--pch ON --full` to also build and run
the complete JSON/CLI tests.

Verified on Ubuntu 24.04 with libstdc++ 14, CMake 3.31.6, and C++20:

| Clang | PCH ON/OFF importer check | Full JSON/CLI tests with PCH ON |
| --- | --- | --- |
| 19.1.1 | Both passed | 341 passed, 1 locale test skipped |
| 20.1.8 | Both passed | 341 passed, 1 locale test skipped |
| 21.1.8 | Both passed | 341 passed, 1 locale test skipped |
| 22.1.8 | Both passed | 341 passed, 1 locale test skipped |
