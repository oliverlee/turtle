#include "turtle/turtle.hpp"

#include "fmt/core.h"

#include <numbers>

auto main() -> int
{
    using std::numbers::pi;
    using turtle::frame;
    using turtle::orientation;
    using turtle::world;

    using N = frame<"N">;

    constexpr auto n = N{};
    fmt::print("frame: {}\n", n);

    constexpr auto v = 1 * n.x + 2 * n.y + 3 * n.z;
    fmt::print("v: {:.2f}\n", v);

    using A = frame<"A">;
    using B = frame<"B">;

    const auto w = world{orientation<N, A>{pi / 2, N::vector{1, 0, 0}},
                         orientation<N, B>{-pi / 6, N::vector{0, 0, 1}}};
    fmt::print("using world w:\n{}\n", w);

    constexpr auto u = A::vector{1, 2, 3};
    fmt::print("vector {} in frame {} is {}\n", u, B{}, u.in<B>(w));

    using P = decltype(w)::point;

    auto p = P{};
    fmt::print("\ncreating a point p at w's origin {}\n", p);
    fmt::print("expressing p in {}\n", p.position<B>(w));

    constexpr auto q = P{
        A::vector{0, 2, 1}
        //      position(A::vector{0, 2, 1}),
        //      velocity(A::vector{1, 0, 0})
    };
    fmt::print("\nand a point q {}\n", q);
    fmt::print("expressing in {}\n", q.position<B>(w));

    constexpr auto va = velocity(A::vector{1, 0, 0});
    constexpr auto vb = velocity<B>(A::vector{1, 0, 0});
    fmt::print("{}\n{}", va, vb);

    // change setters to ??
    // p.set(position(B::vector{}));
    // p.set(velocity<A>(B::vector{}));
    p.velocity(velocity<A>(B::vector{0, 1, 0}));

    auto x = p.velocity<N>(w);
    (void)x;

    return 0;
}
