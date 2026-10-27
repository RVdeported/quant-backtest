//===========================================================================//
//                        "csv_data_source.cpp":                              //
//===========================================================================//

#include "quant_backtest/csv_data_source.hpp"

#include <fstream>
#include <stdexcept>
#include <utility>

namespace qbt
{

CsvDataSource::CsvDataSource(std::filesystem::path path)
  : path_(std::move(path))
{
}

std::string CsvDataSource::load() const
{
  std::ifstream in(path_, std::ios::binary);
  if (!in.is_open()) {
    throw std::runtime_error("CsvDataSource::load: cannot open '" +
                             path_.string() + "'");
  }

  in.seekg(0, std::ios::end);
  const auto end = in.tellg();
  if (end < 0) {
    throw std::runtime_error("CsvDataSource::load: cannot seek '" +
                             path_.string() + "'");
  }
  in.seekg(0, std::ios::beg);

  std::string buf;
  buf.resize(static_cast<std::size_t>(end));
  if (end > 0) {
    in.read(buf.data(), end);
    if (in.gcount() != end) {
      throw std::runtime_error("CsvDataSource::load: short read on '" +
                               path_.string() + "'");
    }
  }
  return buf;
}

const std::filesystem::path& CsvDataSource::path() const noexcept
{
  return path_;
}

} // namespace qbt
