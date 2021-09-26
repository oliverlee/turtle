#include "turtle/orientation.hpp"

#include "boost/ut.hpp"
#include "turtle/frame.hpp"

#include <cmath>

using N = turtle::frame<"N">;
using A = turtle::frame<"A">;

auto main() -> int
{
    using namespace boost::ut;

    test("orientation default constructible") = [] {
        constexpr auto ori = turtle::orientation<N, A>{};

        expect(0.0_d == ori.angle());

        const auto axis = ori.axis();
        expect(std::isnan(axis.x()));
        expect(std::isnan(axis.y()));
        expect(std::isnan(axis.z()));
    };

    test("orientation axis-angle constructible") = [] {
        constexpr auto angle = 0.1;
        constexpr auto axis = N::vector{1., 0., 0.};

        const auto ori = turtle::orientation<N, A>{angle, axis};

        expect(eq(angle, ori.angle()));
        expect(eq(axis, ori.axis()));
    };
}
