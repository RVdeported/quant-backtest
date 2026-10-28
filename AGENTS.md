# AGENTS.md — quant-backtest

C++26 backtesting framework for quant trading strategies. Data flows through a
unified `DataSource` interface (CSV + ClickHouse backends); strategies are
streaming/batch-oriented. Architecture decisions are recorded as ADRs
(Architecture Decision Records) in `requirements.md` — treat that file as the
design source of truth before implementing anything new.

## Dev environment

- Linux/UNIX only (`CMakeLists.txt` hard-fails on non-UNIX).
- C++26, GCC 15 (`g++-15`), CMake >= 3.23, Ninja.
- Deps (GoogleTest v1.15.2, Google Benchmark v1.10.0) are pulled via
  `FetchContent`, no manual install needed.

## Build & test

Tests and benchmarks are OFF by default — enable them explicitly.

    cmake -B build -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_COMPILER=g++-15 \
      -DBUILD_TESTS=ON \
      -DBUILD_BENCH=ON
    cmake --build build --parallel "$(nproc)"
    ctest --test-dir build --output-on-failure

Benchmark (write JSON to benchmark.json):

    ./build/bench/bench_csv_load --benchmark_out=benchmark.json --benchmark_out_format=json

Docs (Doxygen → `html/`):

    doxygen Doxyfile

These mirror `.github/workflows/ci.yml`, which runs on every push/PR to
`master` and every `v*` tag (build+test+bench, Doxygen → GitHub Pages, release).

## Layout & conventions

- `include/quant_backtest/*.hpp` — public headers; `src/*.cpp` — implementation.
  All code in namespace `qbt`.
- Library target `quant_backtest` (static); executable target `main`
  (`main.cpp` is a placeholder "Hello, world!" — not a real entry point yet).
- New test: add `test/test_<name>.cpp`, then append `<name>` to the
  `FOREACH(TEST base csv_data_source)` list in `test/CMakeLists.txt`.
  Tests use GoogleTest (`TEST`, `EXPECT_*`), file-scope `namespace {}` for
  helpers.
- New benchmark: add `bench/bench_<name>.cpp` and register it explicitly in
  `bench/CMakeLists.txt` (no FOREACH loop there).
- `CXXFLAGS` (`-std=c++26`) is set once at top level and threaded into every
  target via `TARGET_COMPILE_OPTIONS(... PRIVATE ${CXXFLAGS})` — new targets
  must add that line too.
- clang-format (`.clang-format`): 2-space indent, Allman braces, 80-col limit,
  `PointerAlignment: Middle`, no namespace indentation. Doxygen `//!` comments
  on public APIs.

## Pitfalls

- `requirements.md` ADRs are in Russian and all statuses are still `Предложено`
  (Proposed) — schemas/types may change as decisions get accepted.
- `utxx::time_val` is the canonical time type (per ADR-0004), NOT raw ms.
- `compile_commands.json` is a symlink to `build/Debug/compile_commands.json`;
  configure a Debug build (or `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`) for clangd.
- `CsvDataSource` is a stub: `load()` reads the whole file into one string, no
  parsing/typing yet (ADR-0003) and no ClickHouse ingest (ADR-0007).
- `.gitignore` drops `*.json`, `build/`, `html/`, `latex/` — don't hand-edit
  generated files under those paths.
