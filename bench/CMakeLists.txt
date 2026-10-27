#=============================================================================#
# Google benchmark dep:                                                       #
#=============================================================================#

INCLUDE(FetchContent)

FetchContent_DECLARE(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.10.0
)

# Don't pull googletest, don't build benchmark's own tests, don't install:
SET(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
SET(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
SET(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FETCHCONTENT_MAKEAVAILABLE(benchmark)

#=============================================================================#
# Benchmarks:                                                                 #
#=============================================================================#
ADD_EXECUTABLE(bench_csv_load bench_csv_load.cpp)

TARGET_COMPILE_OPTIONS(bench_csv_load PRIVATE ${CXXFLAGS})
TARGET_LINK_LIBRARIES(bench_csv_load PRIVATE benchmark::benchmark quant_backtest)
