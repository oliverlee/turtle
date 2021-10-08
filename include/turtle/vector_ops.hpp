#pragma once

#include "fwd.hpp"

#include <cmath>
#include <numeric>

namespace turtle {

/// @brief Calculates the vector dot product
/// @tparam V Kinematic vector type
/// @param v, u Vector values
/// @return Vector dot product as a scalar value
template <kinematic::vector V>
constexpr auto dot_product(const V& v, const V& u) -> typename V::scalar
{
    using T = typename V::scalar;
    return std::inner_product(v.cbegin(), v.cend(), u.cbegin(), T{});
}

/// @brief Calculates the vector cross product
/// @tparam V Kinematic vector type
/// @param v, u Vector values
/// @return Vector cross product as a vector value
template <kinematic::vector V>
constexpr auto cross_product(const V& v, const V& u) -> V
{
    const auto qx = quaternion{v} * quaternion{u};
    return {qx.x(), qx.y(), qx.z()};
}

/// @brief Calculates the vector norm
/// @tparam V Kinematic vector type
/// @param v Vector value
/// @return Vector norm as a scalar value
template <kinematic::vector V>
constexpr auto norm(const V& v) -> typename V::scalar
{
    return std::hypot(v.x(), v.y(), v.z());
}

/// @brief Returns the normalized vector
/// @tparam V Kinematic vector type
/// @param v Vector to normalize
/// @return Vector with the same direction as v but with norm equal to unity
/// @note If v is the zero vector, the zero vector is returned
template <kinematic::vector V>
constexpr auto normalized(const V& v) -> V
{
    if (V{} == v) {
        return v;
    }
    return v / norm(v);
}

}  // namespace turtle
