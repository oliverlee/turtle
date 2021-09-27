#pragma once

#include "fwd.hpp"
#include "quaternion.hpp"
#include "vector_ops.hpp"

#include <concepts>
#include <fmt/format.h>
#include <utility>

namespace turtle {

template <con::reference_frame From, con::reference_frame To>
requires std::same_as<typename From::scalar_type, typename To::scalar_type>
class orientation {
  public:
    using scalar_type = typename From::scalar_type;

    using from_type = From;
    using to_type = To;

    constexpr orientation() = default;

    orientation(scalar_type angle, typename From::vector axis)
        : rotation_{std::move(angle), std::move(axis)}
    {}

    auto angle() const -> scalar_type
    {
        // https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Recovering_the_axis-angle_representation
        return scalar_type{2} * std::atan2(magnitude(vector_part()), rotation_.w());
    }
    auto axis() const -> typename From::vector { return normalized(vector_part()); }

    constexpr auto rotate(const typename From::vector& v) const -> typename To::vector
    {
        const auto u = turtle::rotate(v, rotation_.conjugate());
        return {u.x(), u.y(), u.z()};
    }

  private:
    constexpr auto vector_part() const -> typename From::vector
    {
        return {rotation_.x(), rotation_.y(), rotation_.z()};
    }

    quaternion<scalar_type> rotation_{scalar_type{1}, scalar_type{}, scalar_type{}, scalar_type{}};
};

}  // namespace turtle

template <class From, class To>
struct fmt::formatter<turtle::orientation<From, To>> : fmt::formatter<typename From::vector> {
    template <class FormatContext>
    auto format(const turtle::orientation<From, To>& ori, FormatContext& ctx)
    {
        using T = typename From::scalar_type;

        auto&& out = ctx.out();

        format_to(out, "[{}] <- ", To::name);
        formatter<typename From::vector>::format(ori.axis(), ctx);
        format_to(out, ", θ: ");
        formatter<T>::format(ori.angle(), ctx);

        return out;
    }
};
