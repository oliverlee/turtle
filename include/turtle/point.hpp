#pragma once

#include "fwd.hpp"
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
    using vector_variant =
        metal::apply<metal::lambda<std::variant>,
                     metal::transform<metal::lambda<vector>,
                                      meta::flatten<typename World::tree>>>;

    template <kinematic::frame F>
    static constexpr auto in_world_v = World::tree::template contains_v<F>;

  public:
    using world = World;  ///< Associated kinematic world

    /// @brief Constructs a point at the world origin
    /// @note Fixed with respect to `world::root`
    constexpr point() = default;

    /// @brief Constructs a point with a displacement from the world origin
    /// @tparam V A world vector type
    /// @param v Displacement from the origin expressed in `V::frame`
    ///
    /// Sets the position of this point in the world, expressed in `V::frame`.
    /// This position is fixed in `V::frame` which may not be the same as the
    /// `world::root`.
    template <kinematic::vector V>
    requires in_world_v<typename V::frame>
    explicit constexpr point(V v)
        : displacement_{std::in_place_type<V>, std::move(v)}
    {}

    /// @brief Sets the point's position
    /// @tparam V A world vector type
    /// @param v Displacement from the origin expressed in `V::frame`
    ///
    /// Sets the position of this point in the world, expressed in `V::frame`.
    /// This position is fixed in `V::frame` which may not be the same as the
    /// `world::root`.
    template <kinematic::vector V>
    requires in_world_v<typename V::frame>
    constexpr auto position(V v) -> void { displacement_ = v; }

    /// @brief Obtains the point's set position
    constexpr auto position() const& -> const vector_variant&
    {
        return displacement_;
    }

    /// @brief Obtains the point's position, expressed in the specified frame
    /// @tparam F Observation frame
    /// @param w A world instance
    template <kinematic::frame F>
    requires in_world_v<F>
    constexpr auto position(const world& w) const -> vector<F>
    {
        return std::visit(
            [&w](const auto& v) { return v.template in<F>(w); }, position());
    }

  private:
    /// Displacement from world origin
    vector_variant displacement_{};
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
            [out = ctx.out()](const auto& v) {
                return format_to(out, "point at {}", v);
            },
            p.position());
    }
};
