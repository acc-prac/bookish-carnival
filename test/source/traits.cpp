#include <gtest/gtest.h>
#include <irregularia/mi.hpp>

using OneIntOneBit = irregularia::multiple_int<6, std::uint8_t>;
using TwoInts3Bit = irregularia::detail::_multiple_int_traits<2, 3, std::uint8_t>;

TEST(Traits, Masks)
{
  EXPECT_EQ(0b00111111, OneIntOneBit::traits::int_mask);
  EXPECT_EQ(0b01000000, OneIntOneBit::traits::carry_mask);
  EXPECT_EQ(0b10000000, OneIntOneBit::traits::empty_mask);
  EXPECT_EQ(0b00000000,
            OneIntOneBit::traits::int_mask & OneIntOneBit::traits::carry_mask
                & OneIntOneBit::traits::empty_mask);

  EXPECT_EQ(0b01110111, TwoInts3Bit::int_mask);
  EXPECT_EQ(0b10001000, TwoInts3Bit::carry_mask);
  EXPECT_EQ(0b00000000, TwoInts3Bit::empty_mask);
  EXPECT_EQ(0b00000000,
            TwoInts3Bit::int_mask & TwoInts3Bit::carry_mask & TwoInts3Bit::empty_mask);
}