#pragma once

#include "fwd.hpp"
#include "util/array.hpp"
#include "util/zip_transform_iterator.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <ranges>
#include <utility>

namespace turtle {

/// @brief CRTP interface for defining vector types
/// @tparam T Scalar type
/// @tparam D Derived vector type
///
/// A generic 3 element vector abstraction, used for defining frame-bound vector
/// types.
template <class T, class D>
class vector_interface {
  public:
    using scalar = T;  ///< Vector scalar type

    /// @brief Constructs a zero vector
    constexpr vector_interface() = default;

    /// @brief Constructs a vector from components
    /// @param x x-axis component
    /// @param y y-axis component
    /// @param z z-axis component
    constexpr vector_interface(scalar x, scalar y, scalar z)
        : data_{std::move(x), std::move(y), std::move(z)}
    {}

  private:
    friend D;

    vector_interface(const vector_interface&) = default;
    vector_interface(vector_interface&&) noexcept = default;
    auto operator=(const vector_interface&) -> vector_interface& = default;
    auto operator=(vector_interface&&) noexcept -> vector_interface& = default;
    ~vector_interface() = default;

    static constexpr auto dimension = 3;
    using data_type = std::array<T, dimension>;

    template <class Iterable>
    static constexpr auto make_derived(Iterable it) -> D
    {
        return util::make_arraylike<D, dimension>(it);
    }

    data_type data_{};

    constexpr auto derived() & -> D& { return static_cast<D&>(*this); }

  public:
    /// @name Container iterator types
    /// @{
    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;
    /// @}

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

    /// @name Elementwise transform methods
    /// @{

    template <class UnaryOp>
    static constexpr auto
    apply_elementwise(const vector_interface& v, UnaryOp uop) -> D
    {
        return make_derived(v | std::views::transform(std::move(uop)));
    }
    template <class BinOp>
    static constexpr auto apply_elementwise(
        const vector_interface& v, const vector_interface& u, BinOp bop) -> D
    {
        return make_derived(util::zip_transform_iterator{
            v.cbegin(), u.cbegin(), std::move(bop)});
    }

    /// @}

    /// @name Compound assignment methods
    /// @{

    /// @brief Compound vector addition and assignment
    constexpr auto operator+=(const vector_interface& u) -> D&
    {
        std::transform(cbegin(), cend(), u.cbegin(), begin(), std::plus<>{});
        return derived();
    }
    /// @brief Compound vector subtraction and assignment
    constexpr auto operator-=(const vector_interface& u) -> D&
    {
        std::transform(cbegin(), cend(), u.cbegin(), begin(), std::minus<>{});
        return derived();
    }
    /// @brief Compound scalar multiplication and assignment
    constexpr auto operator*=(scalar a) -> D&
    {
        std::for_each(begin(), end(), [a = std::move(a)](auto& x) { x *= a; });
        return derived();
    }
    /// @brief Compound scalar division and assignment
    constexpr auto operator/=(scalar a) -> D&
    {
        std::for_each(begin(), end(), [a = std::move(a)](auto& x) { x /= a; });
        return derived();
    }

    /// @}

    /// @name Vector space operations
    /// @{

    /// @brief Vector negation
    friend constexpr auto operator-(const vector_interface& v) -> D
    {
        return apply_elementwise(v, std::negate<>{});
    }

    /// @brief Vector addition
    friend constexpr auto
    operator+(const vector_interface& v, const vector_interface& u) -> D
    {
        return apply_elementwise(v, u, std::plus<>{});
    }
    /// @brief Vector subtraction
    friend constexpr auto
    operator-(const vector_interface& v, const vector_interface& u) -> D
    {
        return apply_elementwise(v, u, std::minus<>{});
    }

    /// @brief Scalar multiplication
    friend constexpr auto operator*(scalar a, const vector_interface& v) -> D
    {
        return apply_elementwise(v, [a = std::move(a)](const auto& x) {
            return a * x;
        });
    }
    /// @brief Scalar multiplication
    friend constexpr auto operator*(const vector_interface& v, scalar a) -> D
    {
        return apply_elementwise(v, [a = std::move(a)](const auto& x) {
            return x * a;
        });
    }
    /// @brief Scalar division
    friend constexpr auto operator/(const vector_interface& v, scalar a) -> D
    {
        return apply_elementwise(v, [a = std::move(a)](const auto& x) {
            return x / a;
        });
    }

    /// @}

    /// @brief Compare two vectors for element-wise equality
    friend constexpr auto
    operator==(const vector_interface&, const vector_interface&)
        -> bool = default;
};

}  // namespace turtle
