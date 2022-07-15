#include <gtest/gtest.h>
#include <irregularia/mi.hpp>

TEST(Casting, NoSignAndCarryBits)
{
  {
    auto l = irregularia::multiple_int<9, std::uint32_t>::encode({0b000000001, 0b010010011, 0b001111010});

    multiple_int<19, std::uint64_t> t = l;          

    EXPECT_EQ(0b0000'01111111111'110010011'01111111111'100000001'00000000000'001111010, l.intv());
    EXPECT_EQ(0, l.carry());
  }
}
