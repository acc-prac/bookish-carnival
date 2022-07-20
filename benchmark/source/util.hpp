#pragma once

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
