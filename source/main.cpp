#include <iostream>
#include <string>

#include "irregularia/cpu/hw.hpp"
#include "irregularia/cpu/multiple_int.hpp"
#include "irregularia/device/hw.cuh"

auto main() -> int
{
  hello_world_cpu();
  hello_world_device();
}
