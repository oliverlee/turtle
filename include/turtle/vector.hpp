#pragma once

#include "fwd.hpp"
#include "util/array.hpp"
#include "util/zip_transform_iterator.hpp"

#include "fmt/format.h"
#include "fmt/ranges.h"

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <ranges>
#include <utility>

namespace turtle {

/// @brief Reference frame bound 3D vector
/// @tparam F Kinematic reference frame
///
/// A 3D vector belonging to a reference frame. A vector value is always tied to
/// the associated frame F but may be expressed in another frame via a world
/// instance.
template <kinematic::frame F, class T = typename F::scalar>
class vector {
    static_assert(std::is_same_v<typename F::scalar, T>);

  public:
    /// @name Kinematic types
    /// @{

    /// @brief Vector associated frame
    using frame = F;

    /// @}

    using scalar = typename F::scalar;  ///< Vector scalar type

  private:
    static constexpr auto dimension = 3;
    using data_type = std::array<scalar, dimension>;

    template <class Iterable>
    static constexpr auto make_vector(Iterable it) -> vector
    {
        return util::make_arraylike<vector, dimension>(it);
    }

    data_type data_{};

  public:
    /// @name Container iterator types
    /// @{
    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;
    /// @}

    /// @brief Constructs a zero vector in frame F
    constexpr vector() = default;

    /// @brief Constructs a vector from components
    /// @param x x-axis component
    /// @param y y-axis component
    /// @param z z-axis component
    constexpr vector(scalar x, scalar y, scalar z)
        : data_{std::move(x), std::move(y), std::move(z)}
    {}

    /// @name Vector component methods
    /// @{

    /// @brief Returns a reference to the x component
    constexpr auto x() & -> scalar& { return std::get<0>(data_); }
    /// @copydoc x()
    constexpr auto x() && -> scalar&& { return std::get<0>(data_); }
    /// @copydoc x()
    [[nodiscard]] constexpr auto x() const& -> const scalar&
    {
        return std::get<0>(data_);
    }

    /// @brief Returns a reference to the y component
    constexpr auto y() & -> scalar& { return std::get<1>(data_); }
    /// @copydoc y()
    constexpr auto y() && -> scalar&& { return std::get<1>(data_); }
    /// @copydoc y()
    [[nodiscard]] constexpr auto y() const& -> const scalar&
    {
        return std::get<1>(data_);
    }

    /// @brief Returns a reference to the z component
    constexpr auto z() & -> scalar& { return std::get<2>(data_); }
    /// @copydoc z()
    constexpr auto z() && -> scalar&& { return std::get<2>(data_); }
    /// @copydoc z()
    [[nodiscard]] constexpr auto z() const& -> const scalar&
    {
        return std::get<2>(data_);
    }

    /// @}

    /// @name Iterator methods
    /// @{

    /// @brief Returns an iterator to the beginning of the underlying data
    constexpr auto begin() & -> iterator { return data_.begin(); }
    /// @copydoc begin
    [[nodiscard]] constexpr auto begin() const& -> const_iterator
    {
        return data_.begin();
    }
    /// @copydoc begin
    [[nodiscard]] constexpr auto cbegin() const& -> const_iterator
    {
        return data_.cbegin();
    }

    /// @brief Returns an iterator to the end of the underlying data
    constexpr auto end() & -> iterator { return data_.end(); }
    /// @copydoc end
    [[nodiscard]] constexpr auto end() const& -> const_iterator
    {
        return data_.end();
    }
    /// @copydoc end
    [[nodiscard]] constexpr auto cend() const& -> const_iterator
    {
        return data_.cend();
    }

    /// @}

    /// @name Frame expression operations
    /// @{

    /// @brief Express this vector in another frame
    /// @tparam To Destination frame
    /// @param ori Orientation of `To` relative the frame associated with this
    /// vector
    /// @return This vector expressed in frame `To`
    template <kinematic::frame To>
    [[nodiscard]] auto in(const orientation<F, To>& ori) const ->
        typename To::vector
    {
        return ori.rotate(*this);
    }

