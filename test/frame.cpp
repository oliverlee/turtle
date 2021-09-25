#include "turtle/frame.hpp"

#include "boost/ut.hpp"

#include <type_traits>

void test_frame_trait()
{
    using N = turtle::frame<"N">;

    static_assert(turtle::is_frame_v<N>);
    static_assert(turtle::is_frame_v<turtle::frame<"A">>);

    static_assert(not turtle::is_frame_v<int>);
    static_assert(not turtle::is_frame_v<turtle::vector<N>>);
    static_assert(not turtle::is_frame_v<N::vector>);

    static_assert(std::is_same_v<N, turtle::frame<"N">>);
    static_assert(not std::is_same_v<N, turtle::frame<"A">>);
};

auto main() -> int
{
    using namespace boost::ut;

    test("Construct") = [] {
        constexpr auto n = turtle::frame<"N">{};
        (void)n;
    };

    test("Name") = [] {
        constexpr auto n = turtle::frame<"N">{};

        expect("N" == n.name);
    };

    test("BasisVectors") = [] {
        using N = turtle::frame<"N">;

        constexpr auto v = N::vector{1., 2., 3.};
        constexpr auto u = 1 * N::x + 2 * N::y + 3 * N::z;

        expect(u == v);
    };

    test("BasisVectorsWithFrameValue") = [] {
        constexpr auto n = turtle::frame<"N">{};

        constexpr auto v = std::remove_cvref_t<decltype(n)>::vector{1., 2., 3.};
        constexpr auto u = 1 * n.x + 2 * n.y + 3 * n.z;

        expect(u == v);
    };

    test("BasisVectorsWithFrameAssociatedVectorTypeAlias") = [] {
        using N = turtle::frame<"N">;
        constexpr auto n = N{};

        constexpr auto v = N::vector{1., 2., 3.};
        constexpr auto u = 1 * n.x + 2 * n.y + 3 * n.z;

        expect(u == v);
    };
}
