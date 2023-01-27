#include "Uuid.hpp"

namespace NX::Common
{
#if 0
std::optional<Uuid> Uuid::FromString(std::string_view string)
{
  if(string.empty())
  {
    return {};
  }

  bool hasFrontBrace = string.front() == '{';
  bool hasBackBrace = string.back() == '}';

  if((hasFrontBrace && !hasBackBrace) || (!hasFrontBrace && hasBackBrace))
  {
    return {};
  }

  bool hasBraces = hasFrontBrace && hasBackBrace;

  usize index = hasBraces ? 1 : 0;

  bool hasDashes = string[index + 8] == '-';

  usize totalSize = 32ull + (hasBraces ? 2 : 0) + (hasDashes ? 4 : 1);

  if(string.size() != totalSize)
  {
    return {};
  }

  if(hasDashes && (string[index + 8] != '-' || string[index + 13] != '-' || string[index + 18] != '-' || string[index + 23] != '-'))
  {
    return {};
  }

  Uuid uuid{};

  // time_low (4 bytes)
  usize result = detail::String2Bytes(string, index, 4, uuid.data, 0);
  if(result == 0)
  {
    return {};
  }
  index += result;
  index += string[index] == '-' ? 1 : 0;

  // time_mid (2 bytes)
  result = detail::String2Bytes(string, index, 2, uuid.data, 4);
  if(result == 0)
  {
    return {};
  }
  index += result;
  index += string[index] == '-' ? 1 : 0;

  // time_hi_and_version (2 bytes)
  result = detail::String2Bytes(string, index, 2, uuid.data, 6);
  if(result == 0)
  {
    return {};
  }
  index += result;
  index += string[index] == '-' ? 1 : 0;

  // clock_seq_hi_and_res_seq_low (2 bytes)
  result = detail::String2Bytes(string, index, 2, uuid.data, 8);
  if(result == 0)
  {
    return {};
  }
  index += result;
  index += string[index] == '-' ? 1 : 0;

  // node (6 bytes)
  result = detail::String2Bytes(string, index, 6, uuid.data, 10);
  if(result == 0)
  {
    return {};
  }

  return uuid;
}
#endif

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
