# turtle
A reference frame library

### What is this?

A library for kinematics calculations that does all the book keeping for you.
Think
[sympy.physics.vector](https://docs.sympy.org/latest/modules/physics/vector/index.html)
but in C++ and strongly typed. 
Here's how you could calculate the velocity of
[disc](https://docs.sympy.org/latest/modules/physics/mechanics/examples/rollingdisc_example_kane.html)

~~~cpp
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
    
    constexpr auto q1 = 0.1;
    constexpr auto q2 = 0.2;
    constexpr auto q3 = 0.3;

    constexpr auto u1 = 2.;
    constexpr auto u2 = 3.;
    constexpr auto u3 = 4.;

    const auto w = world{
        orientation<N, Y>{q1, N::z},
        orientation<Y, L>{q2, Y::x},
        orientation<L, R>{q3, L::y}.with(L::velocity{u1, u2, u3}),
    };
    fmt::print("using world w:\n{}\n", w);

    using P = decltype(w)::point;
    constexpr auto dmc = P{L::position{0, 0, 1}, R::velocity{}};
    fmt::print("vel of disc mass center: {:.2f}\n", dmc.velocity<N, L>(w));
}
~~~

This is a work in progress.

### Building
Concepts and ranges are used in this library, so you'll need a compiler with
decent C++20 support.

Run the example with

    bazel run //example

### Testing
Run the tests with

    bazel test //...

### Linting
Run `clang-tidy` with

    bazel build --config=clang-tidy //...

This will require an install of LLVM 14.

### Documentation
Generate and view with

    bazel run @mcss//:doxygen
    open bazel-out/build-doc/html/index.html

