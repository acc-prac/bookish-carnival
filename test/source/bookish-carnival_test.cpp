#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "bookish-carnival" ? 0 : 1;
}
