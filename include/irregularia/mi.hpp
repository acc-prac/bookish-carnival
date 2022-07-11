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
    if constexpr (irregularia::multiple_int<BitWidth>::traits::carry_width <= 1)
    {
      return;
    }

    if (auto c = this->carry(); c != 0) {
      // zero out carry bits
      this->value = static_cast<typename traits::int_type>(this->value & traits::carry_mask);

      // toggle one on lowest bit of carry section of value
      this->value |= (static_cast<typename traits::int_type>(1) << BitWidth);
    }
  }
};

};  // namespace irregularia

template<std::size_t BitWidth>
auto operator+(irregularia::multiple_int<BitWidth> lhs,
               irregularia::multiple_int<BitWidth> rhs)
    -> irregularia::multiple_int<BitWidth>
{
  auto sum = static_cast<irregularia::multiple_int<BitWidth>::traits::int_type>(
      lhs.value + rhs.value);
  auto mi_sum = irregularia::multiple_int<BitWidth> {sum};
  mi_sum.flatten_carry();

  return mi_sum;
}