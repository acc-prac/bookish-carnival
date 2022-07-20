#pragma once

#include <array>
#include <compare>
#include <functional>
#include <type_traits>

#include "mitraits.hpp"

namespace irregularia
{

template<std::size_t BitWidth, typename BackingStorage>
requires std::is_integral_v<BackingStorage> && std::is_unsigned_v<BackingStorage>
struct multiple_int
{
public:
  static constexpr int IntCount = (8 * sizeof(BackingStorage)) / (BitWidth + 1);

  using traits = detail::_multiple_int_traits<IntCount, BitWidth, BackingStorage>;

  friend class std::numeric_limits<multiple_int<BitWidth, BackingStorage>>;
  // We cannot access the private member in the conversion constructor because we
  // are here in a template class. For this reason, we need to use the "friend class"
  // clausel
  friend class multiple_int<2 * BitWidth + 1,
                            typename traits::template next_widest<BackingStorage>>;

private:
  multiple_int(typename traits::int_type value)
      : value_ {value}
  {
  }

public:
  // Default ctor = all zeros
  multiple_int() = default;

  /* clang-format off */
  template<std::size_t SmallerBitWidth, typename SmallerBackingStorage>
  requires(2 * SmallerBitWidth + 1 == BitWidth
           && 2 * sizeof(SmallerBackingStorage) == sizeof(BackingStorage))
  multiple_int(multiple_int<SmallerBitWidth, SmallerBackingStorage> halfSizeMultipleInt)
      : value_ {0}
  /* clang-format on */
  {
    using lower_multiple_int = multiple_int<SmallerBitWidth, SmallerBackingStorage>;

    static constexpr auto evenIndicesMask = detail::_int_pattern < (IntCount % 2 == 0)
        ? IntCount - 2
        : IntCount - 1,
                          SmallerBitWidth, SmallerBackingStorage > ::value;
    static constexpr auto oddIndicesMask = detail::_int_pattern < (IntCount % 2 == 0)
        ? IntCount - 1
        : IntCount - 2,
                          SmallerBitWidth, SmallerBackingStorage > ::value;

    // Mask all bits other than the sign bits with 1s
    static constexpr auto signMaskShiftedByOne =
        detail::_sign_pattern<IntCount,
                              SmallerBitWidth,
                              SmallerBackingStorage,
                              BackingStorage>::value;

    // E.g. −2 in 3b is 0b110 -> Mask: 0000; 2 in 3b is 0b010 -> Mask: 1000
    static constexpr auto invertedAndShiftedSignBitMask =
        (lower_multiple_int::traits::carry_mask
         | static_cast<BackingStorage>(lower_multiple_int::traits::carry_mask)
             << (IntCount * (SmallerBitWidth + 1)))
        ^ traits::carry_mask;

    // Transfer all small ints with EVEN indices
    value_ |= halfSizeMultipleInt.value_ & evenIndicesMask;

    // Transfer all small ints with ODD indices
    value_ |=
        ((static_cast<BackingStorage>(halfSizeMultipleInt.value_ & oddIndicesMask))
         << ((IntCount % 2 == 0) ? (IntCount - 1) : (IntCount)) * (SmallerBitWidth + 1));

    // Add sign mask to the result (Negative numbers become now negative again)
    value_ += signMaskShiftedByOne;

    // If the number was a positive number, we have to add the inverted and shifted sign
    // bit mask
    value_ += (((~value_) & traits::int_mask) << 1) & invertedAndShiftedSignBitMask;

    // Reset all carry (overflow)-bits
    value_ &= traits::int_mask;
  }

private:
  typename traits::int_type value_;

public:
  /* clang-format off */
  template<std::size_t AtMostIntCount> 
  requires(AtMostIntCount <= IntCount && AtMostIntCount > 0)
  static auto encode(const std::array<int, AtMostIntCount>& input)
  /* clang-format on */
  {
    // Create a mask with #bit-width bits set to one
    static auto mask = (static_cast<unsigned int>(1) << BitWidth) - 1;

    BackingStorage value_ = 0;

    if constexpr (IntCount > 1) {
      for (std::size_t i = 0; i < (IntCount - 1); ++i) {
        // Insert value
        value_ |= (input[i] & mask);
        // Shift by bit width + 1 (carry bit)
        value_ <<= (BitWidth + 1);
      }
    }

    // Don't shift the last value
    value_ |= (input[IntCount - 1] & mask);

    return multiple_int<BitWidth, BackingStorage> {value_};
  }

