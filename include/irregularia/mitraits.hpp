#pragma once

#include <cinttypes>
#include <limits>
#include <type_traits>

namespace irregularia::detail
{

// Set every BitWidth'th bit
template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
struct _carry_mask
{
  static constexpr BackingStorage value =
      (static_cast<BackingStorage>(1) << (IntCount * BitWidth))
      | _carry_mask<IntCount - 1, BitWidth, BackingStorage>::value;
};

template<std::size_t BitWidth, typename BackingStorage>
struct _carry_mask<0, BitWidth, BackingStorage>
{
  static constexpr BackingStorage value = 0;
};

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
constexpr BackingStorage _carry_mask_v =
    _carry_mask<IntCount, BitWidth, BackingStorage>::value;

// Set n bits, n + 1'th bit is for the carry, and above the IntCount * BitWidth
// bit is only 0s
template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
struct _int_mask
{
  static constexpr BackingStorage pattern = ((1 << BitWidth) - 1);

  static constexpr BackingStorage value = (pattern << (IntCount * BitWidth - 1))
      | _int_mask<IntCount - 1, BitWidth, BackingStorage>::value;
};

template<std::size_t BitWidth, typename BackingStorage>
struct _int_mask<0, BitWidth, BackingStorage>
{
  static constexpr BackingStorage value = 0;
};

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
constexpr BackingStorage _int_mask_v =
    _int_mask<IntCount, BitWidth, BackingStorage>::value;

// Set upper sizeof(BackingStorage) - (IntCount * BitWidth) bits,
// remainder is only 0s
template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
struct _empty_mask
{
  static constexpr BackingStorage value = ~((1 << (IntCount * BitWidth)) - 1);
};


template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
constexpr BackingStorage _empty_mask_v =
    _empty_mask<IntCount, BitWidth, BackingStorage>::value;

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

  // Masks all the bits reserved for the actual values
  // NOTE: If the integers with their carry bit do not fit perfectly into the
  // width of the backing storage, NOTE: this results in the upper otherwise
  // unused bits becomes 1s in this flag. NOTE: However, this is not an issue,
  // as these bits will never be used at all!
  static int_type constexpr int_mask = 
    _int_mask_v<IntCount, BitWidth, BackingStorage>;

  static int_type constexpr empty_mask = 
    _empty_mask_v<IntCount, BitWidth, BackingStorage>;

  // Masks upper N - BitWidth bits of value
};

};  // namespace irregularia::detail