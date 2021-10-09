#include "turtle/turtle.hpp"

#include "fmt/core.h"
#include "fmt/ranges.h"

#include "boost/ut.hpp"

auto main() -> int
{
    using namespace boost::ut;

    test("nothing is a fmt::range") = [] {
        using N = turtle::frame<"N">;
        using A = turtle::frame<"A">;

        using V = N::vector;
        using Q = turtle::quaternion<A::scalar>;

        using O = turtle::orientation<N, A>;
        using W = decltype(turtle::world{O{}});
        using P = W::point;

        static_assert(not fmt::is_range<N, char>::value);
        static_assert(not fmt::is_range<V, char>::value);
        static_assert(not fmt::is_range<Q, char>::value);
        static_assert(not fmt::is_range<O, char>::value);
        static_assert(not fmt::is_range<W, char>::value);
        static_assert(not fmt::is_range<P, char>::value);
    };
}
