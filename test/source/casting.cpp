#include <gtest/gtest.h>
#include <irregularia/mi.hpp>

TEST(Casting, UpCast_NoSignBits)
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

TEST(Casting, UpCast_SignBits)
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

//DownCast
TEST(Casting, DownCast_NoCarryBits)
{
  {
    auto l = irregularia::multiple_int<7, std::uint16_t>::encode<int, 2>({0b1111101, 0b0000111});

    auto t = static_cast<irregularia::multiple_int<3, std::uint8_t>>(l);  

    EXPECT_EQ(0b0101'0111, t.intv());
    EXPECT_EQ(0, t.carry());
  }

  {
    auto l = irregularia::multiple_int<15, std::uint32_t>::encode<int, 2>({0b011101111101101, 0b001010100011000});

    auto t = static_cast<irregularia::multiple_int<7, std::uint16_t>>(l);  

    EXPECT_EQ(0b01101101'00011000, t.intv());
    EXPECT_EQ(0, t.carry());
  }

  {
    auto l = irregularia::multiple_int<9, std::uint64_t>::encode<int, 6>({0b110011001, 0b001100000, 
                                                                          0b101010101, 0b111111111,
                                                                          0b000000000, 0b100000001});

    auto t = static_cast<irregularia::multiple_int<4, std::uint32_t>>(l);  

    EXPECT_EQ(0b01001'00000'00101'01111'00000'00001, t.intv());
    EXPECT_EQ(0, t.carry());
  }
}

TEST(Casting, DownCast_WithCarryBits)
{
  {
    auto l = irregularia::multiple_int<7, std::uint16_t>::encode<int, 2>({0b1111101, 0b1000111});

    l = l + l; // Now we have 0b1111010'0001110 and both carry bits are set

    auto t = static_cast<irregularia::multiple_int<3, std::uint8_t>>(l);  

    EXPECT_EQ(0b0010'0110, t.intv());
    EXPECT_EQ(0b1000'1000, t.carry());
  }

  {
    auto l = irregularia::multiple_int<15, std::uint32_t>::encode<int, 2>({0b111101111101101, 0b110010100011000});

    l = l + l; // Now we have 0b111011111011010'100101000110000 and both carry bits are set

    auto t = static_cast<irregularia::multiple_int<7, std::uint16_t>>(l);  

    EXPECT_EQ(0b01011010'00110000, t.intv());
    EXPECT_EQ(0b10000000'10000000, t.carry());
  }

  {
    auto l = irregularia::multiple_int<9, std::uint64_t>::encode<int, 6>({0b110011001, 0b101100000,
                                                                          0b101010101, 0b111111111,
                                                                          0b100000000, 0b100000001});

    l = l + l; // Now we have 0b100110010'011000000'010101010'111111110'00000000'000000010 and all six carry bits are set

    auto t = static_cast<irregularia::multiple_int<4, std::uint32_t>>(l);  

    EXPECT_EQ(0b00'00010'00000'01010'01110'00000'00010, t.intv());
    EXPECT_EQ(0b00'10000'10000'10000'10000'10000'10000, t.carry());
  }
}
