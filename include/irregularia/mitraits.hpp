#pragma once

#include <cinttypes>
#include <limits>
#include <type_traits>

namespace detail
{

// Set every BitWidth'th bit
template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
struct _carry_mask
{
  static constexpr BackingStorage value =
      (static_cast<BackingStorage>(1) << (IntCount * BitWidth - 1))
      | _carry_mask<IntCount - 1, BitWidth, BackingStorage>::value;
};

template<std::size_t BitWidth, typename BackingStorage>
struct _carry_mask<0, BitWidth, BackingStorage>
{
  static constexpr BackingStorage value = 0;
};

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
constexpr BackingStorage _carry_mask_v =
    typename _carry_mask<IntCount, BitWidth, BackingStorage>::value;

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
struct _multiple_int_traits
{
  static_assert(BitWidth > 0,
                "No such thing as integers with 0 bits (at least, not here!)");

  // Add one for the carry bit
  static auto constexpr min_byte_width = (BitWidth + 1) * IntCount / 8
      + ((((BitWidth + 1) * IntCount) % 8 == 0) ? 0 : 1);

  static_assert(sizeof(BackingStorage) >= min_byte_width,
                "Invalid BackingStorage; the specified amount of ints and "
                "their bits do not fit!");

  using int_type = BackingStorage;

  // Mask only the singular padding carry bits
  static int_type constexpr carry_mask =
      _carry_mask_v<IntCount, BitWidth, BackingStorage>;

  // Masks pre BitWidth bits of value
  static int_type constexpr int_mask = ~carry_mask;

  // Masks upper N - BitWidth bits of value
};

};  // namespace detail