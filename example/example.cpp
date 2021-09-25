#include "fmt/core.h"
#include "turtle/turtle.hpp"

auto main() -> int
{
    fmt::print("hello! - 🐢 \n");

    using N = turtle::frame<"N">;

    fmt::print("frame: {}\n", N{});

    constexpr auto v = turtle::make_vector<N>(1., 2., 3.);

    fmt::print("v: {:.2f}\n", v);

    return 0;
}
