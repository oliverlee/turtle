#include "turtle/quaternion.hpp"

#include "turtle/frame.hpp"
#include "turtle/vector.hpp"
#include "test/util/within.hpp"

#include "boost/ut.hpp"

#include <cmath>
#include <numbers>

using N = turtle::frame<"N">;

auto main() -> int
{
    using namespace boost::ut;
    using turtle::test::within;

    test("quaternion default constructible") = [] {
        constexpr auto q = turtle::quaternion<int>{};

        expect(0_i == q.w());
        expect(0_i == q.x());
        expect(0_i == q.y());
        expect(0_i == q.z());
    };

    test("quaternion constructible from 4 scalars") = [] {
        constexpr auto q = turtle::quaternion{1, 2, 3, 4};

        expect(1_i == q.w());
        expect(2_i == q.x());
        expect(3_i == q.y());
        expect(4_i == q.z());
    };

    test("quaternion constructible from vector") = [] {
        using A = turtle::frame<"A", int>;
        constexpr auto q = turtle::quaternion{A::vector{1, 2, 3}};

        expect(0_i == q.w());
        expect(1_i == q.x());
        expect(2_i == q.y());
        expect(3_i == q.z());
    };

    test("quaternion constructible from angle and axis") = [] {
        const auto q = turtle::quaternion{0.2, N::vector{1., 0., 0.}};

        expect(within<1.e-9>(std::cos(0.1), q.w()));
        expect(within<1.e-9>(std::sin(0.1), q.x()));
        expect(0.0_d == q.y());
        expect(0.0_d == q.z());
    };

    test("quaternion angle axis ctor aborts with non-unit axis") = [] {
        expect(aborts([] { turtle::quaternion{0.2, N::vector{1., 0., 1.}}; }));
    };

    test("rotate about x axis") = [] {
        constexpr auto v = N::vector{1., 2., 3.};
        using std::numbers::pi;

        constexpr auto axis = N::vector{1., 0., 0.};

        expect(within<1.e-9>(v, rotate(v, turtle::quaternion{0., axis})));
        expect(within<1.e-9>(v, rotate(v, turtle::quaternion{2. * pi, axis})));

        expect(within<1.e-9>(N::vector{1., -3., 2.},
                             rotate(v, turtle::quaternion{pi / 2., axis})));
        expect(within<1.e-9>(
            N::vector{1., -3., 2.},
            rotate(v, turtle::quaternion{-3. * pi / 2., axis})));

        expect(within<1.e-9>(
            N::vector{1., -2., -3.}, rotate(v, turtle::quaternion{pi, axis})));
        expect(within<1.e-9>(
            N::vector{1., -2., -3.}, rotate(v, turtle::quaternion{-pi, axis})));

        expect(within<1.e-9>(N::vector{1., 3., -2.},
                             rotate(v, turtle::quaternion{-pi / 2., axis})));
        expect(within<1.e-9>(
            N::vector{1., 3., -2.},
            rotate(v, turtle::quaternion{3. * pi / 2., axis})));
    };

    test("rotate about y axis") = [] {
        constexpr auto v = N::vector{1., 2., 3.};
        using std::numbers::pi;

        constexpr auto axis = N::vector{0., 1., 0.};

        expect(within<1.e-9>(v, rotate(v, turtle::quaternion{0., axis})));
        expect(within<1.e-9>(v, rotate(v, turtle::quaternion{2. * pi, axis})));

        expect(within<1.e-9>(N::vector{3., 2., -1.},
                             rotate(v, turtle::quaternion{pi / 2., axis})));
        expect(within<1.e-9>(
            N::vector{3., 2., -1.},
            rotate(v, turtle::quaternion{-3. * pi / 2., axis})));

        expect(within<1.e-9>(
            N::vector{-1., 2., -3.}, rotate(v, turtle::quaternion{pi, axis})));
        expect(within<1.e-9>(
            N::vector{-1., 2., -3.}, rotate(v, turtle::quaternion{-pi, axis})));

        expect(within<1.e-9>(N::vector{-3., 2., 1.},
                             rotate(v, turtle::quaternion{-pi / 2., axis})));
        expect(within<1.e-9>(
            N::vector{-3., 2., 1.},
            rotate(v, turtle::quaternion{3. * pi / 2., axis})));
    };

    test("rotate about z axis") = [] {
        constexpr auto v = N::vector{1., 2., 3.};
        using std::numbers::pi;

        constexpr auto axis = N::vector{0., 0., 1.};

        expect(within<1.e-9>(v, rotate(v, turtle::quaternion{0., axis})));
        expect(within<1.e-9>(v, rotate(v, turtle::quaternion{2. * pi, axis})));

        expect(within<1.e-9>(N::vector{-2., 1., 3.},
                             rotate(v, turtle::quaternion{pi / 2., axis})));
        expect(within<1.e-9>(
            N::vector{-2., 1., 3.},
            rotate(v, turtle::quaternion{-3. * pi / 2., axis})));

        expect(within<1.e-9>(
            N::vector{-1., -2., 3.}, rotate(v, turtle::quaternion{pi, axis})));
        expect(within<1.e-9>(
            N::vector{-1., -2., 3.}, rotate(v, turtle::quaternion{-pi, axis})));

        expect(within<1.e-9>(N::vector{2., -1., 3.},
                             rotate(v, turtle::quaternion{-pi / 2., axis})));
        expect(within<1.e-9>(
            N::vector{2., -1., 3.},
            rotate(v, turtle::quaternion{3. * pi / 2., axis})));
    };

    test("rotate aborts with non-unit quaternion") = [] {
        expect(aborts([] {
            rotate(N::vector{1., 0., 0.}, turtle::quaternion{1., 1., 1., 1.});
        }));
    };
}
