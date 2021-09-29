#pragma once

#include <bit>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>

namespace turtle::util {

namespace detail {
template <class Int, class T>
constexpr auto ulp_diff(const T& t, const T& u) -> std::size_t
{
    assert(std::isfinite(t) and std::isfinite(u));

    const auto a = (std::signbit(t) == std::signbit(u)) ? -1 : 1;
    const auto i = std::bit_cast<Int>(std::abs(t));
    const auto j = std::bit_cast<Int>(std::abs(u));

    return static_cast<std::size_t>(std::abs(i + (a * j)));
}
}  // namespace detail

template <class T, class U>
requires std::same_as<T, U> &&(4 == sizeof(T)) constexpr auto ulp_diff(const T& t, const U& u)
    -> std::size_t
{
    return detail::ulp_diff<std::int32_t>(t, u);
}

template <class T, class U>
requires std::same_as<T, U> &&(8 == sizeof(T)) constexpr auto ulp_diff(const T& t, const U& u)
    -> std::size_t
{
    return detail::ulp_diff<std::int64_t>(t, u);
}

}  // namespace turtle::util
