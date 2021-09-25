#pragma once

#include <cstddef>
#include <ranges>
#include <utility>

namespace turtle::util {

namespace detail {

template <class ArrayLike, std::size_t... Is, class Iter>
constexpr auto make_arraylike_impl(Iter first, std::index_sequence<Is...>) -> ArrayLike
{
    return ArrayLike{(void(Is), *first++)...};
}

template <class ArrayLike, std::size_t... Is, std::ranges::random_access_range R>
constexpr auto make_arraylike_impl(R range, std::index_sequence<Is...>) -> ArrayLike
{
    return ArrayLike{range[Is]...};
}

}  // namespace detail

template <class ArrayLike, std::size_t N, class Iter>
constexpr auto make_arraylike(Iter first) -> ArrayLike
{
    return detail::make_arraylike_impl<ArrayLike>(first, std::make_index_sequence<N>{});
}

}  // namespace turtle::util
