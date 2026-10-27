//===========================================================================//
//                        "csv_data_source.hpp":                              //
//===========================================================================//
// Stub of the CSV-backed data source.                                        //
//                                                                            //
// First implementation step: the source knows how to load a CSV file from    //
// disk into memory. Parsing into typed rows (ADR-0003) and ingest into       //
// ClickHouse (ADR-0007) come in later steps.                                 //
//===========================================================================//

#pragma once

#include <filesystem>
#include <string>

namespace qbt
{

//! @brief CSV-backed data source (stub).
//!
//! Binds to a single CSV file. Per ADR-0005 the file corresponds to one
//! instrument + one table type, and the instrument symbol is recovered from
//! the file name (not stored as a column).
class CsvDataSource
{
public:
  //! Construct a source bound to @p path.
  explicit CsvDataSource(std::filesystem::path path);

  //! Read the entire file from disk into a single contiguous buffer.
  //! @return the raw file content as a string.
  //! @throws std::runtime_error if the file cannot be opened or read.
  std::string load() const;

  //! The file path this source is bound to.
  [[nodiscard]] const std::filesystem::path& path() const noexcept;

private:
  std::filesystem::path path_;
};

} // namespace qbt
