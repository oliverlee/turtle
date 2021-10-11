#include "turtle/position.hpp"

#include "turtle/frame.hpp"
#include "turtle/orientation.hpp"
#include "test/util/within.hpp"

#include "boost/ut.hpp"

#include <numbers>

auto main() -> int
{
    using namespace boost::ut;

    using turtle::frame;

    test("frame position constructible from frame vector") = [] {
        using turtle::position;
        using N = frame<"N">;

        static_assert(
            std::is_same_v<decltype(position{N::vector{}}), N::position>);
    };

    test("frame position not addable with frame vector") = [] {
        using N = frame<"N">;

        const auto add = [](auto a, auto b) -> decltype(a + b) { return {}; };

        static_assert(
            not std::is_invocable_v<decltype(add), N::position, N::vector>);
    };

    test("position expressed in different frame") = [] {
        using std::numbers::pi;
        using turtle::orientation;
        using turtle::test::within;

        using N = frame<"N">;
        using A = frame<"A">;

        constexpr auto r = N::position{1, 2, 3};
        const auto ori = orientation<N, A>{pi / 2., N::vector{1, 0, 0}};

        expect(within<1e-9>(A::position{1, 3, -2}, r.in<A>(ori)));
    };
}
