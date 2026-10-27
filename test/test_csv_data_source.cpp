//===========================================================================//
//                    "test_csv_data_source.cpp":                             //
//===========================================================================//

#include "quant_backtest/csv_data_source.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

namespace
{
std::filesystem::path make_temp_csv(const std::string& name,
                                    const std::string& content)
{
  const auto path = std::filesystem::temp_directory_path() / name;
  std::ofstream out(path, std::ios::binary | std::ios::trunc);
  out << content;
  out.close();
  return path;
}
} // namespace

TEST(CsvDataSourceTest, LoadReadsWholeFile)
{
  const std::string content =
    "1,0.1,1.0,1699999999.123456789,1\n"
    "2,0.2,2.0,1699999999.223456789,0\n";
  const auto path = make_temp_csv("qbt_test_trades.csv", content);

  const qbt::CsvDataSource src(path);
  EXPECT_EQ(src.path(), path);
  EXPECT_EQ(src.load(), content);

  std::error_code ec;
  std::filesystem::remove(path, ec);
}

TEST(CsvDataSourceTest, LoadThrowsOnMissingFile)
{
  const qbt::CsvDataSource src("/nonexistent/qbt_missing.csv");
  EXPECT_THROW(src.load(), std::runtime_error);
}
