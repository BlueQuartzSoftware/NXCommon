#pragma once

#include <cstddef>
#include <cstdint>
#include <set>

namespace NX
{
namespace types
{
using int8 = std::int8_t;
using uint8 = std::uint8_t;
using int16 = std::int16_t;
using uint16 = std::uint16_t;
using int32 = std::int32_t;
using uint32 = std::uint32_t;
using int64 = std::int64_t;
using uint64 = std::uint64_t;

using usize = std::size_t;

using float32 = float;
using float64 = double;
} // namespace types
using namespace types;

enum class NumericType : uint8
{
  int8,
  uint8,
  int16,
  uint16,
  int32,
  uint32,
  int64,
  uint64,
  float32,
  float64
};

enum class DataType : uint8
{
  int8,
  uint8,
  int16,
  uint16,
  int32,
  uint32,
  int64,
  uint64,
  float32,
  float64,
  boolean
};

enum class RunState
{
  Idle = 0,
  Queued = 1,
  Preflighting = 2,
  Executing = 3
};

enum class FaultState
{
  None = 0,
  Warnings = 1,
  Errors = 2
};

inline const std::set<DataType>& GetAllDataTypes()
{
  static const std::set<DataType> dataTypes = {NX::DataType::int8,  NX::DataType::uint8,  NX::DataType::int16,   NX::DataType::uint16,  NX::DataType::int32,  NX::DataType::uint32,
                                               NX::DataType::int64, NX::DataType::uint64, NX::DataType::float32, NX::DataType::float64, NX::DataType::boolean};
  return dataTypes;
}

inline const std::set<DataType>& GetAllNumericTypes()
{
  static const std::set<DataType> dataTypes = {NX::DataType::int8,   NX::DataType::uint8, NX::DataType::int16,  NX::DataType::uint16,  NX::DataType::int32,
                                               NX::DataType::uint32, NX::DataType::int64, NX::DataType::uint64, NX::DataType::float32, NX::DataType::float64};
  return dataTypes;
}

inline const std::set<DataType>& GetIntegerDataTypes()
{
  static const std::set<DataType> dataTypes = {NX::DataType::int8,  NX::DataType::uint8,  NX::DataType::int16, NX::DataType::uint16,
                                               NX::DataType::int32, NX::DataType::uint32, NX::DataType::int64, NX::DataType::uint64};
  return dataTypes;
}

} // namespace NX
