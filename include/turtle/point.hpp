#pragma once

#include "derivative.hpp"
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
    using frame_variant =
        metal::apply<metal::lambda<std::variant>,
                     meta::flatten<typename World::tree>>;

    using vector_variant = metal::apply<
        metal::lambda<std::variant>,
        metal::transform<metal::lambda<vector>, metal::as_list<frame_variant>>>;

    struct velocity_variant {
        frame_variant observer;
        vector_variant value;
    };

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
    /// @tparam V A world vector type
    /// @param pos Displacement from the origin expressed in `V::frame`
    ///
    /// Sets the position of this point in the world, expressed in `V::frame`.
    /// This position is fixed in `V::frame` which may not be the same as the
    /// `world::root`.
    template <kinematic::vector V>
    requires in_world_v<typename V::frame>
    explicit constexpr point(V pos)
        : displacement_{std::in_place_type<V>, std::move(pos)}, velocity_{}
    {}

    /// @brief Sets the point's position
    /// @tparam V A world vector type
    /// @param pos Displacement from the origin expressed in `V::frame`
    ///
    /// Sets the position of this point in the world, expressed in `V::frame`.
    /// This position is fixed in `V::frame` which may not be the same as the
    /// `world::root`.
    template <kinematic::vector V>
    requires in_world_v<typename V::frame>
    constexpr auto position(V pos) -> void { displacement_ = std::move(pos); }

    /// @brief Obtains the point's set position
    [[nodiscard]] constexpr auto position() const& -> const vector_variant&
    {
        return displacement_;
    }

    /// @brief Obtains the point's position, expressed in the specified frame
    /// @tparam F Observation frame
    /// @param w A world instance
    template <kinematic::frame F>
    requires in_world_v<F>
    [[nodiscard]] constexpr auto position(const world& w) const -> vector<F>
    {
        return std::visit(
            [&w](const auto& v) { return v.template in<F>(w); }, position());
    }

    template <kinematic::frame B, kinematic::vector V>
    requires in_world_v<B> && in_world_v<typename V::frame>
    constexpr auto velocity(derivative<B, V, 1> vel) -> void
    {
        velocity_.observer.template emplace<B>();
        velocity_.value = std::move(vel.value);
    }

    template <kinematic::frame A, kinematic::frame F = A>
    requires in_world_v<A> && in_world_v<F>
    [[nodiscard]] constexpr auto velocity(const world& w) const
        -> derivative<A, typename F::vector, 1>
    {
        const auto v_A_B_bar =
            std::visit([&w](const auto& v) { return v.template in<A>(w); },
                       velocity_.value);

        const auto w_A_B = std::visit(
            [&w]<class B>(B) {
                // FIXME get angular velocity instead of rotation - the code
                // below just something that should compile
                //
                // return w.template angular_velocity<A, B>();
                return w.template express<A, B>().axis();
            },
            velocity_.observer);

        return {(v_A_B_bar + cross_product(w_A_B, position<A>(w)))
                    .template in<F>(w)};
    }

  private:
    /// Displacement from world origin
    vector_variant displacement_{};

    /// Observed velocity
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
            [out = ctx.out()](const auto& v) {
                return format_to(out, "point at {}", v);
            },
            p.position());
    }
};
