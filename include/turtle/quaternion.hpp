#pragma once

#include "util/ulp_diff.hpp"
#include "vector.hpp"
#include "vector_ops.hpp"

#include "fmt/format.h"

#include <array>
#include <cassert>
#include <cmath>
#include <numeric>
#include <utility>

namespace turtle {

/// @brief Hardcoded library default floating point tolerance limit
// TODO move to a better location and don't hardcode
inline constexpr std::size_t MAX_NORMALIZED_ULP_DIFF{4};

/// @brief Limited quaternion type
/// @tparam T Scalar type
///
/// Limited and incomplete four parameter quaternion type. This library uses
/// unit quaternions to represent the 3D rotation group and does not aim to
/// implement a complete quaternion vector space and its algebraic properties.
///
/// This type is used to represent relative orientations of reference frames and
/// unlike `vector`, specializations of this class template are not bound to a
/// specific frame.
template <class T = DefaultScalar>
class quaternion {
    static constexpr auto dimension = 4;
    using data_type = std::array<T, dimension>;

    data_type data_{};

  public:
    using scalar = T;  ///< Quaternion scalar type

    /// @name Container iterator types
    /// @{
    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;
    /// @}

    /// @brief Constructs a rotation identity quaternion
    constexpr quaternion() = default;

    /// @brief Constructs a quaternion from components
    /// @param q0 Scalar component
    /// @param q1 x-axis component
    /// @param q2 y-axis component
    /// @param q3 z-axis component
    ///
    /// Constructs a quaternion q = q0 + q1*i + q2*j + q3*k where i, j, k are
    /// unit-vectors pointing along three-spatial axes
    ///
    /// @note This constructor does not restrict the created object to be a unit
    /// quaternion.
    constexpr quaternion(T q0, T q1, T q2, T q3)
        : data_{std::move(q0), std::move(q1), std::move(q2), std::move(q3)}
    {}

    /// @brief Constructs a unit quaternion from an angle and axis
    /// @tparam V Kinematic vector type
    /// @param angle Rotation angle
    /// @param axis Rotation axis
    ///
    /// @note This constructor ignores the frame associated with `V`
    /// @pre `axis` is normalized
    template <kinematic::vector V>
    quaternion(T angle, V axis)
        : quaternion{std::cos(angle / T{2}),
                     std::move(axis.x()) * std::sin(angle / T{2}),
                     std::move(axis.y()) * std::sin(angle / T{2}),
                     std::move(axis.z()) * std::sin(angle / T{2})}
    {
        if (angle != T{}) {
            // TODO Define normalization-bypass ctor
            // TODO Define tolerance customization point
            assert(MAX_NORMALIZED_ULP_DIFF >=
                   util::ulp_diff(T{1}, dot_product(axis, axis)));
        }
    }

    /// @brief Constructs a unit quaternion with rotation angle π
    /// @tparam V Kinematic vector type
    /// @param axis Rotation axis
    ///
    /// @note This constructor ignores the frame associated with `V`
    /// @pre `axis` is normalized
    template <kinematic::vector V>
    explicit constexpr quaternion(V axis)
        : quaternion{T{},
                     std::move(axis.x()),
                     std::move(axis.y()),
                     std::move(axis.z())}
    {}

    /// @brief Calculates the quaternion conjugate
    ///
    /// For unit quaternions, defines an inverse rotation.
    constexpr auto conjugate() const -> quaternion
    {
        return {w(), -x(), -y(), -z()};
    };

    /// @brief Calculates the squared norm
    /// @note This may be used to determine if a quaternion has norm 1 more
    /// efficiently than calculating the actual norm.
    constexpr auto squared_norm() const -> T
    {
        return std::inner_product(cbegin(), cend(), cbegin(), T{});
    }

    /// @brief Calculate the Hamilton product of two quaternions
    /// @param q, p Quaternion values
    /// @note The product of two rotation quaternions is equivalent to a
    /// composed rotation.
    /// @see https://en.wikipedia.org/wiki/Quaternion#Hamilton_product
    friend constexpr auto operator*(const quaternion& q, const quaternion& p)
        -> quaternion
    {
        return {q.w() * p.w() - q.x() * p.x() - q.y() * p.y() - q.z() * p.z(),
                q.w() * p.x() + q.x() * p.w() + q.y() * p.z() - q.z() * p.y(),
                q.w() * p.y() + q.y() * p.w() - q.x() * p.z() + q.z() * p.x(),
                q.w() * p.z() + q.z() * p.w() + q.x() * p.y() - q.y() * p.x()};
    }