    /// @brief Express this vector in another frame
    /// @tparam To Destination frame
    /// @tparam World Kinematic world
    /// @param world World instance relating the frame associated with this
    /// vector and `To`
    /// @return This vector expressed in frame `To`
    template <kinematic::frame To, kinematic::world World>
    [[nodiscard]] auto in(const World& world) const -> typename To::vector
    {
        return world.template express<frame, To>().rotate(*this);
    }

    /// @}

    /// @name Elementwise transform methods
    /// @{

    template <class UnaryOp>
    static constexpr auto apply_elementwise(const vector& v, UnaryOp uop)
        -> vector
    {
        return make_vector(v | std::views::transform(std::move(uop)));
    }
    template <class BinOp>
    static constexpr auto
    apply_elementwise(const vector& v, const vector& u, BinOp bop) -> vector
    {
        return make_vector(util::zip_transform_iterator{
            v.cbegin(), u.cbegin(), std::move(bop)});
    }

    /// @}

    /// @name Compound assignment methods
    /// @{

    /// @brief Compound vector addition and assignment
    constexpr auto operator+=(const vector& u) -> vector&
    {
        std::transform(cbegin(), cend(), u.cbegin(), begin(), std::plus<>{});
        return *this;
    }
    /// @brief Compound vector subtraction and assignment
    constexpr auto operator-=(const vector& u) -> vector&
    {
        std::transform(cbegin(), cend(), u.cbegin(), begin(), std::minus<>{});
        return *this;
    }
    /// @brief Compound scalar multiplication and assignment
    constexpr auto operator*=(scalar a) -> vector&
    {
        std::for_each(begin(), end(), [a = std::move(a)](auto& x) { x *= a; });
        return *this;
    }
    /// @brief Compound scalar division and assignment
    constexpr auto operator/=(scalar a) -> vector&
    {
        std::for_each(begin(), end(), [a = std::move(a)](auto& x) { x /= a; });
        return *this;
    }

    /// @}

    /// @name Vector space operations
    /// @{

    /// @brief Vector negation
    friend constexpr auto operator-(const vector& v) -> vector
    {
        return apply_elementwise(v, std::negate<>{});
    }

    /// @brief Vector addition
    friend constexpr auto operator+(const vector& v, const vector& u) -> vector
    {
        return apply_elementwise(v, u, std::plus<>{});
    }
    /// @brief Vector subtraction
    friend constexpr auto operator-(const vector& v, const vector& u) -> vector
    {
        return apply_elementwise(v, u, std::minus<>{});
    }

    /// @brief Scalar multiplication
    friend constexpr auto operator*(scalar a, const vector& v) -> vector
    {
        return apply_elementwise(v, [a = std::move(a)](const auto& x) {
            return a * x;
        });
    }
    /// @brief Scalar multiplication
    friend constexpr auto operator*(const vector& v, scalar a) -> vector
    {
        return apply_elementwise(v, [a = std::move(a)](const auto& x) {
            return x * a;
        });
    }
    /// @brief Scalar division
    friend constexpr auto operator/(const vector& v, scalar a) -> vector
    {
        return apply_elementwise(v, [a = std::move(a)](const auto& x) {
            return x / a;
        });
    }

    /// @}

    /// @brief Compare two vectors for element-wise equality
    friend constexpr auto operator==(const vector&, const vector&)
        -> bool = default;
};

}  // namespace turtle

namespace fmt {

template <class F, class Char>
struct is_range<turtle::vector<F>, Char> : std::false_type {};

template <class F>
struct formatter<turtle::vector<F>> : formatter<typename F::scalar> {
    template <class FormatContext>
    auto format(const turtle::vector<F>& v, FormatContext& ctx)
    {
        using T = typename F::scalar;

        auto&& out = ctx.out();

        format_to(out, "[{}] (", F::name);
        formatter<T>::format(v.x(), ctx);
        format_to(out, ", ");
        formatter<T>::format(v.y(), ctx);
        format_to(out, ", ");
        formatter<T>::format(v.z(), ctx);
        format_to(out, ")");

        return out;
    }
};

}  // namespace fmt
