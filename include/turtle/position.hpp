#pragma once

#include "fwd.hpp"
#include "vector.hpp"
#include "vector_interface.hpp"

#include "fmt/format.h"
#include "fmt/ranges.h"

#include <bit>
#include <utility>

namespace turtle {

/// @brief A position vector
/// @tparam E Expression frame
template <kinematic::frame E>
struct position : vector_interface<typename E::scalar, position<E>> {
    /// @brief Expression frame
    using frame = E;

    using vector_interface<typename E::scalar, position<E>>::vector_interface;

    /// @brief Construct a position vector from a frame vector
    /// @param v Frame vector
    constexpr position(typename E::vector v)
        : vector_interface<typename E::scalar, position<E>>{
              std::move(v.x()), std::move(v.y()), std::move(v.z())}
    {}

    /// @name Frame expression operations
    /// @{

    /// @brief Express this position in another frame
    /// @tparam To Destination frame
    /// @param ori Orientation of `To` relative the frame associated with this
    /// position
    /// @return This position expressed in frame `To`
    template <kinematic::frame To>
    [[nodiscard]] auto in(const orientation<E, To>& ori) const ->
        typename To::position
    {
        return ori.rotate(std::bit_cast<typename E::vector>(*this));
    }

    /// @brief Express this position in another frame
    /// @tparam To Destination frame
    /// @tparam World Kinematic world
    /// @param world World instance relating the frame associated with this
    /// position and `To`
    /// @return This position expressed in frame `To`
    template <kinematic::frame To, kinematic::world World>
    [[nodiscard]] auto in(const World& world) const -> typename To::position
    {
        return world.template express<E, To>().rotate(
            std::bit_cast<typename E::vector>(*this));
    }

    /// @}
};

/// @name Deduction guides
/// @{

template <kinematic::vector V>
position(V) -> position<typename V::frame>;

/// @}

}  // namespace turtle

namespace fmt {

template <class E, class Char>
struct is_range<turtle::position<E>, Char> : std::false_type {};

template <class E>
struct formatter<turtle::position<E>> : formatter<turtle::vector<E>> {
    template <class FormatContext>
    auto format(const turtle::position<E>& r, FormatContext& ctx)
    {
        auto&& out = ctx.out();

        format_to(out, "<p>: ");
        formatter<turtle::vector<E>>::format(
            std::bit_cast<turtle::vector<E>>(r), ctx);

        return out;
    }
};

}  // namespace fmt
