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

inline constexpr std::size_t MAX_NORMALIZED_ULP_DIFF{4};

template <class T = DefaultScalar>
class quaternion {
    static constexpr auto dimension = 4;
    using data_type = std::array<T, dimension>;

    data_type data_{};

  public:
    using scalar = T;

    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;

    constexpr quaternion() = default;

    constexpr quaternion(T q0, T q1, T q2, T q3)
        : data_{std::move(q0), std::move(q1), std::move(q2), std::move(q3)}
    {}

    template <kinematic::vector Vector>
    quaternion(T angle, Vector axis)
        : quaternion{std::cos(angle / T{2}),
                     axis.x() * std::sin(angle / T{2}),
                     axis.y() * std::sin(angle / T{2}),
                     axis.z() * std::sin(angle / T{2})}
    {
        if (angle != T{}) {
            assert(MAX_NORMALIZED_ULP_DIFF >=
                   util::ulp_diff(T{1}, dot_product(axis, axis)));
        }
    }

    template <kinematic::vector Vector>
    explicit constexpr quaternion(Vector v)
        : quaternion{T{}, std::move(v.x()), std::move(v.y()), std::move(v.z())}
    {}

    constexpr auto conjugate() const -> quaternion
    {
        return {w(), -x(), -y(), -z()};
    };

    constexpr auto squared_magnitude() const -> T
    {
        return std::inner_product(cbegin(), cend(), cbegin(), T{});
    }

    friend constexpr auto operator*(const quaternion& q, const quaternion& p)
        -> quaternion
    {
        // https://en.wikipedia.org/wiki/Quaternion#Hamilton_product
        return {q.w() * p.w() - q.x() * p.x() - q.y() * p.y() - q.z() * p.z(),
                q.w() * p.x() + q.x() * p.w() + q.y() * p.z() - q.z() * p.y(),
                q.w() * p.y() + q.y() * p.w() - q.x() * p.z() + q.z() * p.x(),
                q.w() * p.z() + q.z() * p.w() + q.x() * p.y() - q.y() * p.x()};
    }

    friend constexpr auto operator==(const quaternion&, const quaternion&)
        -> bool = default;

    constexpr auto w() & -> scalar& { return std::get<0>(data_); }
    constexpr auto w() && -> scalar&& { return std::get<0>(data_); }
    constexpr auto w() const& -> const scalar& { return std::get<0>(data_); }

    constexpr auto x() & -> scalar& { return std::get<1>(data_); }
    constexpr auto x() && -> scalar&& { return std::get<1>(data_); }
    constexpr auto x() const& -> const scalar& { return std::get<1>(data_); }

    constexpr auto y() & -> scalar& { return std::get<2>(data_); }
    constexpr auto y() && -> scalar&& { return std::get<2>(data_); }
    constexpr auto y() const& -> const scalar& { return std::get<2>(data_); }

    constexpr auto z() & -> scalar& { return std::get<3>(data_); }
    constexpr auto z() && -> scalar&& { return std::get<3>(data_); }
    constexpr auto z() const& -> const scalar& { return std::get<3>(data_); }

    constexpr auto begin() & -> iterator { return data_.begin(); }
    constexpr auto begin() const& -> const_iterator { return data_.begin(); }
    constexpr auto cbegin() const& -> const_iterator { return data_.cbegin(); }

    constexpr auto end() & -> iterator { return data_.end(); }
    constexpr auto end() const& -> const_iterator { return data_.end(); }
    constexpr auto cend() const& -> const_iterator { return data_.cend(); }
};

template <kinematic::vector Vector>
quaternion(typename Vector::scalar a, const Vector& v)
    -> quaternion<typename Vector::scalar>;

template <kinematic::vector Vector>
quaternion(const Vector& v) -> quaternion<typename Vector::scalar>;

template <kinematic::frame Frame>
constexpr auto
rotate(const vector<Frame>& v, const quaternion<typename Frame::scalar>& qr)
    -> vector<Frame>
{
    using T = typename Frame::scalar;
    assert(MAX_NORMALIZED_ULP_DIFF >=
           util::ulp_diff(T{1}, qr.squared_magnitude()));

    // https://en.wikipedia.org/wiki/Rotation_(mathematics)#Quaternions
    auto const qo = qr * quaternion{v} * qr.conjugate();

    assert(1e-10 > (qo.w() * qo.w() / qo.squared_magnitude()));

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
