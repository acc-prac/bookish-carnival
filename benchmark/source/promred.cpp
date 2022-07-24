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

BENCHMARK(sum_prom_int_bench<host_par_unseq,
                             thrust::host_vector,
                             std::uint32_t,
                             std::uint64_t,
                             std::uint64_t,
                             32>)
    ->Name("_warmup_")
    ->Arg(1 << 28);