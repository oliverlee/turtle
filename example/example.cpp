#include "fmt/core.h"
#include "turtle/turtle.hpp"

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

    return 0;
}
