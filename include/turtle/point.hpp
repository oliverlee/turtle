#pragma once

#include "fwd.hpp"
#include "position.hpp"
#include "world.hpp"

#include "fmt/format.h"

#include <type_traits>
#include <variant>

namespace turtle {

/// @brief A point bound to a kinematic world
/// @tparam World World type this `point` is bound to
/// @note Unlike `vector`, a `point` is bound to a single world.
template <kinematic::world World>
class point {
    using position_variant =
        metal::apply<metal::lambda<std::variant>,
                     metal::transform<metal::lambda<turtle::position>,
                                      meta::flatten<typename World::tree>>>;

    template <kinematic::frame F>
    static constexpr auto in_world_v = World::tree::template contains_v<F>;

  public:
    /// @name Kinematic types
    /// @{

    /// @brief Associated kinematic world
    using world = World;

    /// @}

    /// @brief Constructs a point at the world origin
    /// @note Fixed with respect to `world::root`
    constexpr point() = default;

    /// @brief Constructs a point with a displacement from the world origin
    /// @tparam F Reference frame
    /// @param r Displacement from the origin expressed in `F`
    ///
    /// Sets the position of this point in the world, expressed in `F`.
    /// This position is fixed in `F` which may not be the same as the
    /// `world::root`.
    template <kinematic::frame F>
    requires in_world_v<F>
    explicit constexpr point(turtle::position<F> r)
        : displacement_{std::in_place_type<turtle::position<F>>, std::move(r)}
    {}

    /// @brief Sets the point's position
    /// @tparam F Reference frame
    /// @param r Displacement from the origin expressed in `F`
    ///
    /// Sets the position of this point in the world, expressed in `F`.
    /// This position is fixed in `F` which may not be the same as the
    /// `world::root`.
    template <kinematic::frame F>
    requires in_world_v<F>
    constexpr auto position(turtle::position<F> r) -> void
    {
        displacement_ = std::move(r);
    }

    /// @brief Obtains the point's set position
    [[nodiscard]] constexpr auto position() const& -> const position_variant&
    {
        return displacement_;
    }

    /// @brief Obtains the point's position, expressed in the specified frame
    /// @tparam F Observation frame
    /// @param w A world instance
    template <kinematic::frame F>
    requires in_world_v<F>
    [[nodiscard]] constexpr auto position(const world& w) const
        -> turtle::position<F>
    {
        return std::visit(
            [&w](const auto& r) { return r.template in<F>(w); }, position());
    }

  private:
    /// Displacement from world origin
    position_variant displacement_{};
};

}  // namespace turtle

template <class World>
struct fmt::formatter<turtle::point<World>>
    : fmt::formatter<typename World::scalar> {
    template <class FormatContext>
    auto format(const turtle::point<World>& p, FormatContext& ctx)
    {
        // TODO: short form printing for world
        return std::visit(
            [out = ctx.out()](const auto& r) {
                return format_to(out, "point at {}", r);
            },
            p.position());
    }
};
