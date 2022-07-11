#pragma once

#include "mitraits.hpp"

namespace irregularia
{

template<std::size_t BitWidth>
struct multiple_int
{
  using traits = detail::_multiple_int_traits<BitWidth>;
  typename traits::int_type value;

  auto intv() -> typename traits::int_type
  {
    return this->value & traits::int_mask;
  }

  auto carry() -> typename traits::int_type
  {
    return this->value & traits::carry_mask;
  }
};

};  // namespace irregularia