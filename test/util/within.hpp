#pragma once

#include "turtle/fwd.hpp"

#include "boost/ut.hpp"

#include <concepts>

namespace turtle::test {

#ifdef __clang__

template <class T>
using tolerance_type_t = decltype(std::declval<T>().val);

template <class T>
struct floating_literal {
    constexpr floating_literal(T t) : val{t} {}

    operator T() const { return val; }

    T val;
};

#define AUTO floating_literal

#else

template <class T>
using tolerance_type_t = T;

#define AUTO auto

#endif

template <AUTO Tol, class T>
requires std::same_as<tolerance_type_t<decltype(Tol)>, T>
auto within(const T& v, const T& u)
{
    using ut_value = boost::ut::detail::value<T>;

    return boost::ut::eq(ut_value{v, Tol}, u);
}

// clang-format off
template <AUTO Tol, turtle::kinematic::vector V>
requires std::same_as <tolerance_type_t<decltype(Tol)>, typename V::scalar>
auto within(const V& v, const V& u)
{
    using boost::ut::operator and;
    using ut_value = boost::ut::detail::value<typename V::scalar>;

    return boost::ut::eq(ut_value{v.x(), Tol}, u.x()) and
           boost::ut::eq(ut_value{v.y(), Tol}, u.y()) and
           boost::ut::eq(ut_value{v.z(), Tol}, u.z());
}
// clang-format on

}  // namespace turtle::test
