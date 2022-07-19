#include <vector>

#include <benchmark/benchmark.h>
#include <irregularia/accbench_targets.hpp>
#include <irregularia/mi.hpp>


#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/execution_policy.h>

static constexpr auto host_par_unseq = thrust::host;
static constexpr auto device_par_unseq = thrust::cuda::par;

template<auto exec, template<typename value_type> class Container, typename T>
static void xpy_bench(benchmark::State& state)
{
  auto const n_elements = state.range(0);
  Container<T> const x(n_elements, T {1});
  Container<T> y(n_elements, T {2});

  for (auto _ : state) {
    acc::xpy(exec, x.cbegin(), x.cend(), y.cbegin(), y.begin());
  }
}
// needs to be first defined benchmark!
BENCHMARK(xpy_bench<host_par_unseq, thrust::host_vector, int>)->Name("_warmup_")->Arg(1 << 28);
BENCHMARK(xpy_bench<host_par_unseq, thrust::host_vector, int>)
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
BENCHMARK(xpy_bench<device_par_unseq, thrust::device_vector, int>)
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

template<auto exec, template<typename value_type> class Container, typename T>
static void elemwise_max_bench(benchmark::State& state)
{
  auto const n_elements = state.range(0);
  Container<T> const x(n_elements, T {1});
  Container<T> y(n_elements, T {2});

  for (auto _ : state) {
    acc::elemwise_max(exec, x.cbegin(), x.cend(), y.cbegin(), y.begin());
  }
}
BENCHMARK(elemwise_max_bench<host_par_unseq, thrust::host_vector, int>)
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

template<auto exec, template<typename value_type> class Container, typename T>
static void sum_red_bench(benchmark::State& state)
{
  auto const n_elements = state.range(0);
  Container<T> const vals(n_elements, T {1});

  auto const init = T {};

  for (auto _ : state) {
    benchmark::DoNotOptimize(acc::sum_red(exec, vals.cbegin(), vals.cend(), init));
  }
}
BENCHMARK(sum_red_bench<host_par_unseq, thrust::host_vector, int>)
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

template<auto exec, template<typename value_type> class Container, typename T>
static void max_red_bench(benchmark::State& state)
{
  auto const n_elements = state.range(0);
  Container<T> const vals(n_elements, T {1});

  for (auto _ : state) {
    benchmark::DoNotOptimize(acc::max_red(exec, vals.cbegin(), vals.cend()));
  }
}
BENCHMARK(sum_red_bench<host_par_unseq, thrust::host_vector, int>)
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

BENCHMARK_MAIN();
