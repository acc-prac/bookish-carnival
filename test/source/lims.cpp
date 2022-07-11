#include <gtest/gtest.h>
#include <irregularia/milimits.hpp>

using limsub1 = std::numeric_limits<irregularia::multiple_int<1>>;
using limsub2 = std::numeric_limits<irregularia::multiple_int<2>>;
using limsub3 = std::numeric_limits<irregularia::multiple_int<3>>;
using limsub4 = std::numeric_limits<irregularia::multiple_int<4>>;
using limsub5 = std::numeric_limits<irregularia::multiple_int<5>>;
using limsub6 = std::numeric_limits<irregularia::multiple_int<6>>;
using limsub7 = std::numeric_limits<irregularia::multiple_int<7>>;

TEST(Limits, Max)
{
  EXPECT_EQ((static_cast<std::uint8_t>(1) << 1) - 1, limsub1::max());
  EXPECT_EQ((static_cast<std::uint8_t>(1) << 2) - 1, limsub2::max());
  EXPECT_EQ((static_cast<std::uint8_t>(1) << 3) - 1, limsub3::max());
  EXPECT_EQ((static_cast<std::uint8_t>(1) << 4) - 1, limsub4::max());
  EXPECT_EQ((static_cast<std::uint8_t>(1) << 5) - 1, limsub5::max());
  EXPECT_EQ((static_cast<std::uint8_t>(1) << 6) - 1, limsub6::max());
  EXPECT_EQ((static_cast<std::uint8_t>(1) << 7) - 1, limsub7::max());
}