    /// @brief Compare two quaternions for element-wise equality
    friend constexpr auto operator==(const quaternion&, const quaternion&)
        -> bool = default;

    /// @name Quaternion component methods
    /// @{

    /// @brief Returns a reference to the scalar component
    constexpr auto w() & -> scalar& { return std::get<0>(data_); }
    /// @copydoc w()
    constexpr auto w() && -> scalar&& { return std::get<0>(data_); }
    /// @copydoc w()
    constexpr auto w() const& -> const scalar& { return std::get<0>(data_); }

    /// @brief Returns a reference to the x component
    constexpr auto x() & -> scalar& { return std::get<1>(data_); }
    /// @copydoc x()
    constexpr auto x() && -> scalar&& { return std::get<1>(data_); }
    /// @copydoc x()
    constexpr auto x() const& -> const scalar& { return std::get<1>(data_); }

    /// @brief Returns a reference to the y component
    constexpr auto y() & -> scalar& { return std::get<2>(data_); }
    /// @copydoc y()
    constexpr auto y() && -> scalar&& { return std::get<2>(data_); }
    /// @copydoc y()
    constexpr auto y() const& -> const scalar& { return std::get<2>(data_); }

    /// @brief Returns a reference to the z component
    constexpr auto z() & -> scalar& { return std::get<3>(data_); }
    /// @copydoc z()
    constexpr auto z() && -> scalar&& { return std::get<3>(data_); }
    /// @copydoc z()
    constexpr auto z() const& -> const scalar& { return std::get<3>(data_); }

    /// @}

    /// @name Iterator methods
    /// @{

    /// @brief Returns an iterator to the beginning of the underlying data
    constexpr auto begin() & -> iterator { return data_.begin(); }
    /// @copydoc begin
    constexpr auto begin() const& -> const_iterator { return data_.begin(); }
    /// @copydoc begin
    constexpr auto cbegin() const& -> const_iterator { return data_.cbegin(); }

    /// @brief Returns an iterator to the end of the underlying data
    constexpr auto end() & -> iterator { return data_.end(); }
    /// @copydoc end
    constexpr auto end() const& -> const_iterator { return data_.end(); }
    /// @copydoc end
    constexpr auto cend() const& -> const_iterator { return data_.cend(); }

    /// @}
};

/// @name Deduction guides
/// @{

template <kinematic::vector V>
quaternion(typename V::scalar a, const V& v) -> quaternion<typename V::scalar>;

template <kinematic::vector V>
quaternion(const V& v) -> quaternion<typename V::scalar>;

/// @}

/// @brief Applies a rotation to a vector
/// @tparam F Reference frame type
/// @param v Kinematic vector expressed in frame F
/// @param qr Rotation quaternion
/// @return Rotated vector in frame F
/// @see https://en.wikipedia.org/wiki/Rotation_(mathematics)#Quaternions
template <kinematic::frame F>
constexpr auto
rotate(const vector<F>& v, const quaternion<typename F::scalar>& qr)
    -> vector<F>
{
    // TODO: use a better check for zero vector
    if (v == vector<F>{}) {
        return {};
    }

    using T = typename F::scalar;
    assert(MAX_NORMALIZED_ULP_DIFF >= util::ulp_diff(T{1}, qr.squared_norm()));

    auto const qo = qr * quaternion{v} * qr.conjugate();

    assert(1e-10 > (qo.w() * qo.w() / qo.squared_norm()));
    return {qo.x(), qo.y(), qo.z()};
}

}  // namespace turtle

template <class T>
struct fmt::formatter<turtle::quaternion<T>> : fmt::formatter<T> {
    template <class FormatContext>
    auto format(const turtle::quaternion<T>& q, FormatContext& ctx)
    {
        auto&& out = ctx.out();

        format_to(out, "(");
        formatter<T>::format(q.w(), ctx);
        format_to(out, ", ");
        formatter<T>::format(q.x(), ctx);
        format_to(out, ", ");
        formatter<T>::format(q.y(), ctx);
        format_to(out, ", ");
        formatter<T>::format(q.z(), ctx);
        format_to(out, ")");

        return out;
    }
};
