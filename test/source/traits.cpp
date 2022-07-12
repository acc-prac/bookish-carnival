#include <gtest/gtest.h>
#include <irregularia/mitraits.hpp>

using OneIntOneBit = irregularia::multiple_int<1, 1, std::uint8_t>;

using TwoInts3Bit = irregularia::multiple_int<2, 3, std::uint8_t>;

TEST(Traits, Masks)
{
  EXPECT_EQ(0b00000001, OneIntOneBit::int_mask);
  EXPECT_EQ(0b01110111, TwoInts3Bit::int_mask);

  EXPECT_EQ(0b00000010, OneIntOneBit::carry_mask);
  EXPECT_EQ(0b10001000, TwoInts3Bit::carry_mask);

  EXPECT_EQ(0b11111100, OneIntOneBit::empty_mask);
  EXPECT_EQ(0b00000000, TwoInts3Bit::empty_mask);
}