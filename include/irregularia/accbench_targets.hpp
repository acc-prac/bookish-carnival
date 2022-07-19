#pragma once

#include <algorithm>
#include <execution>
#include <iterator>
#include <numeric>

#include <thrust/transform.h>
#include <thrust/reduce.h>

namespace acc
{

template<class Exec, class InputIterator1, class InputIterator2, class OutputIterator>
void xpy(Exec&& exec,
         InputIterator1 x_b,
         InputIterator1 x_e,
         InputIterator2 y_b,
         OutputIterator z_b)
{
  using X = std::iterator_traits<InputIterator1>::value_type;
  using Y = std::iterator_traits<InputIterator2>::value_type;
  using Z = std::iterator_traits<OutputIterator>::value_type;
  thrust::transform(std::forward<Exec>(exec),
                 x_b,
                 x_e,
                 y_b,
                 z_b,
                 [](X const& x, Y const& y) -> Z { return x + y; });
}

template<class Exec, class InputIterator1, class InputIterator2, class OutputIterator>
void elemwise_max(Exec&& exec,
                  InputIterator1 x_b,
                  InputIterator1 x_e,
                  InputIterator2 y_b,
                  OutputIterator z_b)
{
  using X = std::iterator_traits<InputIterator1>::value_type;
  using Y = std::iterator_traits<InputIterator2>::value_type;
  using Z = std::iterator_traits<OutputIterator>::value_type;
  using std::max;  // customization point
  thrust::transform(std::forward<Exec>(exec),
                 x_b,
                 x_e,
                 y_b,
                 z_b,
                 [](X const& x, Y const& y) -> Z { return max(x, y); });
}

template<class Exec, class InputIterator, typename T>
auto sum_red(Exec&& exec, InputIterator vals_b, InputIterator vals_e, T init)
{
  return thrust::reduce(std::forward<Exec>(exec), vals_b, vals_e, init);
}

template<class Exec, class InputIterator>
auto max_red(Exec&& exec, InputIterator vals_b, InputIterator vals_e)
{
  using T = std::iterator_traits<InputIterator>::value_type;
  using std::max;  // customization point
  using std::numeric_limits;  // customization point
  return thrust::reduce(std::forward<Exec>(exec),
                     vals_b,
                     vals_e,
                     numeric_limits<T>::lowest,
                     [](T const& x, T const& y) -> T { return max(x, y); });
}

}  // namespace acc
