#include <gtest/gtest.h>
#include <irregularia/mi.hpp>

TEST(Addition, NonOverflowingSingleIntSingleBit)
{
  {
    auto l = irregularia::multiple_int<1, 1, std::uint8_t> {0};
    auto r = irregularia::multiple_int<1, 1, std::uint8_t> {0};

    auto s = l + r;
    EXPECT_EQ(0, s.intv());
    EXPECT_EQ(0, s.carry());
  }

  {
    auto l = irregularia::multiple_int<1, 1, std::uint8_t> {0};
    auto r = irregularia::multiple_int<1, 1, std::uint8_t> {1};

    auto s = l + r;
    EXPECT_EQ(1, s.intv());
    EXPECT_EQ(0, s.carry());
  }

  {
    auto l = irregularia::multiple_int<1, 1, std::uint8_t> {1};
    auto r = irregularia::multiple_int<1, 1, std::uint8_t> {0};

    auto s = l + r;
    EXPECT_EQ(1, s.intv());
    EXPECT_EQ(0, s.carry());
  }
}

TEST(Addition, OverflowingSingleIntSingleBit)
{
  auto l = irregularia::multiple_int<1, 1, std::uint8_t> {1};
  auto r = irregularia::multiple_int<1, 1, std::uint8_t> {1};

  auto s = l + r;

  if constexpr (IRREGULARIA_BIT_CARRY_POLICY == 1) {
    EXPECT_EQ(0, s.intv());
    EXPECT_EQ(1 << 1, s.carry());
  }

  else
  {
    EXPECT_EQ(0, s.intv());
    EXPECT_EQ(0, s.carry());
  }
}


TEST(Addition, PartiallyOverflowing4Ints7Bit)
{
  // Max container for 32 bits
  auto l = irregularia::multiple_int<4, 7, std::uint32_t> {0xEFEFFFFF};

  // Will overflow on the lower two ints, but keep the upper two
  auto r = irregularia::multiple_int<4, 7, std::uint32_t> {0x01010101};

  auto s = l + r;

  if constexpr (IRREGULARIA_BIT_CARRY_POLICY == 1) {
    EXPECT_EQ(0xE0E00000, s.intv());
    EXPECT_EQ(0x00007070, s.carry());
  }

  else
  {
    EXPECT_EQ(0xE0E00000, s.intv());
    EXPECT_EQ(0x00000000, s.carry());
  }

}