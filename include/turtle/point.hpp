#pragma once

#include "fwd.hpp"
#include "position.hpp"
#include "velocity.hpp"
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

    using velocity_variant = metal::cascade<
        metal::combine<meta::flatten<typename World::tree>, metal::number<2>>,
        metal::lambda<std::variant>,
        metal::lambda<turtle::velocity>>;

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

    template <kinematic::frame B, kinematic::frame E>
    requires in_world_v<B> && in_world_v<E>
    constexpr auto velocity(turtle::velocity<B, E> v) -> void
    {
        velocity_ = std::move(v);
    }

    [[nodiscard]] constexpr auto velocity() const& -> const velocity_variant&
    {
        return velocity_;
    }

    template <kinematic::frame A>
    requires in_world_v<A>
    [[nodiscard]] constexpr auto velocity(const world& w) const
        -> turtle::velocity<A>
    {
        const auto v_A_B_bar = std::visit(
            [&w](const auto& v) { return v.template express_in<A>(w); },
            velocity_);

        const auto w_A_B = std::visit(
            [&w]<class B, class E>(const turtle::velocity<B, E>&) {
                // FIXME get angular velocity instead of rotation - the code
                // below just something that should compile
                //
                // return w.template angular_velocity<A, B>();
                return w.template express<A, B>().axis();
            },
            velocity_);

        return {(v_A_B_bar + cross_product(w_A_B, position<A>(w)))};
    }

  private:
    /// Displacement from world origin
    position_variant displacement_{};

    velocity_variant velocity_{};
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
