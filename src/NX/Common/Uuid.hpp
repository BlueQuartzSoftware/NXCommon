#pragma once

#include "NX/Common/Bit.hpp"
#include "NX/Common/NXCommon_export.hpp"
#include "NX/Common/Types.hpp"

#include <fmt/format.h>

#include <array>
#include <functional>
#include <optional>
#include <string_view>

namespace NX::Common
{
namespace detail
{
inline constexpr std::byte Hex2Byte(char ch) noexcept
{
  if(ch >= '0' && ch <= '9')
  {
    uint8 value = ch - '0';
    return std::byte{value};
  }
  if(ch >= 'a' && ch <= 'f')
  {
    uint8 value = 10 + ch - 'a';
    return std::byte{value};
  }
  if(ch >= 'A' && ch <= 'F')
  {
    uint8 value = 10 + ch - 'A';
    return std::byte{value};
  }
  return std::byte{0};
}

inline constexpr std::byte Hex2Byte(char first, char second) noexcept
{
  std::byte result = Hex2Byte(first) << 4;
  result |= Hex2Byte(second);
  return result;
}

inline constexpr bool IsHex(char ch) noexcept
{
  return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

inline constexpr usize String2Bytes(std::string_view string, usize offset, usize size, std::array<std::byte, 16>& uuid, usize uuidOffset) noexcept
{
  for(usize i = 0; i < size; i++)
  {
    usize firstIndex = (i * 2) + offset;
    usize secondIndex = firstIndex + 1;
    char firstChar = string[firstIndex];
    if(!IsHex(firstChar))
    {
      return 0;
    }
    char secondChar = string[secondIndex];
    if(!IsHex(secondChar))
    {
      return 0;
    }
    uuid[i + uuidOffset] = Hex2Byte(firstChar, secondChar);
  }

  return size * 2;
}
} // namespace detail

/**
 * @brief Uuid struct
 * Stored in big-endian order as according to RFC 4122
 */
struct NXCOMMON_EXPORT Uuid
{
  static inline constexpr usize k_Size = 16;

  /**
   * @brief Parses a uuid string into a Uuid.
   * @param string Must be of the form "{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}".
   * Must have both braces or none. Must have all dashes or none.
   * @return Parsed uuid if successful. Otherwise, empty optional.
   */
  static inline constexpr std::optional<Uuid> FromString(std::string_view string)
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

  uint32 time_low() const noexcept;

  uint16 time_mid() const noexcept;

  uint16 time_hi_version() const noexcept;

  uint16 clock_seq_hi_and_res_clock_seq_low() const noexcept;

  uint8 clock_seq_hi_variant() const noexcept;

  uint8 clock_seq_low() const noexcept;

  uint64 node() const noexcept;

  std::string str() const;

  std::array<std::byte, k_Size> data;
};

inline constexpr bool operator==(const Uuid& lhs, const Uuid& rhs) noexcept
{
  for(usize i = 0; i < Uuid::k_Size; i++)
  {
    if(lhs.data[i] != rhs.data[i])
    {
      return false;
    }
  }
  return true;
}

inline constexpr bool operator!=(const Uuid& lhs, const Uuid& rhs) noexcept
{
  return !(lhs == rhs);
}

inline constexpr bool operator<(const Uuid& lhs, const Uuid& rhs) noexcept
{
  for(usize i = 0; i < Uuid::k_Size; i++)
  {
    if(lhs.data[i] < rhs.data[i])
    {
      return true;
    }
    if(rhs.data[i] < lhs.data[i])
    {
      return false;
    }
  }
  return false;
}

inline constexpr bool operator>(const Uuid& lhs, const Uuid& rhs) noexcept
{
  return rhs < lhs;
}

inline constexpr bool operator<=(const Uuid& lhs, const Uuid& rhs) noexcept
{
  return !(rhs < lhs);
}

inline constexpr bool operator>=(const Uuid& lhs, const Uuid& rhs) noexcept
{
  return !(lhs < rhs);
}
} // namespace NX::Common

namespace std
{
template <>
struct hash<::NX::Common::Uuid>
{
  std::size_t operator()(const ::NX::Common::Uuid& value) const noexcept
  {
    std::uint64_t v1 = ::NX::Common::bit_cast_ptr<std::uint64_t>(value.data.data());
    std::uint64_t v2 = ::NX::Common::bit_cast_ptr<std::uint64_t>(value.data.data() + sizeof(v1));
    return v1 ^ v2;
  }
};
} // namespace std
