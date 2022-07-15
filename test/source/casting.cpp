#include <gtest/gtest.h>
#include <irregularia/mi.hpp>

TEST(Casting, NoSignAndCarryBits)
{
  {
    auto l = irregularia::multiple_int<9, std::uint32_t>::encode<int, 3>({0b000000001, 0b010010011, 0b001111010});

    irregularia::multiple_int<19, std::uint64_t> t = l;          

    EXPECT_EQ(0b0000'00000000000'010010011'00000000000'000000001'00000000000'001111010, t.intv());
    EXPECT_EQ(0, t.carry());
  }
}
