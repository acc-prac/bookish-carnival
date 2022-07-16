#include <iostream>
#include <string>

#include <irregularia/mi.hpp>

auto main() -> int
{
  using std::max;

  auto l = irregularia::multiple_int<7, std::uint32_t>::encode<int, 4>(
      {0x10, 0x10, 0x01, 0x01});
  auto r = irregularia::multiple_int<7, std::uint32_t>::encode<int, 4>(
      {0xEF, 0xEF, 0xEF, 0xEF});

  auto s = l + r;
  auto max_lr = max(l, r);

  return max_lr == l;
}
