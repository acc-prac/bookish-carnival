#include <gtest/gtest.h>
#include <irregularia/milimits.hpp>

using limsub1 = std::numeric_limits<irregularia::multiple_int<1, 1, std::uint8_t>>;
using limsub2 = std::numeric_limits<irregularia::multiple_int<1, 2, std::uint8_t>>;
using limsub3 = std::numeric_limits<irregularia::multiple_int<1, 3, std::uint8_t>>;
using limsub4 = std::numeric_limits<irregularia::multiple_int<1, 4, std::uint8_t>>;
using limsub5 = std::numeric_limits<irregularia::multiple_int<1, 5, std::uint8_t>>;
using limsub6 = std::numeric_limits<irregularia::multiple_int<1, 6, std::uint8_t>>;
using limsub7 = std::numeric_limits<irregularia::multiple_int<1, 7, std::uint8_t>>;

TEST(Limits, Max)
{
  EXPECT_EQ((static_cast<std::uint8_t>(1) << 1) - 1, limsub1::max().intv());
  EXPECT_EQ(0, limsub1::max().carry());

  EXPECT_EQ((static_cast<std::uint8_t>(1) << 2) - 1, limsub2::max().intv());
  EXPECT_EQ(0, limsub2::max().carry());

  EXPECT_EQ((static_cast<std::uint8_t>(1) << 3) - 1, limsub3::max().intv());
  EXPECT_EQ(0, limsub3::max().carry());

  EXPECT_EQ((static_cast<std::uint8_t>(1) << 4) - 1, limsub4::max().intv());
  EXPECT_EQ(0, limsub4::max().carry());

  EXPECT_EQ((static_cast<std::uint8_t>(1) << 5) - 1, limsub5::max().intv());
  EXPECT_EQ(0, limsub5::max().carry());

  EXPECT_EQ((static_cast<std::uint8_t>(1) << 6) - 1, limsub6::max().intv());
  EXPECT_EQ(0, limsub6::max().carry());

  EXPECT_EQ((static_cast<std::uint8_t>(1) << 7) - 1, limsub7::max().intv());
  EXPECT_EQ(0, limsub7::max().carry());
}

TEST(Limits, Min)
{
  EXPECT_EQ(0, limsub1::min().intv());
  EXPECT_EQ(0, limsub1::min().carry());

  EXPECT_EQ(0, limsub2::min().intv());
  EXPECT_EQ(0, limsub2::min().carry());

  EXPECT_EQ(0, limsub3::min().intv());
  EXPECT_EQ(0, limsub3::min().carry());

  EXPECT_EQ(0, limsub4::min().intv());
  EXPECT_EQ(0, limsub4::min().carry());

  EXPECT_EQ(0, limsub5::min().intv());
  EXPECT_EQ(0, limsub5::min().carry());

  EXPECT_EQ(0, limsub6::min().intv());
  EXPECT_EQ(0, limsub6::min().carry());

  EXPECT_EQ(0, limsub7::min().intv());
  EXPECT_EQ(0, limsub7::min().carry());
}