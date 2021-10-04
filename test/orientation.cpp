#include "turtle/orientation.hpp"

#include "turtle/frame.hpp"
#include "test/util/within.hpp"

#include "boost/ut.hpp"

#include <cmath>
#include <numbers>
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

        const auto& q = ori.rotation();
        expect(1.0_d == q.w());
        expect(0.0_d == q.x());
        expect(0.0_d == q.y());
        expect(0.0_d == q.z());
    };

    test("orientation axis-angle constructible") = [] {
        constexpr auto angle = 0.1;
        constexpr auto axis = N::vector{1., 0., 0.};

        const auto ori = turtle::orientation<N, A>{angle, axis};

        expect(eq(angle, ori.angle()));
        expect(eq(axis, ori.axis()));
    };

    test(
        "orientation axis-angle constructible non-basis axis") = []<class T>() {
        using B = turtle::frame<"B", T>;
        using C = turtle::frame<"C", T>;

        constexpr auto angle = T(0.2);
        const auto axis = normalized(typename B::vector{T{1}, T{1}, T{1}});

        const auto ori = turtle::orientation<B, C>{angle, axis};

        constexpr auto tol = T(std::is_same_v<float, T> ? 1e-6 : 1e-12);

        expect(within<tol>(angle, ori.angle()));
        expect(eq(axis, ori.axis()));
    } | std::tuple<float, double>{};

    {
        using B = turtle::frame<"B">;

        constexpr auto angle = 0.1;
        constexpr auto axis = N::vector{1., 0., 0.};

        const auto ori1 = turtle::orientation<N, A>{angle, axis};

        test("orientation composable with same angle, axis") = [&] {
            const auto ori2 =
                turtle::orientation<A, B>{angle, ori1.rotate(axis)};

            const auto ori12 = ori1 * ori2;
            expect(within<1e-12>(2. * angle, ori12.angle()));
            expect(eq(axis, ori12.axis()));
        };

        test("orientation composable with same axis, inverse angle") = [&] {
            const auto ori2 =
                turtle::orientation<A, B>{-angle, ori1.rotate(axis)};

            const auto ori12 = ori1 * ori2;

            expect(within<1e-12>(0., ori12.angle()));
            expect(eq(turtle::quaternion{1., 0., 0., 0.}, ori12.rotation()));
        };

        test("orientation composable with different axes") = [] {
            constexpr auto angle = std::numbers::pi / 2.;
            constexpr auto axis = N::vector{1., 0., 0.};

            const auto ori1 = turtle::orientation<N, A>{angle, axis};
            const auto ori2 =
                turtle::orientation<A, B>{angle, A::vector{0., 1., 0.}};

            const auto ori12 = ori1 * ori2;

            expect(within<1e-12>(2. * std::numbers::pi / 3., ori12.angle()));
            expect(
                within<1e-12>(normalized(N::vector{1., 1., 1.}), ori12.axis()));
        };
    }
}
