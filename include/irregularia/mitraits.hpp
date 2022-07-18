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
  static constexpr BackingStorage pattern = static_cast<BackingStorage>(1) << BitWidth;

  static constexpr BackingStorage value = pattern << ((IntCount - 1) * (BitWidth + 1))
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
  static constexpr BackingStorage pattern =
      (static_cast<BackingStorage>(1) << BitWidth) - 1;

  static constexpr BackingStorage value = (pattern << ((IntCount - 1) * (BitWidth + 1)))
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
  static constexpr BackingStorage value =
      IntCount * (BitWidth + 1) != sizeof(BackingStorage)
      ?
      // bits need to be padded
      ~((static_cast<BackingStorage>(1) << (IntCount * (BitWidth + 1))) - 1)
      :
      // ints and their carry bits take up entire storage space, empty
      // mask is itself, "empty"
      static_cast<BackingStorage>(0);
};

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
constexpr BackingStorage _empty_mask_v =
    _empty_mask<IntCount, BitWidth, BackingStorage>::value;

// Next widest uint_t-datatype
template<class>
struct _next_widest;

template<>
struct _next_widest<uint8_t>
{
  using type = uint16_t;
};

template<>
struct _next_widest<uint16_t>
{
  using type = uint32_t;
};

template<>
struct _next_widest<uint32_t>
{
  using type = uint64_t;
};

template<>
struct _next_widest<uint64_t>
{
  // There is no bigger uint_t-datatype
  using type = uint64_t;
};

// Mask all even (or odd) integer with 1s
template<int Index, std::size_t BitWidth, typename BackingStorage>
struct _int_pattern
{
  using IntMask = typename detail::_int_mask<Index, BitWidth, BackingStorage>;

  static constexpr BackingStorage value = IntMask::pattern << (Index * (BitWidth + 1))
      | _int_pattern<Index - 2, BitWidth, BackingStorage>::value;
};

template<std::size_t BitWidth, typename BackingStorage>
struct _int_pattern<1, BitWidth, BackingStorage>
{
  using IntMask = typename detail::_int_mask<1, BitWidth, BackingStorage>;

  static constexpr BackingStorage value = IntMask::pattern << (BitWidth + 1);
};

template<std::size_t BitWidth, typename BackingStorage>
struct _int_pattern<0, BitWidth, BackingStorage>
{
  static constexpr BackingStorage value =
      _int_pattern<1, BitWidth, BackingStorage>::IntMask::pattern;
};

// Mask all bits other than the sign bits with 1s
template<int IntCount,
         std::size_t SmallerBitWidth,
         typename SmallerBackingStorage,
         typename BackingStorage>
struct _sign_pattern
{
  using smaller_int_mask = detail::_int_mask<1, SmallerBitWidth, SmallerBackingStorage>;
  using bigger_int_mask = detail::_int_mask<1, 2 * SmallerBitWidth + 1, BackingStorage>;

  static constexpr BackingStorage pattern =
      bigger_int_mask::pattern ^ smaller_int_mask::pattern;

  static constexpr BackingStorage value = pattern
          << ((IntCount - 1) * (2 * SmallerBitWidth + 2))
      | _sign_pattern<IntCount - 1,
                      SmallerBitWidth,
                      SmallerBackingStorage,
                      BackingStorage>::value;
};

template<std::size_t SmallerBitWidth,
         typename SmallerBackingStorage,
         typename BackingStorage>
struct _sign_pattern<0, SmallerBitWidth, SmallerBackingStorage, BackingStorage>
{
  static constexpr BackingStorage value = 0;
};

template<std::size_t IntCount,
         std::size_t OldBitWidth,
         std::size_t NewBitWidth,
         typename BackingStorage>
struct truncation_mask
{
  static constexpr auto pattern =
      (static_cast<BackingStorage>(1) << (NewBitWidth + 1)) - 1;

  static constexpr auto value =
      static_cast<BackingStorage>(pattern << ((IntCount - 1) * (OldBitWidth + 1)))
      | truncation_mask<IntCount - 1, OldBitWidth, NewBitWidth, BackingStorage>::value;
};

template<std::size_t OldBitWidth, std::size_t NewBitWidth, typename BackingStorage>
struct truncation_mask<0, OldBitWidth, NewBitWidth, BackingStorage>
{
  static constexpr BackingStorage value = 0;
};

template<std::size_t IntCount,
         std::size_t OldBitWidth,
         std::size_t NewBitWidth,
         typename BackingStorage>
static constexpr auto truncation_mask_v =
    truncation_mask<IntCount, OldBitWidth, NewBitWidth, BackingStorage>::value;

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
struct _multiple_int_traits
{
  static_assert(BitWidth > 0,
                "No such thing as integers with 0 bits (at least, not here!)");

  static_assert(IntCount >= 1, "At least one integer must be stored");

  using int_type = BackingStorage;

  // Mask only the singular padding carry bits
  static int_type constexpr carry_mask =
      _carry_mask_v<IntCount, BitWidth, BackingStorage>;

  // Masks all the bits reserved for the actual values
  // NOTE: If the integers with their carry bit do not fit perfectly into the
  // width of the backing storage, NOTE: this results in the upper otherwise
  // unused bits becomes 1s in this flag. NOTE: However, this is not an issue,
  // as these bits will never be used at all!
  static int_type constexpr int_mask = _int_mask_v<IntCount, BitWidth, BackingStorage>;

  static int_type constexpr empty_mask =
      _empty_mask_v<IntCount, BitWidth, BackingStorage>;

  template<typename T>
  using next_widest = typename _next_widest<T>::type;

  // Masks upper N - BitWidth bits of value
};

};  // namespace irregularia::detail