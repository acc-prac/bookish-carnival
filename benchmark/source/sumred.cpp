#include <vector>

#include <benchmark/benchmark.h>
#include <irregularia/accbench_targets.hpp>
#include <irregularia/mi.hpp>
#include <thrust/device_vector.h>
#include <thrust/execution_policy.h>
#include <thrust/host_vector.h>

#include "util.hpp"


static constexpr auto host_par_unseq = thrust::host;
static constexpr auto device_par_unseq = thrust::cuda::par;

template<auto exec,
         template<typename value_type>
         class Container,
         typename Integer,
         typename ComparingInteger,
         unsigned int BitWidth>
static void sum_red_int_bench(benchmark::State& state)
{
  auto const n_elements = state.range(0);
  Container<Integer> const vals(n_elements * ((8 * sizeof(ComparingInteger)) / (BitWidth + 1)),
                                Integer(1));

  auto const init = Integer {};

  for (auto _ : state) {
    benchmark::DoNotOptimize(acc::sum_red(exec, vals.cbegin(), vals.cend(), init));
  }
}

template<auto exec, template<typename value_type> class Container, class T>
static void sum_red_multi_int_bench(benchmark::State& state)
{
  auto xs = array_repeat<T::IntCount, int>(1);
  auto is = array_repeat<T::IntCount, int>(0);

  auto const n_elements = state.range(0);
  Container<T> const vals(n_elements, T::encode<T::IntCount>(xs));
  auto const init = T::encode<T::IntCount>(is);

  for (auto _ : state) {
    benchmark::DoNotOptimize(acc::sum_red(exec, vals.cbegin(), vals.cend(), init));
  }
}

// needs to be first defined benchmark!
BENCHMARK(sum_red_int_bench<host_par_unseq, thrust::host_vector, int, std::uint32_t, 32>)
    ->Name("_warmup_")
    ->Arg(1 << 28);
    
//For the 1 bit variant less elements have to be used, otherwise OOM occurs

//64 bit internal storage
//--------------------------------------------------------------------------------------------//

/*BENCHMARK(sum_red_int_bench<host_par_unseq, thrust::host_vector, std::uint32_t, std::uint64_t, 31>)
    ->Name("sumred-host-1-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/

BENCHMARK(
    sum_red_int_bench<device_par_unseq, thrust::device_vector, std::uint32_t, std::uint64_t, 31>)
    ->Name("sumred-device-1-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

/*BENCHMARK(sum_red_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<31, std::uint64_t>>)
    ->Name("sumred-host-1-mi<31, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/
    
BENCHMARK(sum_red_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<31, std::uint64_t>>)
    ->Name("sumred-device-1-mi<31, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
    
/*BENCHMARK(sum_red_int_bench<host_par_unseq, thrust::host_vector, std::uint32_t, std::uint64_t, 17>)
    ->Name("sumred-host-1-u32x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/

BENCHMARK(
    sum_red_int_bench<device_par_unseq, thrust::device_vector, std::uint32_t, std::uint64_t, 17>)
    ->Name("sumred-device-1-u32x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

/*BENCHMARK(sum_red_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<17, std::uint64_t>>)
    ->Name("sumred-host-1-mi<17, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/
    
BENCHMARK(sum_red_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<17, std::uint64_t>>)
    ->Name("sumred-device-1-mi<17, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
    
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

/*BENCHMARK(sum_red_int_bench<host_par_unseq, thrust::host_vector, std::uint8_t, std::uint64_t, 7>)
    ->Name("sumred-host-2-u8x8")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/

BENCHMARK(
    sum_red_int_bench<device_par_unseq, thrust::device_vector, std::uint8_t, std::uint64_t, 7>)
    ->Name("sumred-device-2-u8x8")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);

/*BENCHMARK(sum_red_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<7, std::uint64_t>>)
    ->Name("sumred-host-2-mi<7, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/
    
BENCHMARK(sum_red_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<7, std::uint64_t>>)
    ->Name("sumred-device-2-mi<7, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);
    
/*BENCHMARK(sum_red_int_bench<host_par_unseq, thrust::host_vector, std::uint8_t, std::uint64_t, 1>)
    ->Name("sumred-host-2-u8x32")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/

BENCHMARK(
    sum_red_int_bench<device_par_unseq, thrust::device_vector, std::uint8_t, std::uint64_t, 1>)
    ->Name("sumred-device-2-u8x32")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);

/*BENCHMARK(sum_red_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<1, std::uint64_t>>)
    ->Name("sumred-host-2-mi<1, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/
    
BENCHMARK(sum_red_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<1, std::uint64_t>>)
    ->Name("sumred-device-2-mi<1, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);
    
//--------------------------------------------------------------------------------------------//

//32 bit internal storage
//--------------------------------------------------------------------------------------------//

/*BENCHMARK(sum_red_int_bench<host_par_unseq, thrust::host_vector, std::uint16_t, std::uint32_t, 15>)
    ->Name("sumred-host-3-u16x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/

BENCHMARK(
    sum_red_int_bench<device_par_unseq, thrust::device_vector, std::uint16_t, std::uint32_t, 15>)
    ->Name("sumred-device-3-u16x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

/*BENCHMARK(sum_red_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<15, std::uint32_t>>)
    ->Name("sumred-host-3-mi<15, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/
    
BENCHMARK(sum_red_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<15, std::uint32_t>>)
    ->Name("sumred-device-3-mi<15, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
    
/*BENCHMARK(sum_red_int_bench<host_par_unseq, thrust::host_vector, std::uint16_t, std::uint32_t, 9>)
    ->Name("sumred-host-3-u16x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/

BENCHMARK(
    sum_red_int_bench<device_par_unseq, thrust::device_vector, std::uint16_t, std::uint32_t, 9>)
    ->Name("sumred-device-3-u16x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

/*BENCHMARK(sum_red_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<9, std::uint32_t>>)
    ->Name("sumred-host-3-mi<9, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/
    
BENCHMARK(sum_red_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<9, std::uint32_t>>)
    ->Name("sumred-device-3-mi<9, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
    
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

/*BENCHMARK(sum_red_int_bench<host_par_unseq, thrust::host_vector, std::uint8_t, std::uint32_t, 7>)
    ->Name("sumred-host-4-u8x4")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/

BENCHMARK(
    sum_red_int_bench<device_par_unseq, thrust::device_vector, std::uint8_t, std::uint32_t, 7>)
    ->Name("sumred-device-4-u8x4")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);

/*BENCHMARK(sum_red_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<7, std::uint32_t>>)
    ->Name("sumred-host-4-mi<7, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/
    
BENCHMARK(sum_red_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<7, std::uint32_t>>)
    ->Name("sumred-device-4-mi<7, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);
    
/*BENCHMARK(sum_red_int_bench<host_par_unseq, thrust::host_vector, std::uint8_t, std::uint32_t, 1>)
    ->Name("sumred-host-4-u8x16")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/

BENCHMARK(
    sum_red_int_bench<device_par_unseq, thrust::device_vector, std::uint8_t, std::uint32_t, 1>)
    ->Name("sumred-device-4-u8x16")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);

/*BENCHMARK(sum_red_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<1, std::uint32_t>>)
    ->Name("sumred-host-4-mi<1, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/
    
BENCHMARK(sum_red_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<1, std::uint32_t>>)
    ->Name("sumred-device-4-mi<1, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);
    
//--------------------------------------------------------------------------------------------//

BENCHMARK_MAIN();
