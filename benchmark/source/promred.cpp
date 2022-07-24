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
         typename SmallerInteger,
         typename BiggerInteger,
         typename ComparingInteger,
         unsigned int BitWidth>
static void sum_prom_int_bench(benchmark::State& state)
{
  auto const n_elements = state.range(0);
  Container<SmallerInteger> const vals(
      n_elements * ((8 * sizeof(ComparingInteger)) / (BitWidth + 1)), SmallerInteger(1));

  auto const init = BiggerInteger {};
  for (auto _ : state) {
    benchmark::DoNotOptimize(acc::sum_red(exec, vals.cbegin(), vals.cend(), init));
  }
}

template<auto exec,
         template<typename value_type>
         class Container,
         typename SmallerInteger,
         typename BiggerInteger>
static void sum_red_multi_int_bench(benchmark::State& state)
{
  auto xs = array_repeat<SmallerInteger::IntCount, int>(1);
  auto is = array_repeat<BiggerInteger::IntCount, int>(0);

  auto const n_elements = state.range(0);
  Container<SmallerInteger> const vals(n_elements, T::encode<T::IntCount>(xs));
  auto const init = BiggerInteger::encode<BiggerInteger::IntCount>(is);

  for (auto _ : state) {
    auto sum = acc::sum_red(exec, vals.cbegin(), vals.cend(), init);
    auto subsums = sum.decode();

    benchmark::DoNotOptimize(std::accumulate(subsums.cbegin(), subsums.cend()));
  }
}

BENCHMARK(sum_prom_int_bench<host_par_unseq,
                             thrust::host_vector,
                             std::uint32_t,
                             std::uint64_t,
                             std::uint64_t,
                             32>)
    ->Name("_warmup_")
    ->Arg(1 << 28);

BENCHMARK(sum_red_multi_int_bench<device_par_unseq,
                                  thrust::device_vector,
                                  irregularia::multiple_int<7, std::uint32_t>,
                                  irregularia::multiple_int<15, std::uint64_t>>)
    ->Name("sumred-device-1-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

BENCHMARK_MAIN();