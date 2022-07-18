#include <gtest/gtest.h>
#include <irregularia/mi.hpp>

TEST(Decoding, SignedAddition)
{
  {
    auto l = irregularia::multiple_int<3, std::uint8_t>::encode<2>({2, 1});
    auto r = irregularia::multiple_int<3, std::uint8_t>::encode<2>({-2, 0});

    auto s = l + r;

    EXPECT_EQ(0, s.decode()[0]);
    EXPECT_EQ(1, s.decode()[1]);
  }

  {
    auto l = irregularia::multiple_int<3, std::uint16_t>::encode<3>({2, 1, -2});
    auto r = irregularia::multiple_int<3, std::uint16_t>::encode<3>({-2, 1, -2});

    auto s = l + r;

    EXPECT_EQ(0, s.decode()[0]);
    EXPECT_EQ(2, s.decode()[1]);
    EXPECT_EQ(-4, s.decode()[2]);
  }

  {
    auto l = irregularia::multiple_int<7, std::uint32_t>::encode<2>({63, 23});
    auto r = irregularia::multiple_int<7, std::uint32_t>::encode<2>({-1, 15});

    auto s = l + r;

    EXPECT_EQ(62, s.decode()[0]);
    EXPECT_EQ(38, s.decode()[1]);
  }
}