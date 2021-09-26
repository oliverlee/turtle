#include "turtle/frame.hpp"

#include "boost/ut.hpp"

#include <string_view>
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

    test("frame is default constructible") = [] {
        constexpr auto n = turtle::frame<"">{};
        (void)n;
    };

    test("frame has a name") = [] {
        using namespace std::literals::string_view_literals;
        constexpr auto n = turtle::frame<"N">{};

        expect(eq("N"sv, n.name));
    };

    test("frame has associated vector type") = [] {
        using N = turtle::frame<"N">;

        constexpr auto v = N::vector{1., 2., 3.};
        (void)v;
    };

    test("frame basis vectors used via frame type") = [] {
        using N = turtle::frame<"N">;

        constexpr auto v = N::vector{1., 2., 3.};
        constexpr auto u = 1 * N::x + 2 * N::y + 3 * N::z;

        expect(eq(u, v));
    };

    test("frame basis vectors used via frame value") = [] {
        constexpr auto n = turtle::frame<"N">{};

        constexpr auto v = std::remove_cvref_t<decltype(n)>::vector{1., 2., 3.};
        constexpr auto u = 1 * n.x + 2 * n.y + 3 * n.z;

        expect(eq(u, v));
    };
}
