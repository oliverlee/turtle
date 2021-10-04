#pragma once

#include "fwd.hpp"
#include "quaternion.hpp"
#include "vector_ops.hpp"

#include "fmt/format.h"

#include <concepts>
#include <utility>

namespace turtle {

/// @brief An orientation relating two reference frames
/// @tparam From Source reference frame
/// @tparam To Destination reference frame
///
/// Specifies the orientation of frame `To` relative frame `From` with a single
/// angle-axis rotation.
template <kinematic::frame From, kinematic::frame To>
requires std::same_as<typename From::scalar, typename To::scalar>
class orientation {
  public:
    using scalar = typename From::scalar;  ///< Orientation scalar type

    /// @name Kinematic types
    /// @{

    /// @brief Orientation quaternion type
    using quaternion = turtle::quaternion<scalar>;

    /// @brief Source frame
    using source_frame = From;

    /// @brief Destination frame
    using dest_frame = To;

    /// @}

    /// @brief Constructs zero angle-axis orientation between frame `From` and
    /// frame `To`
    constexpr orientation() = default;

    /// @brief Constructs an orientation from a quaternion between frame `From`
    /// and frame `To`
    /// @param rot Quaternion rotation starting at `From` to align with `To`
    /// @pre `rot` is normalized
    explicit constexpr orientation(quaternion rot) : rotation_{std::move(rot)}
    {
        assert(MAX_NORMALIZED_ULP_DIFF >=
               util::ulp_diff(scalar{1}, rotation_.squared_norm()));
    }

    /// @brief Constructs an orientation from an angle and an axis between frame
    /// `From` and frame `To`
    /// @param angle, axis Angle-axis rotation starting at `From` to align with
    /// `To`
    orientation(scalar angle, typename From::vector axis)
        : rotation_{std::move(angle), std::move(axis)}
    {}

    /// @brief Obtains the rotation angle
    /// @note This performs an internal calculation and may be sensitive to
    /// numerical stability issues.
    auto angle() const -> scalar
    {
        // https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Recovering_the_axis-angle_representation
        return scalar{2} * std::atan2(norm(vector_part()), rotation_.w());
    }

    /// @brief Obtains the rotation axis
    /// @note This performs an internal calculation and may be sensitive to
    /// numerical stability issues.
    auto axis() const -> typename From::vector
    {
        return normalized(vector_part());
    }

    /// @brief Obtains the rotation as a quaternion
    constexpr auto rotation() const& noexcept -> const quaternion&
    {
        return rotation_;
    }

    /// @brief Calculates the inverse orientation starting at `To` and ending at
    /// `From`
    constexpr auto inverse() const -> orientation<To, From>
    {
        return orientation<To, From>{rotation_.conjugate()};
    }

    /// @brief Applies the rotation and converts a vector from `From` to `To`
    /// @param v Vector bound to frame `From`
    constexpr auto rotate(const typename From::vector& v) const ->
        typename To::vector
    {
        const auto u = turtle::rotate(v, rotation_.conjugate());
        return {u.x(), u.y(), u.z()};
    }

  private:
    /// @brief Composes two orientations with the same intermediate frame
    /// @tparam C Final destination frame
    /// @return An orientation between `From` and `C`
    template <kinematic::frame C>
    friend constexpr auto
    operator*(const orientation& ori1, const orientation<To, C>& ori2)
        -> orientation<From, C>
    {
        return orientation<From, C>{ori1.rotation() * ori2.rotation()};
    }

    constexpr auto vector_part() const -> typename From::vector
    {
        return {rotation_.x(), rotation_.y(), rotation_.z()};
    }

    quaternion rotation_{scalar{1}, scalar{}, scalar{}, scalar{}};
};

}  // namespace turtle

template <class From, class To>
struct fmt::formatter<turtle::orientation<From, To>>
    : fmt::formatter<typename From::vector> {
    template <class FormatContext>
    auto format(const turtle::orientation<From, To>& ori, FormatContext& ctx)
    {
        using T = typename From::scalar;

        auto&& out = ctx.out();

        format_to(out, "[{}] <- ", To::name);
        formatter<typename From::vector>::format(ori.axis(), ctx);
        format_to(out, ", θ: ");
        formatter<T>::format(ori.angle(), ctx);

        return out;
    }
};
