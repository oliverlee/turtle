#pragma once

#include "util/array.hpp"
#include "util/zip_transform_iterator.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <fmt/format.h>
#include <ranges>
#include <utility>

namespace turtle {

template <class T>
class vector {
    static constexpr auto dimension = 3;
    using data_type = std::array<T, dimension>;

    template <class Iterable>
    static constexpr auto make_vector(Iterable it) -> vector
    {
        return util::make_arraylike<vector, dimension>(it);
    }

    data_type data_{};

  public:
    using value_type = T;

    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;

    constexpr vector() = default;

    template <class U>
    constexpr vector(U x, U y, U z) requires std::same_as<U, T>
        : data_{std::move(x), std::move(y), std::move(z)}
    {}

    constexpr auto x() & -> T& { return std::get<0>(data_); }
    constexpr auto x() && -> T&& { return std::get<0>(data_); }
    constexpr auto x() const& -> const T& { return std::get<0>(data_); }

    constexpr auto y() & -> T& { return std::get<1>(data_); }
    constexpr auto y() && -> T&& { return std::get<1>(data_); }
    constexpr auto y() const& -> const T& { return std::get<1>(data_); }

    constexpr auto z() & -> T& { return std::get<2>(data_); }
    constexpr auto z() && -> T&& { return std::get<2>(data_); }
    constexpr auto z() const& -> const T& { return std::get<2>(data_); }

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
    constexpr auto operator*=(T a) -> vector&
    {
        std::ranges::for_each(*this, [a = std::move(a)](auto& x) { x *= a; });
        return *this;
    }
    constexpr auto operator/=(T a) -> vector&
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

    friend constexpr auto operator*(T a, const vector& v) -> vector
    {
        return apply_elementwise(v, [a = std::move(a)](const auto& x) { return a * x; });
    }
    friend constexpr auto operator*(const vector& v, T a) -> vector
    {
        return apply_elementwise(v, [a = std::move(a)](const auto& x) { return x * a; });
    }
    friend constexpr auto operator/(const vector& v, T a) -> vector
    {
        return apply_elementwise(v, [a = std::move(a)](const auto& x) { return x / a; });
    }

    friend constexpr auto operator==(const vector&, const vector&) -> bool = default;
};

template <class T>
vector(T x, T y, T z) -> vector<T>;

}  // namespace turtle

template <class T>
struct fmt::formatter<turtle::vector<T>> : fmt::formatter<T> {
    template <class FormatContext>
    auto format(const turtle::vector<T>& v, FormatContext& ctx)
    {
        auto&& out = ctx.out();

        format_to(out, "(");
        formatter<T>::format(v.x(), ctx);
        format_to(out, ", ");
        formatter<T>::format(v.y(), ctx);
        format_to(out, ", ");
        formatter<T>::format(v.z(), ctx);
        format_to(out, ")");

        return out;
    }
};
