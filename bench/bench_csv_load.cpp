//===========================================================================//
//                       "bench_csv_load.cpp":                                //
//===========================================================================//
// Benchmark: CsvDataSource::load() — reading a CSV file from disk.           //
//                                                                            //
// Dummy data: synthetic `trades` rows (ADR-0003). Each run writes a temp     //
// file in SetUp, then measures how long it takes to load it into memory.     //
//                                                                            //
// Caveat: after the first iteration the file is in the OS page cache, so     //
// subsequent iterations measure warm-cache reads, not true cold disk I/O.    //
// For cold measurements either drop caches (needs root) or load each file    //
// only once.                                                                 //
//===========================================================================//

#include <benchmark/benchmark.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "quant_backtest/csv_data_source.hpp"

namespace
{

std::string make_trade_line(std::int64_t id)
{
  return std::to_string(id) + ",0.12345678,1.50000000,1699999999.123456789,1\n";
}

void write_dummy_trades(const std::filesystem::path& path, std::size_t rows)
{
  std::ofstream out(path, std::ios::binary | std::ios::trunc);
  for (std::size_t i = 0; i < rows; ++i) {
    out << make_trade_line(static_cast<std::int64_t>(i));
  }
}

class CsvLoadFixture : public benchmark::Fixture
{
public:
  void SetUp(const benchmark::State& state) override
  {
    rows_      = static_cast<std::size_t>(state.range(0));
    path_      = std::filesystem::temp_directory_path() /
                 ("qbt_bench_trades_" + std::to_string(rows_) + ".csv");
    write_dummy_trades(path_, rows_);
    file_size_ = std::filesystem::file_size(path_);
    src_       = std::make_unique<qbt::CsvDataSource>(path_);
  }

  void TearDown(const benchmark::State&) override
  {
    src_.reset();
    std::error_code ec;
    std::filesystem::remove(path_, ec);
  }

protected:
  std::size_t rows_{};
  std::uintmax_t file_size_{};
  std::filesystem::path path_;
  std::unique_ptr<qbt::CsvDataSource> src_;
};

BENCHMARK_DEFINE_F(CsvLoadFixture, Load)(benchmark::State& state)
{
  for (auto _ : state) {
    std::string content = src_->load();
    benchmark::DoNotOptimize(content.data());
    benchmark::ClobberMemory();
  }
  state.SetBytesProcessed(static_cast<std::int64_t>(state.iterations()) *
                          static_cast<std::int64_t>(file_size_));
}
BENCHMARK_REGISTER_F(CsvLoadFixture, Load)
  ->RangeMultiplier(10)
  ->Range(1'000, 1'000'000);

} // namespace

BENCHMARK_MAIN();
