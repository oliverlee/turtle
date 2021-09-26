#pragma once

#include "fwd.hpp"

#include <numeric>

namespace turtle {

template <con::reference_frame_vector Vector>
constexpr auto dot_product(const Vector& v, const Vector& u) -> typename Vector::scalar_type
{
    using T = typename Vector::scalar_type;
    return std::inner_product(v.cbegin(), v.cend(), u.cbegin(), T{});
}

}  // namespace turtle
