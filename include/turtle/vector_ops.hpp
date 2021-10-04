#pragma once

#include "fwd.hpp"

#include <cmath>
#include <numeric>

namespace turtle {

template <kinematic::vector Vector>
constexpr auto dot_product(const Vector& v, const Vector& u) ->
    typename Vector::scalar
{
    using T = typename Vector::scalar;
    return std::inner_product(v.cbegin(), v.cend(), u.cbegin(), T{});
}

template <kinematic::vector Vector>
constexpr auto magnitude(const Vector& v) -> typename Vector::scalar
{
    return std::hypot(v.x(), v.y(), v.z());
}

template <kinematic::vector Vector>
constexpr auto normalized(const Vector& v) -> Vector
{
    return v / magnitude(v);
}

}  // namespace turtle
