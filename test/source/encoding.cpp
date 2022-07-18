#include <gtest/gtest.h>
#include <irregularia/mi.hpp>

TEST(Encoding, NonTruncating)
{
  {
    auto l = irregularia::multiple_int<7, std::uint8_t>::encode<int, 1>({0b01111111});

    EXPECT_EQ(127, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  {
    auto l = irregularia::multiple_int<7, std::uint16_t>::encode<int, 2>(
        {0b01111111, 0b01110001});

    EXPECT_EQ(0b0111111101110001, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  {
    auto l = irregularia::multiple_int<7, std::uint32_t>::encode<int, 4>(
        {0b01111111, 0b01110001, 0b01111111, 0b01110001});

    EXPECT_EQ(0b01111111011100010111111101110001, l.intv());
    EXPECT_EQ(0, l.carry());
  }
}

TEST(Encoding, Truncating)
{
  {
    auto l = irregularia::multiple_int<7, std::uint8_t>::encode<int, 1>({0b11111111});

    EXPECT_EQ(127, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  {
    auto l = irregularia::multiple_int<7, std::uint16_t>::encode<int, 2>(
        {0b11111111, 0b11110001});

    EXPECT_EQ(0b0111111101110001, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  {
    auto l = irregularia::multiple_int<7, std::uint32_t>::encode<int, 4>(
        {0b11111111, 0b11110001, 0b11111111, 0b11110001});

    EXPECT_EQ(0b01111111011100010111111101110001, l.intv());
    EXPECT_EQ(0, l.carry());
  }
}
