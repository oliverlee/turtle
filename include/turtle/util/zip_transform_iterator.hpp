#pragma once

#include <iterator>
#include <type_traits>

namespace turtle::util {

template <class Iter1, class Iter2, class BinOp>
struct zip_transform_iterator {
    Iter1 first1;
    Iter2 first2;
    BinOp bop;

    using iterator_category = std::input_iterator_tag;
    using value_type =
        std::invoke_result_t<BinOp,
                             std::iter_reference_t<Iter1>,
                             std::iter_reference_t<Iter2>>;
    using difference_type =
        std::common_type_t<std::iter_difference_t<Iter1>,
                           std::iter_difference_t<Iter2>>;
    using pointer = void;
    using reference = void;

    using iterator = zip_transform_iterator;

    constexpr auto operator*() -> value_type { return bop(*first1, *first2); }

    constexpr auto operator++() & -> iterator&
    {
        ++first1;
        ++first2;
        return *this;
    }
    constexpr auto operator++(int) -> iterator
    {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    friend constexpr auto
    operator==(const iterator& lhs, const iterator& rhs) noexcept -> bool
    {
        return (lhs.first1 == rhs.first1) && (lhs.first2 == rhs.first2);
    }
};

}  // namespace turtle::util
