
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
         typename ComparingInteger>
static void sum_red_int_bench(benchmark::State& state)
{
  auto const n_elements = state.range(0);
  Container<Integer> const vals(n_elements * sizeof(ComparingInteger) / sizeof(Integer),
                                Integer(1));

  auto const init = Integer{};

  for (auto _ : state) {
    benchmark::DoNotOptimize(acc::sum_red(exec, vals.cbegin(), vals.cend(), init));
  }
}

template<auto exec, template<typename value_type> class Container, class T>
static void sum_red_multi_int_bench(benchmark::State& state)
{
  auto xs = array_repeat<T::IntCount, int>(1);

  auto const n_elements = state.range(0);
  Container<T> const vals(n_elements, T::encode<T::IntCount>(xs));

  for (auto _ : state) {
    benchmark::DoNotOptimize(acc::sum_red(exec, vals.cbegin(), vals.cend(), init));
  }
}

// needs to be first defined benchmark!
BENCHMARK(sum_red_int_bench<host_par_unseq, thrust::host_vector, int, std::uint32_t>)
    ->Name("_warmup_")
    ->Arg(1 << 28);
BENCHMARK(sum_red_int_bench<host_par_unseq,
                                 thrust::host_vector,
                                 std::uint32_t,
                                 std::uint64_t>)
    ->Name("sumred-host-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
BENCHMARK(sum_red_int_bench<device_par_unseq,
                                 thrust::device_vector,
                                 std::uint32_t,
                                 std::uint64_t>)
    ->Name("sumred-device-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

BENCHMARK(sum_red_multi_int_bench<host_par_unseq,
                                       thrust::host_vector,
                                       irregularia::multiple_int<31, std::uint64_t>>)
    ->Name("sumred-host-mi<31, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
BENCHMARK(sum_red_multi_int_bench<device_par_unseq,
                                       thrust::device_vector,
                                       irregularia::multiple_int<31, std::uint64_t>>)
    ->Name("sumred-device-mi<31, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

BENCHMARK_MAIN();
