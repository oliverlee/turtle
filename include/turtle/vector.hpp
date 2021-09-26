#pragma once

#include "fwd.hpp"
#include "util/array.hpp"
#include "util/zip_transform_iterator.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <fmt/format.h>
#include <ranges>
#include <utility>

namespace turtle {

template <con::reference_frame Frame>
class vector {
  public:
    using reference_frame = Frame;
    using scalar_type = typename Frame::scalar_type;

  private:
    static constexpr auto dimension = 3;
    using data_type = std::array<scalar_type, dimension>;

    template <class Iterable>
    static constexpr auto make_vector(Iterable it) -> vector
    {
        return util::make_arraylike<vector, dimension>(it);
    }

    data_type data_{};

  public:
    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;

    constexpr vector() = default;

    template <class U>
    constexpr vector(U x, U y, U z) requires std::same_as<U, scalar_type>
        : data_{std::move(x), std::move(y), std::move(z)}
    {}

    constexpr auto x() & -> scalar_type& { return std::get<0>(data_); }
    constexpr auto x() && -> scalar_type&& { return std::get<0>(data_); }
    constexpr auto x() const& -> const scalar_type& { return std::get<0>(data_); }

    constexpr auto y() & -> scalar_type& { return std::get<1>(data_); }
    constexpr auto y() && -> scalar_type&& { return std::get<1>(data_); }
    constexpr auto y() const& -> const scalar_type& { return std::get<1>(data_); }

    constexpr auto z() & -> scalar_type& { return std::get<2>(data_); }
    constexpr auto z() && -> scalar_type&& { return std::get<2>(data_); }
    constexpr auto z() const& -> const scalar_type& { return std::get<2>(data_); }

    constexpr auto begin() & -> iterator { return data_.begin(); }
    constexpr auto begin() const& -> const_iterator { return data_.begin(); }
    constexpr auto cbegin() const& -> const_iterator { return data_.cbegin(); }

    constexpr auto end() & -> iterator { return data_.end(); }
    constexpr auto end() const& -> const_iterator { return data_.end(); }
    constexpr auto cend() const& -> const_iterator { return data_.cend(); }

    template <class UnaryOp>
    static constexpr auto apply_elementwise(const vector& v, UnaryOp uop) -> vector
    {
        return make_vector(v | std::views::transform(std::move(uop)));
    }
    template <class BinOp>
    static constexpr auto apply_elementwise(const vector& v, const vector& u, BinOp bop) -> vector
    {
        return make_vector(util::zip_transform_iterator{v.cbegin(), u.cbegin(), std::move(bop)});
    }

    constexpr auto operator+=(const vector& u) -> vector&
    {
        std::ranges::transform(*this, u, begin(), std::plus<>{});
        return *this;
    }
    constexpr auto operator-=(const vector& u) -> vector&
    {
        std::ranges::transform(*this, u, begin(), std::minus<>{});
        return *this;
    }
    constexpr auto operator*=(scalar_type a) -> vector&
    {
        std::ranges::for_each(*this, [a = std::move(a)](auto& x) { x *= a; });
        return *this;
    }
    constexpr auto operator/=(scalar_type a) -> vector&
    {
        std::ranges::for_each(*this, [a = std::move(a)](auto& x) { x /= a; });
        return *this;
    }

    friend constexpr auto operator+(const vector& v, const vector& u) -> vector
    {
        return apply_elementwise(v, u, std::plus<>{});
    }
    friend constexpr auto operator-(const vector& v, const vector& u) -> vector
    {
        return apply_elementwise(v, u, std::minus<>{});
    }

    friend constexpr auto operator-(const vector& v) -> vector
    {
        return apply_elementwise(v, std::negate<>{});
    }

    friend constexpr auto operator*(scalar_type a, const vector& v) -> vector
    {
        return apply_elementwise(v, [a = std::move(a)](const auto& x) { return a * x; });
    }
    friend constexpr auto operator*(const vector& v, scalar_type a) -> vector
    {
        return apply_elementwise(v, [a = std::move(a)](const auto& x) { return x * a; });
    }
    friend constexpr auto operator/(const vector& v, scalar_type a) -> vector
    {
        return apply_elementwise(v, [a = std::move(a)](const auto& x) { return x / a; });
    }

    friend constexpr auto operator==(const vector&, const vector&) -> bool = default;
};

}  // namespace turtle

template <class Frame>
struct fmt::formatter<turtle::vector<Frame>> : fmt::formatter<typename Frame::scalar_type> {
    template <class FormatContext>
    auto format(const turtle::vector<Frame>& v, FormatContext& ctx)
    {
        using T = typename Frame::scalar_type;

        auto&& out = ctx.out();

        format_to(out, "[{}] (", Frame::name);
        formatter<T>::format(v.x(), ctx);
        format_to(out, ", ");
        formatter<T>::format(v.y(), ctx);
        format_to(out, ", ");
        formatter<T>::format(v.z(), ctx);
        format_to(out, ")");

        return out;
    }
};
