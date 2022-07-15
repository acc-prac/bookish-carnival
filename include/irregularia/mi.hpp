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

private:
  multiple_int(typename traits::int_type value)
      : value_ {value}
  {
  }

  typename traits::int_type value_;

public:
  template<typename IndivStorage, std::size_t AtMostIntCount>
  requires(AtMostIntCount <= IntCount
           && AtMostIntCount
               > 0) static auto encode(const std::array<int, AtMostIntCount>& input)
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

  std::array<int, IntCount> decode()
  {
    // Create a mask with #bit-width bits set to one
    static auto mask = (static_cast<unsigned int>(1) << BitWidth) - 1;

    std::array<int, IntCount> data;

    if constexpr (IntCount > 1) {
      for (std::size_t i = 0; i < IntCount; ++i) {
        auto val = ((value_ >> (i * (BitWidth + 1))) & mask);

        // During encoding numbers are inserted in reverse order,
        // decode them in reverse order to correct that
        if ((val >> (BitWidth - 1)) != 0) {
          data[IntCount - i - 1] = (~(mask) | val);
        } else {
          data[IntCount - i - 1] = val;
        }
      }
    }

    return data;
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

    // 1 for handle, 0 for zero out unconditionally
    // Will cause compilation error if not defined
    if constexpr (IRREGULARIA_BIT_CARRY_POLICY == 0) {
      // No need to use intv, as the carry bits are never on, therefore they can
      // never bleed into the LSB of the following integer
      auto sum = static_cast<typename traits::int_type>(this->value_ + rhs.value_);
      auto mi_sum = value_type {sum};
      mi_sum.value_ &= traits::int_mask;

      return mi_sum;
    } else {
      // Use intv instead of the raw value to avoid adding carry bits, which
      // would "bleed" their overflow into the LSB of the following integer
      auto sum = static_cast<typename traits::int_type>(this->intv() + rhs.intv());
      auto mi_sum = value_type {sum};
      // If carry bit was set on neither lhs nor rhs, and no carry bits are set
      // in their sum,  no carry bits are set

      // If carry bit was set on neither lhs nor rhs, and carry bits are set in
      // their sum, retain them

      // If carry bit was set on lhs and / or rhs, and no carry bit is set in
      // their sum, retain it

      // If carry bit was set on lhs and / or rhs, and carry bits are
      // set in their sum, retain them

      // => if carry bits are set anywhere, they must be propagated
      mi_sum.value_ |= (this->value_ | rhs.value_) & traits::carry_mask;
      return mi_sum;
    }
  }

  auto operator<=>(irregularia::multiple_int<BitWidth, BackingStorage> rhs) const
      -> std::strong_ordering
  {
    if constexpr (IRREGULARIA_BIT_CARRY_POLICY == 1) {
      return this->intv() <=> rhs.intv();
    } else {
      return this->value_ <=> rhs.value_;
    }
  }

  auto operator==(irregularia::multiple_int<BitWidth, BackingStorage> rhs) const
      -> bool
  {
    if constexpr (IRREGULARIA_BIT_CARRY_POLICY == 1) {
      return this->intv() == rhs.intv();
    } else {
      return this->value_ == rhs.value_;
    }
  }

  auto operator!=(irregularia::multiple_int<BitWidth, BackingStorage> rhs) const
      -> bool
  {
    if constexpr (IRREGULARIA_BIT_CARRY_POLICY == 1) {
      return this->intv() != rhs.intv();
    } else {
      return this->value_ != rhs.value_;
    }
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