#include "turtle/turtle.hpp"

#include "fmt/core.h"

auto main() -> int
{
    using turtle::frame;
    using turtle::orientation;
    using turtle::world;

    using N = frame<"Inertial">;
    using Y = frame<"Yaw">;
    using L = frame<"Lean">;
    using R = frame<"Roll">;

    constexpr auto make_world =
        [](N::scalar q1 = {},
           N::scalar q2 = {},
           N::scalar q3 = {},
           N::scalar u1 = {},
           N::scalar u2 = {},
           N::scalar u3 = {}) {
            return world{
                orientation<N, Y>{q1, N::z},
                orientation<Y, L>{q2, Y::x},
                orientation<L, R>{q3, L::y}.with(L::velocity{u1, u2, u3}),
            };
        };

    using P = decltype(make_world())::point;
    auto dmc = P{L::position{0, 0, 1}};
    dmc.velocity(R::velocity{});

    {
        const auto w = make_world(0.1, 0.2, 0.3, 2, 3, 4);
        fmt::print("using world w:\n{}\n", w);
        fmt::print("vel of disc mass center: {:.2f}\n", dmc.velocity<N>(w));
        fmt::print("vel of disc mass center: {:.2f}\n", dmc.velocity<N, L>(w));
    }

    {
        const auto w = make_world(1, 2, 3, 2, 3, 4);
        fmt::print("\n\nusing world w:\n{}\n", w);
        fmt::print("vel of disc mass center: {:.2f}\n", dmc.velocity<N>(w));
        fmt::print("vel of disc mass center: {:.2f}\n", dmc.velocity<N, L>(w));
    }
}
