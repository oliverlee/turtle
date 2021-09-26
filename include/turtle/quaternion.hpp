#pragma once

#include "vector.hpp"
#include "vector_ops.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <numeric>
#include <utility>

namespace turtle {

template <class T = double>
class quaternion {
    static constexpr auto dimension = 4;
    using data_type = std::array<T, dimension>;

    data_type data_{};

  public:
    using scalar_type = T;

    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;

    constexpr quaternion() = default;

    constexpr quaternion(T q0, T q1, T q2, T q3)
        : data_{std::move(q0), std::move(q1), std::move(q2), std::move(q3)}
    {}

    template <con::reference_frame_vector Vector>
    quaternion(T angle, Vector axis)
        : quaternion{std::cos(angle / T{2}),
                     axis.x() * std::sin(angle / T{2}),
                     axis.y() * std::sin(angle / T{2}),
                     axis.z() * std::sin(angle / T{2})}
    {
        assert(T{1} == dot_product(axis, axis));
    }

    template <con::reference_frame_vector Vector>
    explicit constexpr quaternion(Vector v)
        : quaternion{T{}, std::move(v.x()), std::move(v.y()), std::move(v.z())}
    {}

    constexpr auto conjugate() const -> quaternion { return {w(), -x(), -y(), -z()}; };

    friend constexpr auto operator*(const quaternion& q, const quaternion& p) -> quaternion
    {
        // https://en.wikipedia.org/wiki/Quaternion#Hamilton_product
        return {q.w() * p.w() - q.x() * p.x() - q.y() * p.y() - q.z() * p.z(),
                q.w() * p.x() + q.x() * p.w() + q.y() * p.z() - q.z() * p.y(),
                q.w() * p.y() + q.y() * p.w() - q.x() * p.z() + q.z() * p.x(),
                q.w() * p.z() + q.z() * p.w() + q.x() * p.y() - q.y() * p.x()};
    }

    constexpr auto w() & -> scalar_type& { return std::get<0>(data_); }
    constexpr auto w() && -> scalar_type&& { return std::get<0>(data_); }
    constexpr auto w() const& -> const scalar_type& { return std::get<0>(data_); }

    constexpr auto x() & -> scalar_type& { return std::get<1>(data_); }
    constexpr auto x() && -> scalar_type&& { return std::get<1>(data_); }
    constexpr auto x() const& -> const scalar_type& { return std::get<1>(data_); }

    constexpr auto y() & -> scalar_type& { return std::get<2>(data_); }
    constexpr auto y() && -> scalar_type&& { return std::get<2>(data_); }
    constexpr auto y() const& -> const scalar_type& { return std::get<2>(data_); }

    constexpr auto z() & -> scalar_type& { return std::get<3>(data_); }
    constexpr auto z() && -> scalar_type&& { return std::get<3>(data_); }
    constexpr auto z() const& -> const scalar_type& { return std::get<3>(data_); }

    constexpr auto begin() & -> iterator { return data_.begin(); }
    constexpr auto begin() const& -> const_iterator { return data_.begin(); }
    constexpr auto cbegin() const& -> const_iterator { return data_.cbegin(); }

    constexpr auto end() & -> iterator { return data_.end(); }
    constexpr auto end() const& -> const_iterator { return data_.end(); }
    constexpr auto cend() const& -> const_iterator { return data_.cend(); }
};

template <con::reference_frame_vector Vector>
quaternion(typename Vector::scalar_type a, const Vector& v)
    -> quaternion<typename Vector::scalar_type>;

template <con::reference_frame_vector Vector>
quaternion(const Vector& v) -> quaternion<typename Vector::scalar_type>;

template <con::reference_frame Frame>
constexpr auto rotate(const vector<Frame>& v, const quaternion<typename Frame::scalar_type>& qr)
    -> vector<Frame>
{
    using T = typename Frame::scalar_type;
    assert(T{1} == std::inner_product(qr.cbegin(), qr.cend(), qr.cbegin(), T{}));

    // https://en.wikipedia.org/wiki/Rotation_(mathematics)#Quaternions
    auto const qo = qr * quaternion{v} * qr.conjugate();

    assert(T{} == qo.w());
    return {qo.x(), qo.y(), qo.z()};
}

}  // namespace turtle
