#include <gtest/gtest.h>
#include <irregularia/mi.hpp>
#include <irregularia/milimits.hpp>

TEST(Addition, NonOverflowingOneIntSevenBit)
{
  {
    auto l = irregularia::multiple_int<7, std::uint8_t>::encode<int, 1>({0});
    auto r = irregularia::multiple_int<7, std::uint8_t>::encode<int, 1>({0});

    auto s = l + r;
    EXPECT_EQ(0, s.intv());
    EXPECT_EQ(0, s.carry());
  }

  {
    auto l = irregularia::multiple_int<7, std::uint8_t>::encode<int, 1>({0});
    auto r = irregularia::multiple_int<7, std::uint8_t>::encode<int, 1>({1});

    auto s = l + r;
    EXPECT_EQ(1, s.intv());
    EXPECT_EQ(0, s.carry());
  }

  {
    auto l = irregularia::multiple_int<7, std::uint8_t>::encode<int, 1>({1});
    auto r = irregularia::multiple_int<7, std::uint8_t>::encode<int, 1>({0});

    auto s = l + r;
    EXPECT_EQ(1, s.intv());
    EXPECT_EQ(0, s.carry());
  }
}

TEST(Addition, OverflowingOneIntSevenBit)
{
  // Max container for 32 bits, carry bits are off
  auto l = std::numeric_limits<irregularia::multiple_int<7, std::uint8_t>>::max();
  auto r = irregularia::multiple_int<7, std::uint8_t>::encode<int, 1>({1});

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
  // Max container for 32 bits, carry bits are off
  auto l = std::numeric_limits<irregularia::multiple_int<7, std::uint32_t>>::max();

  // Will overflow on the lower two ints, but keep the upper two
  auto r = irregularia::multiple_int<7, std::uint32_t>::encode<int, 4>({0x00, 0x00, 0x01, 0x01});

  auto s = l + r;

  if constexpr (IRREGULARIA_BIT_CARRY_POLICY == 1) {
    EXPECT_EQ(0x7F'7F'00'00, s.intv());
    EXPECT_EQ(0x00'00'80'80, s.carry());
  }

  else
  {
    EXPECT_EQ(0x7F'7F'00'00, s.intv());
    EXPECT_EQ(0x00'00'00'00, s.carry());
  }
}


TEST(Max, TwoWay)
{
  using std::max;

  auto l = irregularia::multiple_int<7, std::uint32_t>::encode<int, 4>({0x10, 0x10, 0x01, 0x01});
  auto r = irregularia::multiple_int<7, std::uint32_t>::encode<int, 4>({0xEF, 0xEF, 0xEF, 0xEF});

  auto s = l + r;

  EXPECT_TRUE(l < r);
  EXPECT_EQ(r, max(l, r));

  EXPECT_TRUE(s < r);
  EXPECT_EQ(r, max(s, r))

  EXPECT_TRUE(s < l);
  EXPECT_EQ(l, max(s, l));
}