#pragma once

#include "fwd.hpp"
#include "quaternion.hpp"
#include "vector_ops.hpp"

#include "fmt/format.h"

#include <concepts>
#include <utility>

namespace turtle {

template <kinematic::frame From, kinematic::frame To>
requires std::same_as<typename From::scalar, typename To::scalar>
class orientation {
  public:
    using scalar = typename From::scalar;
    using quaternion = turtle::quaternion<scalar>;

    using source_frame = From;
    using dest_frame = To;

    constexpr orientation() = default;

    explicit constexpr orientation(quaternion rot) : rotation_{std::move(rot)}
    {
        assert(MAX_NORMALIZED_ULP_DIFF >=
               util::ulp_diff(scalar{1}, rotation_.squared_magnitude()));
    }

    orientation(scalar angle, typename From::vector axis)
        : rotation_{std::move(angle), std::move(axis)}
    {}

    auto angle() const -> scalar
    {
        // https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Recovering_the_axis-angle_representation
        return scalar{2} * std::atan2(magnitude(vector_part()), rotation_.w());
    }
    auto axis() const -> typename From::vector
    {
        return normalized(vector_part());
    }

    constexpr auto rotation() const& noexcept -> const quaternion&
    {
        return rotation_;
    }

    constexpr auto inverse() const -> orientation<To, From>
    {
        return orientation<To, From>{rotation_.conjugate()};
    }

    constexpr auto rotate(const typename From::vector& v) const ->
        typename To::vector
    {
        const auto u = turtle::rotate(v, rotation_.conjugate());
        return {u.x(), u.y(), u.z()};
    }

  private:
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
