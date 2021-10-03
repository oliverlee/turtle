#pragma once

#include "turtle/fwd.hpp"

#include "boost/ut.hpp"

#include <concepts>

namespace turtle::test {

template <auto Tol, class T, class U>
requires std::same_as<T, U> && std::same_as<decltype(Tol), T>
auto within(const T& v, const U& u)
{
    using ut_value = boost::ut::detail::value<decltype(Tol)>;

    return boost::ut::eq(ut_value{v, Tol}, u);
}

template <auto Tol, turtle::con::reference_frame_vector Vector>
auto within(const Vector& v, const Vector& u)
{
    using boost::ut::operator and;
    using ut_value = boost::ut::detail::value<decltype(Tol)>;

    return boost::ut::eq(ut_value{v.x(), Tol}, u.x()) and
           boost::ut::eq(ut_value{v.y(), Tol}, u.y()) and
           boost::ut::eq(ut_value{v.z(), Tol}, u.z());
}

}  // namespace turtle::test
