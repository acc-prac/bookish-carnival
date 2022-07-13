#pragma once

#include <type_traits>

#include "mitraits.hpp"

namespace irregularia
{

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
requires std::is_integral_v<BackingStorage> && std::is_unsigned_v<
    BackingStorage>
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

  auto operator+(
      irregularia::multiple_int<IntCount, BitWidth, BackingStorage> rhs)
      -> irregularia::multiple_int<IntCount, BitWidth, BackingStorage>
  {
    using value_type =
        irregularia::multiple_int<IntCount, BitWidth, BackingStorage>;

    // 1 for handle, 0 for zero out unconditionally
    // Will cause compilation error if not defined
    if constexpr (IRREGULARIA_BIT_CARRY_POLICY == 0) {
      // No need to use intv, as the carry bits are never on, therefore they can
      // never bleed into the LSB of the following integer
      auto sum =
          static_cast<typename traits::int_type>(this->value + rhs.value);
      auto mi_sum = value_type {sum};
      mi_sum.value &= traits::int_mask;

      return mi_sum;
    } else {
      // Use intv instead of the raw value to avoid adding carry bits, which
      // would "bleed" their overflow into the LSB of the following integer
      auto sum =
          static_cast<typename traits::int_type>(this->intv() + rhs.intv());
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
      mi_sum.value |= (this->value | rhs.value) & value_type::traits::carry_mask;
      return mi_sum;
    }
  }
};

};  // namespace irregularia