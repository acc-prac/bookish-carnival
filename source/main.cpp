#include <iostream>
#include <string>

#include "../include/cpu/hw.hpp"
#include "../include/device/hw.cuh"

auto main() -> int
{ 
    hello_world_cpu();
    hello_world_device();
}
