#pragma once

#include "mitraits.hpp"

namespace irregularia
{

template<std::size_t BitWidth>
struct multiple_int
{
  using traits = detail::_multiple_int_traits<BitWidth>;
  typename traits::int_type value;

  auto intv() const -> typename traits::int_type
  {
    return this->value & traits::int_mask;
  }

  auto carry() const -> typename traits::int_type
  {
    return this->value & traits::carry_mask;
  }

  auto flatten_carry() -> void
  {
    // TODO: is converting the carry bits into a singular carry bit necessary?
    if constexpr (irregularia::multiple_int<BitWidth>::traits::carry_width <= 1) {
      return;
    }

    if (auto c = this->carry(); c != 0) {
      // zero out carry bits
      this->value &= ~traits::carry_mask;

      // toggle one on lowest bit of carry section of value
      this->value |= (1 << BitWidth);
    }
  }
};

};  // namespace irregularia

template<std::size_t BitWidth>
auto operator+(irregularia::multiple_int<BitWidth> lhs,
               irregularia::multiple_int<BitWidth> rhs)
    -> irregularia::multiple_int<BitWidth>
{
  auto sum = irregularia::multiple_int<BitWidth>{lhs.value + rhs.value};
  sum.flatten_carry();

  return sum;
}