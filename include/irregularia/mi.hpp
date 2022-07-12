#pragma once

#include <type_traits>

#include "mitraits.hpp"

namespace irregularia
{

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
requires std::is_integral_v<BackingStorage>
struct multiple_int
{
  using traits =
      detail::_multiple_int_traits<IntCount, BitWidth, BackingStorage>;
  typename traits::int_type value;

  auto intv() const -> typename traits::int_type
  {
    return this->value & traits::int_mask;
  }

  auto carry() const -> typename traits::int_type
  {
    return this->value & traits::carry_mask;
  }
};

};  // namespace irregularia

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
auto operator+(
    irregularia::multiple_int<IntCount, BitWidth, BackingStorage> lhs,
    irregularia::multiple_int<IntCount, BitWidth, BackingStorage> rhs)
    -> irregularia::multiple_int<IntCount, BitWidth, BackingStorage>
{
  using value_type =
      irregularia::multiple_int<IntCount, BitWidth, BackingStorage>;

  // Use intv instead of the raw value to avoid adding carry bits, which would 
  // "bleed" their overflow into the LSB of the following integer
  auto sum = static_cast<value_type::traits::int_type>(lhs.intv() + rhs.intv());
  auto mi_sum = value_type {sum};


  // 1 for handle, 0 for zero out unconditionally
  // Will cause compilation error if not defined
  if constexpr (IRREGULARIA_CARRY_BIT_POLICY == 0) {
    mi_sum.value &= value_type::traits::int_mask;
  }
  else {
    // If carry bit was set on neither lhs nor rhs, and no carry bits are set in their sum, no carry bits are set
    // If carry bit was set on neither lhs nor rhs, and carry bits are set in their sum, retain them
    // If carry bit was set on lhs and / or rhs, and no carry bit is set in their sum, retain it
    // If carry bit was set on lhs and / or rhs, and carry bits are set in their sum, retain them

    // => if carry bits are set anywhere, they must be propagated
    mi_sum.value |= (lhs.value | rhs.value | mi_sum.value) & value_type::traits::carry_mask
  }

  return mi_sum;
}