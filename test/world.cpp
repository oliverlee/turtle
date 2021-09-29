#include "turtle/world.hpp"

#include "boost/ut.hpp"
#include "test/util/within.hpp"
#include "turtle/frame.hpp"
#include "turtle/orientation.hpp"

#include <numbers>

auto main() -> int
{
    using namespace boost::ut;
    using turtle::frame;
    using turtle::orientation;
    using turtle::world;
    using turtle::test::within;

    test("world constructible - single chain") = [] {
        using N = frame<"N">;
        using A = frame<"A">;

        constexpr auto w = world{
            orientation<N, A>{},
        };
        (void)w;

        using W = std::remove_cvref_t<decltype(w)>;
        using turtle::meta::type_tree;
        static_assert(std::is_same_v<type_tree<N, A>, W::frame_tree_type>);
    };

    test("world constructible - simple chain") = [] {
        using N = frame<"N">;
        using A = frame<"A">;
        using B = frame<"B">;
        using C = frame<"C">;

        constexpr auto w = world{
            orientation<N, A>{},
            orientation<A, B>{},
            orientation<B, C>{},
        };

        using W = std::remove_cvref_t<decltype(w)>;
        using turtle::meta::type_tree;
        static_assert(
            std::is_same_v<type_tree<N, type_tree<A, type_tree<B, C>>>, W::frame_tree_type>);
    };

    test("world constructible - simple tree") = [] {
        using N = frame<"N">;
        using A = frame<"A">;
        using B = frame<"B">;
        using C = frame<"C">;

        constexpr auto w = world{
            orientation<N, A>{},
            orientation<A, B>{},
            orientation<N, C>{},
        };

        using W = std::remove_cvref_t<decltype(w)>;
        using turtle::meta::type_tree;
        static_assert(std::is_same_v<type_tree<N, type_tree<A, B>, C>, W::frame_tree_type>);
    };

    test("world access orientation") = [] {
        using N = frame<"N">;
        using A = frame<"A">;
        using B = frame<"B">;
        using C = frame<"C">;

        constexpr auto angle = 0.1;
        constexpr auto axis = A::vector{1., 0., 0.};

        const auto w = world{
            orientation<N, A>{},
            orientation<A, B>{angle, axis},
            orientation<N, C>{},
        };

        const auto& ori = w.get<A, B>();

        static_assert(std::is_same_v<const orientation<A, B>&, decltype(ori)>);
        expect(eq(angle, ori.angle()));
        expect(eq(axis, ori.axis()));
    };

    test("express vector in world with single chain") = [] {
        using N = frame<"N">;
        using A = frame<"A">;

        constexpr auto angle = std::numbers::pi / 2.;
        constexpr auto axis = N::vector{1., 0., 0.};

        const auto w = world{
            orientation<N, A>{angle, axis},
        };

        {
            const auto v = N::vector{1., 0., 0.};
            expect(eq(v, v.in<N>(w)));
            expect(eq(A::vector{1., 0., 0.}, v.in<A>(w)));
        }

        {
            const auto v = N::vector{1., 1., 0.};
            expect(eq(v, v.in<N>(w)));
            expect(within<1e-12>(A::vector{1., 0., -1.}, v.in<A>(w)));
        }
    };

    test("express vector in world with 2 chains") = [] {
        using N = frame<"N">;
        using A = frame<"A">;
        using B = frame<"B">;

        constexpr auto angle = std::numbers::pi / 2.;

        const auto w = world{orientation<N, A>{angle, N::vector{1., 0., 0.}},
                             orientation<A, B>{angle, A::vector{1., 0., 0.}}};

        expect(within<1e-12>(B::vector{0., -1., 0.}, N::vector{0., 1., 0.}.in<B>(w)));
    };

    test("express vector in world with 2 branches") = [] {
        using N = frame<"N">;
        using A = frame<"A">;
        using B = frame<"B">;

        constexpr auto angle = std::numbers::pi / 2.;

        const auto w = world{orientation<N, A>{angle, N::vector{1., 0., 0.}},
                             orientation<N, B>{-angle, N::vector{1., 0., 0.}}};

        expect(within<1e-12>(B::vector{0., -1., 0.}, A::vector{0., 1., 0.}.in<B>(w)));
    };
}
