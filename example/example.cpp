#include "fmt/core.h"
#include "turtle/turtle.hpp"

auto main() -> int
{
    fmt::print("hello! - 🐢 \n");

    constexpr auto v = turtle::vector{1., 2., 3.};

    fmt::print("v: {:.2f}\n", v);

    return 0;
}
