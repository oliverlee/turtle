#include "fmt/core.h"
#include "turtle/turtle.hpp"

auto main() -> int
{
    fmt::print("hello! - 🐢 \n");

    constexpr auto v = turtle::vector{1., 2., 3.};

    fmt::print("v: {:.2f}\n", v);

    constexpr auto n = turtle::frame<"N">{};

    fmt::print("frame: {}\n", n);
    fmt::print("frame.name: {}\n", n.name);

    return 0;
}
