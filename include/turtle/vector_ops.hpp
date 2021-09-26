#pragma once

#include "fwd.hpp"

#include <cmath>
#include <numeric>

namespace turtle {

template <con::reference_frame_vector Vector>
constexpr auto dot_product(const Vector& v, const Vector& u) -> typename Vector::scalar_type
{
    using T = typename Vector::scalar_type;
    return std::inner_product(v.cbegin(), v.cend(), u.cbegin(), T{});
}

template <con::reference_frame_vector Vector>
constexpr auto magnitude(const Vector& v) -> typename Vector::scalar_type
{
    return std::hypot(v.x(), v.y(), v.z());
}

template <con::reference_frame_vector Vector>
constexpr auto normalized(const Vector& v) -> Vector
{
    return v / magnitude(v);
}


}  // namespace turtle
