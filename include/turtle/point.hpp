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
    /// @tparam B Velocity observation frame
    /// @tparam B Velocity expression frame
    /// @param r Displacement from the origin expressed in `F`
    /// @param v Velocity observed in `B` and expressed in `E`
    ///
    /// Sets the position of this point in the world, expressed in `F` and the
    /// velocity of this point, observed in `B` and expressed in `E`.
    /// This position is fixed in `F` which may not be the same as the
    /// `world::root`.
    template <kinematic::frame F,
              kinematic::frame B = typename World::root,
              kinematic::frame E = B>
    requires in_world_v<F>
    explicit constexpr point(turtle::position<F> r,
                             turtle::velocity<B, E> v = {})
        : displacement_{std::in_place_type<turtle::position<F>>, std::move(r)},
          velocity_{std::in_place_type<turtle::velocity<B, E>>, std::move(v)}
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

    template <kinematic::frame A, kinematic::frame F = A>
    requires in_world_v<A> && in_world_v<F>
    [[nodiscard]] constexpr auto velocity(const world& w) const
        -> turtle::velocity<A, F>
    {
        // TODO update interfaces to remove need to bit cast so much

        const auto v_A_B_bar = std::visit(
            [&w](const auto& v) {
                return std::bit_cast<turtle::velocity<A>>(
                    v.template express_in<A>(w));
            },
            velocity_);

        const auto w_A_B = std::visit(
            [&w]<class B, class E>(const turtle::velocity<B, E>&) {
                return w.template express<A, B>().angular_velocity();
            },
            velocity_);

        return (v_A_B_bar +
                std::bit_cast<turtle::velocity<A>>(cross_product(
                    std::bit_cast<typename A::vector>(w_A_B),
                    std::bit_cast<typename A::vector>(position<A>(w)))))
            .template express_in<F>(w);
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
