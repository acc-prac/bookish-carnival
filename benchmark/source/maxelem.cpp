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
static void elemwise_max_int_bench(benchmark::State& state)
{
  auto const n_elements = state.range(0);
  Container<Integer> const x(n_elements * ((8 * sizeof(ComparingInteger)) / (BitWidth + 1)),
                             Integer(1));

  // e.g. creating std::uint8_t, but multiple_int pendant calculates in std::uint32_t -> 4
  // more elements for same "size"
  Container<Integer> y(n_elements * ((8 * sizeof(ComparingInteger)) / (BitWidth + 1)),
                       Integer(2));

  for (auto _ : state) {
    acc::elemwise_max(exec, x.cbegin(), x.cend(), y.cbegin(), y.begin());
  }
}

template<auto exec, template<typename value_type> class Container, class T>
static void elemwise_max_multi_int_bench(benchmark::State& state)
{
  auto xs = array_repeat<T::IntCount, int>(1);
  auto ys = array_repeat<T::IntCount, int>(2);

  auto const n_elements = state.range(0);
  Container<T> const x(n_elements, T::encode<T::IntCount>(xs));
  Container<T> y(n_elements, T::encode<T::IntCount>(ys));

  for (auto _ : state) {
    acc::elemwise_max(exec, x.cbegin(), x.cend(), y.cbegin(), y.begin());
  }
}

// needs to be first defined benchmark!
BENCHMARK(elemwise_max_int_bench<host_par_unseq, thrust::host_vector, int, std::uint32_t, 32>)
    ->Name("_warmup_")
    ->Arg(1 << 28);
    
//For the 1 bit variant less elements have to be used, otherwise OOM occurs

//64 bit internal storage
//--------------------------------------------------------------------------------------------//

/*BENCHMARK(elemwise_max_int_bench<host_par_unseq, thrust::host_vector, std::uint32_t, std::uint64_t, 31>)
    ->Name("maxelem-host-1-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/

BENCHMARK(
    elemwise_max_int_bench<device_par_unseq, thrust::device_vector, std::uint32_t, std::uint64_t, 31>)
    ->Name("maxelem-device-1-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

/*BENCHMARK(elemwise_max_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<31, std::uint64_t>>)
    ->Name("maxelem-host-1-mi<31, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/
    
BENCHMARK(elemwise_max_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<31, std::uint64_t>>)
    ->Name("maxelem-device-1-mi<31, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
    
/*BENCHMARK(elemwise_max_int_bench<host_par_unseq, thrust::host_vector, std::uint32_t, std::uint64_t, 17>)
    ->Name("maxelem-host-1-u32x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/

BENCHMARK(
    elemwise_max_int_bench<device_par_unseq, thrust::device_vector, std::uint32_t, std::uint64_t, 17>)
    ->Name("maxelem-device-1-u32x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

/*BENCHMARK(elemwise_max_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<17, std::uint64_t>>)
    ->Name("maxelem-host-1-mi<17, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/
    
BENCHMARK(elemwise_max_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<17, std::uint64_t>>)
    ->Name("maxelem-device-1-mi<17, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
    
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

/*BENCHMARK(elemwise_max_int_bench<host_par_unseq, thrust::host_vector, std::uint8_t, std::uint64_t, 7>)
    ->Name("maxelem-host-2-u8x8")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/

BENCHMARK(
    elemwise_max_int_bench<device_par_unseq, thrust::device_vector, std::uint8_t, std::uint64_t, 7>)
    ->Name("maxelem-device-2-u8x8")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);

/*BENCHMARK(elemwise_max_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<7, std::uint64_t>>)
    ->Name("maxelem-host-2-mi<7, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/
    
BENCHMARK(elemwise_max_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<7, std::uint64_t>>)
    ->Name("maxelem-device-2-mi<7, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);
    
/*BENCHMARK(elemwise_max_int_bench<host_par_unseq, thrust::host_vector, std::uint8_t, std::uint64_t, 1>)
    ->Name("maxelem-host-2-u8x32")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/

BENCHMARK(
    elemwise_max_int_bench<device_par_unseq, thrust::device_vector, std::uint8_t, std::uint64_t, 1>)
    ->Name("maxelem-device-2-u8x32")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);

/*BENCHMARK(elemwise_max_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<1, std::uint64_t>>)
    ->Name("maxelem-host-2-mi<1, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/
    
BENCHMARK(elemwise_max_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<1, std::uint64_t>>)
    ->Name("maxelem-device-2-mi<1, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);
    
//--------------------------------------------------------------------------------------------//

//32 bit internal storage
//--------------------------------------------------------------------------------------------//

/*BENCHMARK(elemwise_max_int_bench<host_par_unseq, thrust::host_vector, std::uint16_t, std::uint32_t, 15>)
    ->Name("maxelem-host-3-u16x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/

BENCHMARK(
    elemwise_max_int_bench<device_par_unseq, thrust::device_vector, std::uint16_t, std::uint32_t, 15>)
    ->Name("maxelem-device-3-u16x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

/*BENCHMARK(elemwise_max_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<15, std::uint32_t>>)
    ->Name("maxelem-host-3-mi<15, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/
    
BENCHMARK(elemwise_max_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<15, std::uint32_t>>)
    ->Name("maxelem-device-3-mi<15, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
    
/*BENCHMARK(elemwise_max_int_bench<host_par_unseq, thrust::host_vector, std::uint16_t, std::uint32_t, 9>)
    ->Name("maxelem-host-3-u16x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/

BENCHMARK(
    elemwise_max_int_bench<device_par_unseq, thrust::device_vector, std::uint16_t, std::uint32_t, 9>)
    ->Name("maxelem-device-3-u16x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

/*BENCHMARK(elemwise_max_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<9, std::uint32_t>>)
    ->Name("maxelem-host-3-mi<9, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);*/
    
BENCHMARK(elemwise_max_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<9, std::uint32_t>>)
    ->Name("maxelem-device-3-mi<9, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
    
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

/*BENCHMARK(elemwise_max_int_bench<host_par_unseq, thrust::host_vector, std::uint8_t, std::uint32_t, 7>)
    ->Name("maxelem-host-4-u8x4")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/

BENCHMARK(
    elemwise_max_int_bench<device_par_unseq, thrust::device_vector, std::uint8_t, std::uint32_t, 7>)
    ->Name("maxelem-device-4-u8x4")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);

/*BENCHMARK(elemwise_max_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<7, std::uint32_t>>)
    ->Name("maxelem-host-4-mi<7, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/
    
BENCHMARK(elemwise_max_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<7, std::uint32_t>>)
    ->Name("maxelem-device-4-mi<7, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);
    
/*BENCHMARK(elemwise_max_int_bench<host_par_unseq, thrust::host_vector, std::uint8_t, std::uint32_t, 1>)
    ->Name("maxelem-host-4-u8x16")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/

BENCHMARK(
    elemwise_max_int_bench<device_par_unseq, thrust::device_vector, std::uint8_t, std::uint32_t, 1>)
    ->Name("maxelem-device-4-u8x16")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);

/*BENCHMARK(elemwise_max_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<1, std::uint32_t>>)
    ->Name("maxelem-host-4-mi<1, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);*/
    
BENCHMARK(elemwise_max_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<1, std::uint32_t>>)
    ->Name("maxelem-device-4-mi<1, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 24);
    
//--------------------------------------------------------------------------------------------//

BENCHMARK_MAIN();
