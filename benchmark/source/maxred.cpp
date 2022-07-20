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
         typename ComparingInteger>
static void max_red_int_bench(benchmark::State& state)
{
  auto const n_elements = state.range(0);
  Container<Integer> const vals(n_elements * sizeof(ComparingInteger) / sizeof(Integer),
                                Integer(1));

  for (auto _ : state) {
    benchmark::DoNotOptimize(acc::max_red(exec, vals.cbegin(), vals.cend()));
  }
}

template<auto exec, template<typename value_type> class Container, typename T>
static void max_red_multi_int_bench(benchmark::State& state)
{
  auto xs = array_repeat<T::IntCount, int>(1);

  auto const n_elements = state.range(0);
  Container<T> const vals(n_elements, T::encode<T::IntCount>(xs));

  for (auto _ : state) {
    benchmark::DoNotOptimize(acc::max_red(exec, vals.cbegin(), vals.cend()));
  }
}
BENCHMARK(max_red_multi_int_bench<host_par_unseq, thrust::host_vector, int>)
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);