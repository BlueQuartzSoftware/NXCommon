#include "NX/Common/Uuid.hpp"

namespace NX::Common
{
uint32 Uuid::time_low() const noexcept
{
  return bit_cast_int<uint32, endian::big>(data.data());
}

uint16 Uuid::time_mid() const noexcept
{
  return bit_cast_int<uint16, endian::big>(data.data() + 4);
}

uint16 Uuid::time_hi_version() const noexcept
{
  return bit_cast_int<uint16, endian::big>(data.data() + 6);
}

uint16 Uuid::clock_seq_hi_and_res_clock_seq_low() const noexcept
{
  return bit_cast_int<uint16, endian::big>(data.data() + 8);
}

uint8 Uuid::clock_seq_hi_variant() const noexcept
{
  return bit_cast_int<uint8, endian::big>(data.data() + 8);
}

uint8 Uuid::clock_seq_low() const noexcept
{
  return bit_cast_int<uint8, endian::big>(data.data() + 9);
}

uint64 Uuid::node() const noexcept
{
  return bit_cast_int<uint64, endian::big, 6>(data.data() + 10);
}

std::string Uuid::str() const
{
  return fmt::format("{:08x}-{:04x}-{:04x}-{:02x}{:02x}-{:012x}", time_low(), time_mid(), time_hi_version(), clock_seq_hi_variant(), clock_seq_low(), node());
}
} // namespace NX::Common
