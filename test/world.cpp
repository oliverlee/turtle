#include "turtle/world.hpp"

#include "boost/ut.hpp"
#include "turtle/frame.hpp"
#include "turtle/orientation.hpp"

auto main() -> int
{
    using namespace boost::ut;

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

        const auto& ori = w.orientation<A, B>();

        static_assert(std::is_same_v<const turtle::orientation<A, B>&, decltype(ori)>);
        expect(eq(angle, ori.angle()));
        expect(eq(axis, ori.axis()));
    };
}
