#include "turtle/world.hpp"

#include "boost/ut.hpp"
#include "test/util/within.hpp"
#include "turtle/frame.hpp"
#include "turtle/orientation.hpp"

#include <numbers>

auto main() -> int
{
    using namespace boost::ut;
    using turtle::test::within;

    test("world constructible - single chain") = [] {
        using N = turtle::frame<"N">;
        using A = turtle::frame<"A">;

        constexpr auto w = turtle::world{
            turtle::orientation<N, A>{},
        };
        (void)w;

        using W = std::remove_cvref_t<decltype(w)>;
        using turtle::meta::type_tree;
        static_assert(std::is_same_v<type_tree<N, A>, W::frame_tree_type>);
    };

    test("world constructible - simple chain") = [] {
        using N = turtle::frame<"N">;
        using A = turtle::frame<"A">;
        using B = turtle::frame<"B">;
        using C = turtle::frame<"C">;

        constexpr auto w = turtle::world{
            turtle::orientation<N, A>{},
            turtle::orientation<A, B>{},
            turtle::orientation<B, C>{},
        };

        using W = std::remove_cvref_t<decltype(w)>;
        using turtle::meta::type_tree;
        static_assert(
            std::is_same_v<type_tree<N, type_tree<A, type_tree<B, C>>>, W::frame_tree_type>);
    };

    test("world constructible - simple tree") = [] {
        using N = turtle::frame<"N">;
        using A = turtle::frame<"A">;
        using B = turtle::frame<"B">;
        using C = turtle::frame<"C">;

        constexpr auto w = turtle::world{
            turtle::orientation<N, A>{},
            turtle::orientation<A, B>{},
            turtle::orientation<N, C>{},
        };

        using W = std::remove_cvref_t<decltype(w)>;
        using turtle::meta::type_tree;
        static_assert(std::is_same_v<type_tree<N, type_tree<A, B>, C>, W::frame_tree_type>);
    };

    test("world access orientation") = [] {
        using N = turtle::frame<"N">;
        using A = turtle::frame<"A">;
        using B = turtle::frame<"B">;
        using C = turtle::frame<"C">;

        constexpr auto angle = 0.1;
        constexpr auto axis = A::vector{1., 0., 0.};

        const auto w = turtle::world{
            turtle::orientation<N, A>{},
            turtle::orientation<A, B>{angle, axis},
            turtle::orientation<N, C>{},
        };

        const auto& ori = w.get<A, B>();

        static_assert(std::is_same_v<const turtle::orientation<A, B>&, decltype(ori)>);
        expect(eq(angle, ori.angle()));
        expect(eq(axis, ori.axis()));
    };

    test("world expression to frame with single chain") = [] {
        using N = turtle::frame<"N">;
        using A = turtle::frame<"A">;

        constexpr auto angle = std::numbers::pi / 2.;
        constexpr auto axis = N::vector{1., 0., 0.};

        const auto w = turtle::world{
            turtle::orientation<N, A>{angle, axis},
        };

        {
            const auto v = N::vector{1., 0., 0.};
            expect(eq(v, v.in(w.express<N>())));
            expect(eq(A::vector{1., 0., 0.}, v.in(w.express<A>())));
        }

        {
            const auto v = N::vector{1., 1., 0.};
            expect(eq(v, v.in(w.express<N>())));
            expect(within<1e-12>(A::vector{1., 0., -1.}, v.in(w.express<A>())));
        }
    };

    test("world expression to frame with 2 chain") = [] {
        using N = turtle::frame<"N">;
        using A = turtle::frame<"A">;
        using B = turtle::frame<"B">;

        constexpr auto angle = std::numbers::pi / 2.;

        const auto w = turtle::world{turtle::orientation<N, A>{angle, N::vector{1., 0., 0.}},
                                     turtle::orientation<A, B>{angle, A::vector{1., 0., 0.}}};

        expect(within<1e-9>(B::vector{0., -1., 0.}, N::vector{0., 1., 0.}.in(w.express<B>())));
    };

    test("world expression to from with 2 branches") = [] {
        using N = turtle::frame<"N">;
        using A = turtle::frame<"A">;
        using B = turtle::frame<"B">;

        constexpr auto angle = std::numbers::pi / 2.;

        const auto w = turtle::world{turtle::orientation<N, A>{angle, N::vector{1., 0., 0.}},
                                     turtle::orientation<N, B>{-angle, N::vector{1., 0., 0.}}};

        expect(within<1e-9>(B::vector{0., -1., 0.}, A::vector{0., 1., 0.}.in(w.express<A, B>())));
    };
}
