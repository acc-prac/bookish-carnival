#include <gtest/gtest.h>
#include <irregularia/mitraits.hpp>

using OneIntOneBit =
    irregularia::detail::_multiple_int_traits<1, 1, std::uint8_t>;

using ThreeInts7Bit =
    irregularia::detail::_multiple_int_traits<3, 7, std::uint32_t>;

TEST(Traits, Masks) 
{
    EXPECT_EQ(0b11111110, OneIntOneBit::carry_mask);
    EXPECT_EQ(0x707070, ThreeInts7Bit::carry_mask);
}