#include "fmt/core.h"
#include "turtle/turtle.hpp"

#include <numbers>

auto main() -> int
{
    using std::numbers::pi;
    using turtle::frame;
    using turtle::orientation;
    using turtle::world;

    fmt::print("hello! - 🐢 \n");

    using N = frame<"N">;

    constexpr auto n = N{};
    fmt::print("frame: {}\n", n);

    constexpr auto v = 1 * n.x + 2 * n.y + 3 * n.z;
    fmt::print("v: {:.2f}\n", v);

    using A = frame<"A">;
    using B = frame<"B">;

    const auto w = world{orientation<N, A>{pi / 2., N::vector{1., 0., 0.}},
                         orientation<N, B>{-pi / 6., N::vector{0., 0., 1.}}};

    constexpr auto u = A::vector{1., 2., 3.};
    fmt::print("vector {} in frame {} is {}\n", u, B{}, u.in<B>(w));

    return 0;
}
