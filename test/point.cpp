#include "turtle/point.hpp"

#include "turtle/frame.hpp"
#include "turtle/world.hpp"
#include "test/util/within.hpp"

#include "boost/ut.hpp"

#include <numbers>

auto main() -> int
{
    using namespace boost::ut;
    using std::numbers::pi;
    using turtle::frame;
    using turtle::orientation;
    using turtle::world;
    using turtle::test::within;

    using N = frame<"N">;
    using A = frame<"A">;

    const auto w = world{orientation<N, A>{pi / 2., N::vector{0., 0., 1.}}};

    using W = decltype(w);
    using P = W::point;

    constexpr auto reset_world = W{};

    test("point at origin") = [&w, &reset_world] {
        constexpr auto p = P{};

        expect(eq(N::position{}, p.position<N>(reset_world)));
        expect(eq(A::position{}, p.position<A>(reset_world)));

        expect(eq(N::position{}, p.position<N>(w)));
        expect(eq(A::position{}, p.position<A>(w)));
    };

    test("point displaced from origin") = [&w, &reset_world] {
        constexpr auto p = P{A::position{1, 0, 0}};

        expect(eq(N::position{1, 0, 0}, p.position<N>(reset_world)));
        expect(eq(A::position{1, 0, 0}, p.position<A>(reset_world)));

        expect(within<1e-15>(N::position{0, 1, 0}, p.position<N>(w)));
        expect(eq(A::position{1, 0, 0}, p.position<A>(w)));
    };

    test("point displaced from origin with multiple frames") = [] {
        using B = frame<"B">;

        const auto w =
            world{orientation<N, A>{pi / 2., N::vector{0., 0., 1.}},
                  orientation<N, B>{-pi / 2., N::vector{0., 0., 1.}}};

        using Point = decltype(w)::point;

        constexpr auto p = Point{A::position{1, 0, 0}};

        expect(eq(A::position{1, 0, 0}, p.position<A>(w)));
        expect(within<1e-15>(N::position{0, 1, 0}, p.position<N>(w)));
        expect(within<1e-15>(B::position{-1, 0, 0}, p.position<B>(w)));
    };

    test("point position settable") = [&w] {
        auto p = P{};

        p.position(A::position{1, 0, 1});
        expect(eq(A::position{1, 0, 1}, p.position<A>(w)));
    };
}
