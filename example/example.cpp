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
    using A = frame<"A">;
    using B = frame<"B">;

    const auto w = world{orientation<N, A>{pi / 2., N::vector{1., 0., 0.}},
                         orientation<N, B>{-pi / 6., N::vector{0., 0., 1.}}};
    fmt::print("using world w:\n{}\n", w);

    constexpr auto r1 = A::position{1., 2., 3.};
    fmt::print("position {} in frame {} is {}\n", r1, B{}, r1.in<B>(w));

    using P = decltype(w)::point;

    constexpr auto p = P{};
    fmt::print("\ncreating a point p at w's origin {}\n", p);
    fmt::print("expressing p in {}\n", p.position<B>(w));

    auto q = P{A::position{0., 2., 1.}};
    fmt::print("\nand a point q {}\n", q);
    fmt::print("expressing in {}\n", q.position<B>(w));

    constexpr auto v1 = N::velocity{3, 0, 0};
    fmt::print("v1: {:.2f}\n", v1);

    constexpr auto v2 = turtle::velocity<B, A>{3, 0, 0};
    fmt::print("v2: {:.2f}\n", v2);

    q.velocity(v2);

    return 0;
}
