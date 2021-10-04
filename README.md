# turtle
A reference frame library

### What is this?

A library for kinematics calculations that does all the book keeping for you.
Think
[sympy.physics.vector](https://docs.sympy.org/latest/modules/physics/vector/index.html)
but in C++ and strongly typed. And for those coming from Python -
[fmt](https://fmt.dev/latest/index.html) is used for formatting.

~~~cpp
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

    const auto w = world{
        orientation<N, A>{pi / 2., N::vector{1., 0., 0.}},
        orientation<N, B>{-pi / 6., N::vector{0., 0., 1.}}
    };
    fmt::print("using world w:\n{}\n", w);

    constexpr auto u = A::vector{1., 2., 3.};
    fmt::print("{} can also be expressed as {}\n",
        u, u.in<B>(w)
    );
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

### Documentation
Generate and view with

    bazel run @mcss//:doxygen
    open bazel-out/build-doc/html/index.html

