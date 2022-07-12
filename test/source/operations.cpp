#include <gtest/gtest.h>
#include <irregularia/mi.hpp>

TEST(Operations, NonOverflowingAddition)
{
  {
    auto l = irregularia::multiple_int<1> {0};
    auto r = irregularia::multiple_int<1> {0};

    auto s = l + r;
    EXPECT_EQ(0, s.intv());
    EXPECT_EQ(0, s.carry());
  }

  {
    auto l = irregularia::multiple_int<1> {0};
    auto r = irregularia::multiple_int<1> {1};

    auto s = l + r;
    EXPECT_EQ(1, s.intv());
    EXPECT_EQ(0, s.carry());
  }

  {
    auto l = irregularia::multiple_int<1> {1};
    auto r = irregularia::multiple_int<1> {0};

    auto s = l + r;
    EXPECT_EQ(1, s.intv());
    EXPECT_EQ(0, s.carry());
  }
}

TEST(Operations, OverflowingAddition)
{
  {
    auto l = irregularia::multiple_int<1> {1};
    auto r = irregularia::multiple_int<1> {1};

    auto s = l + r;
    EXPECT_EQ(0, s.intv());
    EXPECT_EQ(1 << 1, s.carry());
  }
}