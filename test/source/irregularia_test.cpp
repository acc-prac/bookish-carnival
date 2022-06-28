#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "irregularia" ? 0 : 1;
}
