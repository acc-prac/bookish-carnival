#include <gtest/gtest.h>
#include <irregularia/milimits.hpp>

#include <thrust/universal_vector.h>

using limsub1 = std::numeric_limits<irregularia::multiple_int<1>>;
using limsub2 = std::numeric_limits<irregularia::multiple_int<2>>;
using limsub3 = std::numeric_limits<irregularia::multiple_int<3>>;
using limsub4 = std::numeric_limits<irregularia::multiple_int<4>>;
using limsub5 = std::numeric_limits<irregularia::multiple_int<5>>;
using limsub6 = std::numeric_limits<irregularia::multiple_int<6>>;
using limsub7 = std::numeric_limits<irregularia::multiple_int<7>>;

TEST(Limits, Max)
{
  auto letters = "Hello World";
  thrust::universal_vector<char> v {letters,
                                    letters + std::size("Hello World") - 1};
  thrust::copy(v.begin(), v.end(), std::ostream_iterator<char>(std::cout, " "));

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