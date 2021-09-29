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

    using C = frame<"C">;
    using D = frame<"D">;
    using E = frame<"E">;
    using F = frame<"F">;
    using G = frame<"G">;

    const auto w = world{orientation<N, A>{pi / 2., N::vector{1., 0., 0.}},
                         orientation<N, B>{-pi / 6., N::vector{0., 0., 1.}},
                         orientation<B, C>{pi / 4., B::vector{0., 1., 0.}},
                         orientation<A, D>{pi / 4., A::vector{0., 1., 0.}},
                         orientation<D, E>{pi / 4., D::vector{0., 1., 0.}},
                         orientation<A, F>{pi / 4., A::vector{0., 1., 0.}},
                         orientation<A, G>{pi / 4., A::vector{0., 1., 0.}}};

    fmt::print("using world w:\n{}\n", w);

    constexpr auto u = A::vector{1., 2., 3.};
    fmt::print("vector {} in frame {} is {}\n", u, B{}, u.in<B>(w));

    return 0;
}
