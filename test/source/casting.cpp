#include <gtest/gtest.h>
#include <irregularia/mi.hpp>

TEST(Casting, NoSignAndNoCarryBits)
{
  {
    auto l = irregularia::multiple_int<3, std::uint8_t>::encode<int, 2>({0b011, 0b000});

    irregularia::multiple_int<7, std::uint16_t> t = l;          

    EXPECT_EQ(0b00000'011'00000'000, t.intv());
    EXPECT_EQ(0, t.carry());
  }

  {
    auto l = irregularia::multiple_int<5, std::uint16_t>::encode<int, 2>({0b01111, 0b01010});

    irregularia::multiple_int<11, std::uint32_t> t = l;          

    EXPECT_EQ(0b00000000'0000000'01111'0000000'01010, t.intv());
    EXPECT_EQ(0, t.carry());
  }

  {
    auto l = irregularia::multiple_int<9, std::uint32_t>::encode<int, 3>({0b000000001, 0b010010011, 0b001111010});

    irregularia::multiple_int<19, std::uint64_t> t = l;          

    EXPECT_EQ(0b0000'00000000000'010010011'00000000000'000000001'00000000000'001111010, t.intv());
    EXPECT_EQ(0, t.carry());
  }
}

TEST(Casting, SignButNoCarryBits)
{
  {
    auto l = irregularia::multiple_int<3, std::uint8_t>::encode<int, 2>({0b111, 0b100});

    irregularia::multiple_int<7, std::uint16_t> t = l;          

    EXPECT_EQ(0b01111'111'01111'100, t.intv());
    EXPECT_EQ(0, t.carry());
  }

  {
    auto l = irregularia::multiple_int<5, std::uint16_t>::encode<int, 2>({0b11111, 0b11010});

    irregularia::multiple_int<11, std::uint32_t> t = l;          

    EXPECT_EQ(0b00000000'0111111'11111'0111111'11010, t.intv());
    EXPECT_EQ(0, t.carry());
  }

  {
    auto l = irregularia::multiple_int<9, std::uint32_t>::encode<int, 3>({0b100000001, 0b110010011, 0b101111010});

    irregularia::multiple_int<19, std::uint64_t> t = l;          

    EXPECT_EQ(0b0000'01111111111'110010011'01111111111'100000001'01111111111'101111010, t.intv());
    EXPECT_EQ(0, t.carry());
  }
}
//This test does not work anymore because we reset all carry-bits
/*TEST(Casting, UpCast_SignAndCarryBits)
{
  {
    auto l = irregularia::multiple_int<3, std::uint8_t>::encode<int, 2>({0b111, 0b110});

    irregularia::multiple_int<7, std::uint16_t> t = l; 

    t = t + t;         

    EXPECT_EQ(0b01111'110'01111'100, t.intv());
    EXPECT_EQ(0b10000'000'10000'000, t.carry());
  }

  {
    auto l = irregularia::multiple_int<5, std::uint16_t>::encode<int, 2>({0b11111, 0b11010});

    irregularia::multiple_int<11, std::uint32_t> t = l;    

    t = t + t;         

    EXPECT_EQ(0b00000000'0111111'11110'0111111'10100, t.intv());
    EXPECT_EQ(0b00000000'10000000'0000'1000000'00000, t.carry());
  }

  {
    auto l = irregularia::multiple_int<9, std::uint32_t>::encode<int, 3>({0b110000000, 0b110010011, 0b111111010});

    irregularia::multiple_int<19, std::uint64_t> t = l;

    t = t + t;         

    EXPECT_EQ(0b0000'01111111111'100100110'01111111111'100000000'01111111111'111110100, t.intv());
    EXPECT_EQ(0b0000'10000000000'000000000'10000000000'000000000'10000000000'000000000, t.carry());
  }
}
*/