#include <vector>

#include <benchmark/benchmark.h>
#include <irregularia/accbench_targets.hpp>
#include <irregularia/mi.hpp>
#include <thrust/device_vector.h>
#include <thrust/execution_policy.h>
#include <thrust/host_vector.h>

static constexpr auto host_par_unseq = thrust::host;
static constexpr auto device_par_unseq = thrust::cuda::par;

// https://stackoverflow.com/a/63821008
namespace detail
{

template<size_t, class T>
constexpr T&& identity(T&& x)
{
  return std::forward<T>(x);
}

template<class T, size_t... Indices>
constexpr auto array_repeat_impl(T&& x, std::index_sequence<Indices...>)
{
  return std::array {identity<Indices>(x)...};
}

}  // namespace detail

template<size_t N, class T>
constexpr auto array_repeat(T&& x) -> std::array<T, N>
{
  return detail::array_repeat_impl(std::forward<T>(x), std::make_index_sequence<N>());
}

template<auto exec, template<typename value_type> class Container, typename T>
static void xpy_bench(benchmark::State& state);

template<auto exec, template<typename value_type> class Container, class T>
static void xpy_multi_int_bench(benchmark::State& state)
{
  auto xs = array_repeat<T::IntCount, int>(1);
  auto ys = array_repeat<T::IntCount, int>(2);

  auto const n_elements = state.range(0);
  Container<T> const x(n_elements, T::encode<T::IntCount>(xs));
  Container<T> y(n_elements, T::encode<T::IntCount>(ys));

  for (auto _ : state) {
    acc::xpy(exec, x.cbegin(), x.cend(), y.cbegin(), y.begin());
  }
}

template<auto exec,
         template<typename value_type>
         class Container,
         typename Integer,
         typename ComparingInteger>
static void xpy_int_bench(benchmark::State& state)
{
  auto const n_elements = state.range(0);
  Container<Integer> const x(n_elements * sizeof(ComparingInteger) / sizeof(Integer),
                             Integer(1));

  // e.g. creating std::uint8_t, but multiple_int pendant calculates in std::uint32_t -> 4
  // more elements for same "size"
  Container<Integer> y(n_elements * sizeof(ComparingInteger) / sizeof(Integer),
                       Integer(2));

  for (auto _ : state) {
    acc::xpy(exec, x.cbegin(), x.cend(), y.cbegin(), y.begin());
  }
}
// needs to be first defined benchmark!
BENCHMARK(xpy_int_bench<host_par_unseq, thrust::host_vector, int, std::uint32_t>)
    ->Name("_warmup_")
    ->Arg(1 << 28);

BENCHMARK(xpy_int_bench<host_par_unseq, thrust::host_vector, std::uint32_t, std::uint64_t>)
    ->Name("xpy-host-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
BENCHMARK(
    xpy_int_bench<device_par_unseq, thrust::device_vector, std::uint32_t, std::uint64_t>)
    ->Name("xpy-device-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);

BENCHMARK(xpy_multi_int_bench<host_par_unseq,
                              thrust::host_vector,
                              irregularia::multiple_int<31, std::uint64_t>>)
    ->Name("xpy-host-mi<31, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
BENCHMARK(xpy_multi_int_bench<device_par_unseq,
                              thrust::device_vector,
                              irregularia::multiple_int<31, std::uint64_t>>)
    ->Name("xpy-device-mi<31, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 28);
    
/*
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
    ->Range(1 << 14, 1 << 28); */

BENCHMARK_MAIN();
