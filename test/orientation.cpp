#include "turtle/orientation.hpp"

#include "boost/ut.hpp"
#include "test/util/within.hpp"
#include "turtle/frame.hpp"

#include <cmath>
#include <tuple>

using N = turtle::frame<"N">;
using A = turtle::frame<"A">;

auto main() -> int
{
    using namespace boost::ut;
    using turtle::test::within;

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

    test("orientation axis-angle constructible non-basis axis") = []<class T>() {
        using B = turtle::frame<"B", T>;
        using C = turtle::frame<"C", T>;

        constexpr auto angle = T(0.2);
        const auto axis = normalized(typename B::vector{T{1}, T{1}, T{1}});

        const auto ori = turtle::orientation<B, C>{angle, axis};

        constexpr auto tol = T(std::is_same_v<float, T> ? 1.0e-6 : 1.0e-9);

        expect(within<tol>(angle, ori.angle()));
        expect(eq(axis, ori.axis()));
    } | std::tuple<float, double>{};
}
