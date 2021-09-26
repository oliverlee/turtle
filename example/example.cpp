#include "fmt/core.h"
#include "turtle/turtle.hpp"

#include <cmath>
#include <numbers>

auto main() -> int
{
    fmt::print("hello! - 🐢 \n");

    using N = turtle::frame<"N">;
    constexpr auto n = N{};
    fmt::print("frame: {}\n", n);

    constexpr auto v = N::vector{1., 2., 3.};
    fmt::print("v: {}\n", v);

    constexpr auto u = 1 * n.x + 2 * n.y + 3 * n.z;
    fmt::print("u: {:.2f}\n", u);

    using std::numbers::pi;

    fmt::print("u: {}\n",
               turtle::rotate(
                   u, turtle::quaternion{std::cos(pi / 2. / 2.), std::sin(pi / 2. / 2.), 0., 0.}));

    fmt::print("u: {}\n", turtle::rotate(u, turtle::quaternion{pi / 2., N::vector{1., 0., 0.}}));

    return 0;
}
