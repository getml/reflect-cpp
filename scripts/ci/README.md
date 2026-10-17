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

The image provides GCC 11–15, Clang 16–19, their dependency scanners, Ninja,
CMake 3.31.6, and several system dependencies. By default the runner uses the
repository's pinned vcpkg submodule, like CI. The source tree and build cache
remain on the host. GCC 15 comes from the Ubuntu toolchain test PPA.
First-time vcpkg dependency builds require network access.
CI also pins CMake 3.31.6 for benchmarks: the pinned vcpkg benchmark port passes
an unrecognized warning category that CMake 4.4 rejects during configuration.

```sh
python3 scripts/ci_matrix.py --container reflectcpp-ci \
  --compiler gcc-15 --compiler llvm-19 --format JSON --cxx 20 --modules ON

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

This project's module builds require GCC 15 or Clang 19 or newer. GCC 14
remains supported through headers: its CTRE module importer fails with
`Bad file data`. GCC 15 requires an ADL visibility workaround in the private
`rfl.ctre` dependency; CTRE is not re-exported by `rfl`.

Clang 18 and older remain in the header-build compatibility matrix. Clang 18 module importers fail
with PCH enabled, so that combination is no longer supported. Library and test
PCH remain enabled by default; `-DCMAKE_DISABLE_PRECOMPILE_HEADERS=ON` can disable
them explicitly. Module interface sources keep `SKIP_PRECOMPILE_HEADERS` because
their global module fragments must precede declarations.

With PCH enabled, GCC 15.2 (Ubuntu 24.04, C++20), GCC 15.3 (C++20 and C++23),
and GCC 16.2.1 (C++20) passed
the full JSON/CLI module suite: 342 tests passed and one locale test was skipped.

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
