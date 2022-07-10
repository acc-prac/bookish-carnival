#include <cinttypes>
#include <iostream>
#include <type_traits>

namespace irregularia
{

namespace detail
{

template<std::size_t BitWidth>
struct _multiple_int_traits
{
  static_assert(BitWidth > 0,
                "No such thing as integers with 0 bits (at least, not here!)");

  static_assert(
      BitWidth % 8 != 0,
      "Use (u)int{8, 16, 32, 64}_t if you want widths with multiples of 8");

  // Round up for uneven amount of bits
  static auto constexpr byte_width =
      BitWidth / 8 + ((BitWidth % 8 == 0) ? 0 : 1);

  using int_type = std::conditional_t<
      BitWidth <= 8,
      uint8_t,
      std::conditional_t<
          BitWidth <= 16,
          uint16_t,
          std::conditional_t<BitWidth <= 32, uint32_t, uint64_t>>>;
  using carry_type = int_type;

  static auto constexpr int_width = BitWidth;
  static auto constexpr carry_width = byte_width * 8 - int_width;
};

};  // namespace detail

template<std::size_t BitWidth>
struct multiple_int
{
  using traits = detail::_multiple_int_traits<BitWidth>;

  union
  {
    struct
    {
      typename traits::carry_type carry : traits::carry_width;
      typename traits::int_type active : traits::int_width;
    } bf;
    typename traits::int_type value;
  } bits;

  static_assert(sizeof(bits) == sizeof(decltype(bits)::bf),
                "Bitfield struct is smaller than total union");
  static_assert(sizeof(bits) == sizeof(decltype(bits)::value),
                "Backing integer storage is smaller than total union");
};

};  // namespace irregularia
