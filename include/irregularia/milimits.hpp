#pragma once

#include <limits>
#include <type_traits>

#include "mi.hpp"

namespace irregularia::detail
{

template<std::size_t IntCount,
         std::size_t BitWidth,
         typename BackingStorage,
         BackingStorage Pattern>
struct _repeat_bit_pattern
{
  static constexpr BackingStorage value = (Pattern << (IntCount * BitWidth - 1))
      | _repeat_bit_pattern<IntCount - 1, BitWidth, BackingStorage, Pattern>::value;
};

template<std::size_t BitWidth, typename BackingStorage, BackingStorage Pattern>
struct _repeat_bit_pattern<0, BitWidth, BackingStorage, Pattern>
{
  static constexpr BackingStorage value = 0;
};

template<std::size_t IntCount,
         std::size_t BitWidth,
         typename BackingStorage,
         BackingStorage Pattern>
constexpr auto _repeat_bit_pattern_v =
    _repeat_bit_pattern<IntCount, BitWidth, BackingStorage, Pattern>::value;

};  // namespace irregularia::detail

template<std::size_t BitWidth, typename BackingStorage>
struct std::numeric_limits<
    irregularia::multiple_int<BitWidth, BackingStorage>>
{
private:
  using value_type =
      irregularia::multiple_int<BitWidth, BackingStorage>;

  using traits = typename value_type::traits;

public:
  // TODO: How many more static attrs do we need
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = std::is_signed_v<typename traits::int_type>;
  static constexpr bool is_integer =
      std::is_integral_v<typename traits::int_type>;
  static constexpr bool is_exact = true;

  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;

  static constexpr bool has_denorm = false;
  static constexpr bool has_denorm_loss = false;

  static constexpr std::float_round_style round_style = std::round_toward_zero;

  // TODO: How many more static methods do we need
  static constexpr auto min() noexcept -> value_type
  {
    constexpr auto imin = std::numeric_limits<typename traits::int_type>::min();
    return value_type {imin & traits::int_mask};
  }

  static constexpr auto lowest() noexcept -> value_type
  {
    static_assert(is_integer,
                  "lowest has not been implemented for floating point types");
    return min();
  }

  static constexpr auto max() noexcept -> value_type
  {
    constexpr auto imin = std::numeric_limits<typename traits::int_type>::max();
    return value_type {imin & traits::int_mask};
  }

  static constexpr auto epsilon() noexcept -> value_type
  {
    constexpr auto eps =
        std::numeric_limits<typename traits::int_type>::epsilon();
    return value_type {eps};
  }

  static constexpr auto round_error() noexcept -> value_type
  {
    constexpr auto re =
        std::numeric_limits<typename traits::int_type>::round_error();
    return value_type {re};
  }

  static constexpr auto infinity() noexcept -> value_type
  {
    constexpr auto inf =
        std::numeric_limits<typename traits::int_type>::infinity();
    return value_type {inf};
  }
};