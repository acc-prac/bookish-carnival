#include <accbench_targets.hpp>
#include <benchmark/benchmark.h>

#include <vector>

static constexpr auto acc_par = std::execution::par_unseq;

template <auto exec, template <typename value_type> class Container, typename T>
static void xpy_bench(benchmark::State& state) {
  auto const n_elements = state.range(0);
  Container<T> const x(n_elements, T{1});
  Container<T> y(n_elements, T{2});

  for (auto _ : state) {
    acc::xpy(exec, x.cbegin(), x.cend(), y.cbegin(), y.begin());
  }
}
// needs to be first defined benchmark!
BENCHMARK(xpy_bench<acc_par, std::vector, int>)->Name("_warmup_")->Arg(1 << 28);
BENCHMARK(xpy_bench<acc_par, std::vector, int>)
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

template <auto exec, template <typename value_type> class Container, typename T>
static void elemwise_max_bench(benchmark::State& state) {
  auto const n_elements = state.range(0);
  Container<T> const x(n_elements, T{1});
  Container<T> y(n_elements, T{2});

  for (auto _ : state) {
    acc::elemwise_max(exec, x.cbegin(), x.cend(), y.cbegin(), y.begin());
  }
}
BENCHMARK(elemwise_max_bench<acc_par, std::vector, int>)
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

template <auto exec, template <typename value_type> class Container, typename T>
static void sum_red_bench(benchmark::State& state) {
  auto const n_elements = state.range(0);
  Container<T> const vals(n_elements, T{1});

  for (auto _ : state) {
    benchmark::DoNotOptimize(acc::sum_red(exec, vals.cbegin(), vals.cend()));
  }
}
BENCHMARK(sum_red_bench<acc_par, std::vector, int>)
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

template <auto exec, template <typename value_type> class Container, typename T>
static void max_red_bench(benchmark::State& state) {
  auto const n_elements = state.range(0);
  Container<T> const vals(n_elements, T{1});

  for (auto _ : state) {
    benchmark::DoNotOptimize(acc::max_red(exec, vals.cbegin(), vals.cend()));
  }
}
BENCHMARK(sum_red_bench<acc_par, std::vector, int>)
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

BENCHMARK_MAIN();
