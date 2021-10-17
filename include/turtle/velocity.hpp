#pragma once

#include "fwd.hpp"
#include "vector.hpp"
#include "vector_interface.hpp"

#include "fmt/format.h"
#include "fmt/ranges.h"

#include <bit>
#include <concepts>
#include <utility>

namespace turtle {

/// @brief A velocity vector
/// @tparam B Observation frame
/// @tparam E Expression frame
template <kinematic::frame B, kinematic::frame E = B>
requires std::same_as<typename B::scalar, typename E::scalar>
struct velocity : vector_interface<typename B::scalar, velocity<B, E>> {
    /// Reference frame in which the velocity is observed in
    using observation_frame = B;
    /// Reference frame in which the velocity is expressed in
    using expression_frame = E;

    using vector_interface<typename B::scalar,
                           velocity<B, E>>::vector_interface;

    /// @brief Construct a velocity vector from a frame vector
    /// @param v Frame vector
    constexpr velocity(typename E::vector v) : velocity(B{}, std::move(v)) {}

    /// @brief Construct a velocity vector from a frame vector
    /// @param b Observation frame
    /// @param v Frame vector
    constexpr velocity(B, typename E::vector v)
        : vector_interface<typename B::scalar, velocity<B, E>>{
              std::move(v.x()), std::move(v.y()), std::move(v.z())}
    {}

    /// @name Frame expression operations
    /// @{

    /// @brief Express this velocity in another frame
    /// @tparam E2 Target expression frame
    /// @param ori Orientation of `E2` relative the frame associated with this
    /// velocity
    /// @return This velocity expressed in frame `E2`
    template <kinematic::frame E2>
    [[nodiscard]] auto express_in(const orientation<E, E2>& ori) const
        -> velocity<B, E2>
    {
        return ori.rotate(std::bit_cast<typename E::vector>(*this));
    }

    /// @brief Express this position in another frame
    /// @tparam E2 Target expression frame
    /// @tparam World Kinematic world
    /// @param world World instance relating the frame associated with the
    /// current expression frame and `E2`
    /// @return This velocity expressed in frame `E2`
    template <kinematic::frame E2, kinematic::world World>
    [[nodiscard]] auto express_in(const World& world) const -> velocity<B, E2>
    {
        return world.template express<E, E2>().rotate(
            std::bit_cast<typename E::vector>(*this));
    }

    /// @}
};

/// @name Deduction guides
/// @{

template <kinematic::frame B, kinematic::vector V>
velocity(B, V) -> velocity<B, typename V::frame>;

/// @}

}  // namespace turtle

namespace fmt {

template <class B, class E, class Char>
struct is_range<turtle::velocity<B, E>, Char> : std::false_type {};

template <class B, class E>
struct formatter<turtle::velocity<B, E>> : formatter<turtle::vector<E>> {
    template <class FormatContext>
    auto format(const turtle::velocity<B, E>& v, FormatContext& ctx)
    {
        auto&& out = ctx.out();

        format_to(out, "<v, {}>: ", B{});
        formatter<turtle::vector<E>>::format(
            std::bit_cast<turtle::vector<E>>(v), ctx);

        return out;
    }
};

}  // namespace fmt