  auto decode() const -> std::array<int, IntCount>
  {
    // Create a mask with #bit-width bits set to one
    static auto mask = (static_cast<unsigned int>(1) << BitWidth) - 1;

    std::array<int, IntCount> data;

    // Avoid compiler warnings
    if constexpr (IntCount > 1) {
      for (std::size_t i = 0; i < IntCount; ++i) {
        // During encoding numbers are inserted in reverse order,
        // decode them in reverse order to correct that

        // Extracts one number from the internal storage
        auto val = ((value_ >> (i * (BitWidth + 1))) & mask);
        // Create a mask depending on the MSB of the extracted number
        auto neg_mask = ((~((val >> (BitWidth - 1)) - 1)) << BitWidth);

        data[IntCount - i - 1] = (neg_mask | val);

        // auto val = ((value_ >> (i * (BitWidth + 1))) & mask);

        // During encoding numbers are inserted in reverse order,
        // decode them in reverse order to correct that
        // if ((val >> (BitWidth - 1)) != 0) { data[IntCount - i - 1] = (~(mask) | val); }
        // else { data[IntCount - i - 1] = val; }
      }
    }

    return data;
  }

  /* clang-format off */
  template<std::size_t SmallerBitWidth, typename SmallerBackingStorage>
  requires(2 * SmallerBitWidth + 1 == BitWidth && 
           2 * sizeof(SmallerBackingStorage) == sizeof(BackingStorage))
  explicit operator multiple_int<SmallerBitWidth, SmallerBackingStorage>() const
  /* clang-format on */
  {
    using source_type = multiple_int<BitWidth, BackingStorage>;
    using target_type = multiple_int<SmallerBitWidth, SmallerBackingStorage>;

    static constexpr auto trunc =
        detail::truncation_mask_v<IntCount, BitWidth, SmallerBitWidth, BackingStorage>;

    // EXPECT_EQ(0b00'000111'000111'000111'000111'000111, trunc);

    auto truncated = value_ & trunc;
    // EXPECT_EQ(0b00'000001'000000'000101'000111'000000, truncated);

    // rounds up odd counts of numbers, as these are the 0th, 2nd, 4th etc.
    static constexpr auto lower_shift =
        (IntCount % 2 == 0) ? IntCount / 2 : (IntCount + 1) / 2;

    // matches lower "half" of truncated numbers
    static constexpr auto lower =
        (static_cast<BackingStorage>(1) << (lower_shift * (BitWidth + 1))) - 1;
    // EXPECT_EQ(0b00'000000'000000'111111'111111'111111, lower);

    // matches upper "half" of truncated numbers
    static constexpr auto upper = ~lower & ~source_type::traits::empty_mask;
    // EXPECT_EQ(0b00'111111'111111'000000'000000'000000, upper);

    // extracts integers in the lower "half"
    auto lower_half = static_cast<SmallerBackingStorage>(truncated & lower);
    // EXPECT_EQ(static_cast<SmallerBackingStorage>(0b0'101'000'111'000'000), lower_half)

    // extracts integers in the upper "half"
    auto upper_half = truncated & upper;
    // EXPECT_EQ(0b00'000001'000000'000000'000000'000000, upper_half);

    static constexpr auto upper_shift_down =
        ((IntCount % 2 == 0) ? (IntCount - 1) : (IntCount)) * (BitWidth + 1) / 2;
    auto upper_half2 = static_cast<SmallerBackingStorage>(upper_half >> upper_shift_down);
    // EXPECT_EQ(static_cast<SmallerBackingStorage>(0b0'000'001'000'000'000), upper_half2)

    // merge results
    return target_type {static_cast<SmallerBackingStorage>(lower_half | upper_half2)};
    // EXPECT_EQ(0b0'101'001'111'000'000, result);
  }

  auto intv() const -> typename traits::int_type
  {
    return this->value_ & traits::int_mask;
  }

  auto carry() const -> typename traits::int_type
  {
    return this->value_ & traits::carry_mask;
  }

  auto operator+(irregularia::multiple_int<BitWidth, BackingStorage> rhs) const
      -> irregularia::multiple_int<BitWidth, BackingStorage>
  {
    using value_type = irregularia::multiple_int<BitWidth, BackingStorage>;
    // Use intv instead of the raw value to avoid adding carry bits, which
    // would "bleed" their overflow into the LSB of the following integer
    auto sum = static_cast<typename traits::int_type>(this->intv() + rhs.intv());
    auto mi_sum = value_type {sum};
    return mi_sum;
  }

  auto operator<=>(irregularia::multiple_int<BitWidth, BackingStorage> rhs) const
      -> std::strong_ordering
  {
    return this->intv() <=> rhs.intv();
  }

  auto operator==(irregularia::multiple_int<BitWidth, BackingStorage> rhs) const -> bool
  {
    return this->intv() == rhs.intv();
  }

  auto operator!=(irregularia::multiple_int<BitWidth, BackingStorage> rhs) const -> bool
  {
    return this->intv() != rhs.intv();
  }
};
};  // namespace irregularia

template<std::size_t BitWidth, typename BackingStorage>
struct std::less<irregularia::multiple_int<BitWidth, BackingStorage>>
{
  constexpr auto operator()(
      irregularia::multiple_int<BitWidth, BackingStorage> const& lhs,
      irregularia::multiple_int<BitWidth, BackingStorage> const& rhs) const -> bool
  {
    return lhs < rhs;
  }
};