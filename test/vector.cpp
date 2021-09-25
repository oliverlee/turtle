#include "turtle/vector.hpp"

#include "boost/ut.hpp"
#include "turtle/frame.hpp"

#include <algorithm>
#include <functional>
#include <type_traits>

using N = turtle::frame<"N", int>;

void test_disabled_constructor()
{
    using V = turtle::vector<N>;

    static_assert(std::is_constructible_v<V>);
    static_assert(std::is_constructible_v<V, int, int, int>);

    static_assert(not std::is_constructible_v<V, int>);
    static_assert(not std::is_constructible_v<V, int, int>);
    static_assert(not std::is_constructible_v<V, int, int, unsigned>);
    static_assert(not std::is_constructible_v<V, int, bool, int>);
    static_assert(not std::is_constructible_v<V, short, int, int>);
    static_assert(not std::is_constructible_v<V, short, short, short>);
};

void test_disabled_comparisons()
{
    using V = turtle::vector<N>;

    static_assert(not std::is_invocable_v<std::less<>, V, V>);
    static_assert(not std::is_invocable_v<std::less_equal<>, V, V>);
    static_assert(not std::is_invocable_v<std::greater<>, V, V>);
    static_assert(not std::is_invocable_v<std::greater_equal<>, V, V>);
};

void test_disabled_cross_frame_operation()
{
    using A = turtle::frame<"A">;

    constexpr auto v = turtle::vector<N>{};
    constexpr auto u = turtle::vector<A>{};

    static_assert(not std::is_invocable_v<std::plus<>, decltype(v), decltype(u)>);
};

auto main() -> int
{
    using namespace boost::ut;

    test("DefaultConstruct") = [] {
        constexpr auto v = turtle::vector<N>{};

        expect(0_i == v.x());
        expect(0_i == v.y());
        expect(0_i == v.z());
    };

    test("ConstructFromValues") = [] {
        constexpr auto v = N::vector{1, 2, 3};

        expect(1_i == v.x());
        expect(2_i == v.y());
        expect(3_i == v.z());
    };

    test("SetValues") = [] {
        auto v = turtle::vector<N>{};

        v.x() = 1;
        v.y() = 2;
        v.z() = 3;

        expect(1_i == v.x());
        expect(2_i == v.y());
        expect(3_i == v.z());
    };

    test("IsConstRange") = [] {
        constexpr auto v = turtle::vector<N>{};

        expect(3_l == std::ranges::count(v, 0));
    };

    test("IsMutableRange") = [] {
        auto v = turtle::vector<N>{};
        std::ranges::for_each(v, [](auto& x) { ++x; });

        expect(3_l == std::ranges::count(v, 1));
    };

    test("EqualityComparable") = [] {
        constexpr auto v = turtle::vector<N>{};
        constexpr auto u = turtle::vector<N>{};

        expect(u == v);

        constexpr auto w = turtle::vector<N>{1, 0, 0};
        expect(u != w);
    };

    test("Add") = [] {
        constexpr auto v = turtle::vector<N>{1, 2, 3};
        constexpr auto u = turtle::vector<N>{3, 2, 1};

        constexpr auto w = turtle::vector<N>{4, 4, 4};
        expect(w == (v + u));
    };

    test("Subtract") = [] {
        constexpr auto v = turtle::vector<N>{1, 2, 3};
        constexpr auto u = turtle::vector<N>{3, 2, 1};

        constexpr auto w = turtle::vector<N>{-2, 0, 2};
        expect(w == (v - u));
    };

    test("LeftMul") = [] {
        constexpr auto a = 2;
        constexpr auto v = turtle::vector<N>{1, 2, 3};

        constexpr auto u = turtle::vector<N>{2, 4, 6};
        expect(u == a * v);
    };

    test("RightMul") = [] {
        constexpr auto a = 2;
        constexpr auto v = turtle::vector<N>{1, 2, 3};

        constexpr auto u = turtle::vector<N>{2, 4, 6};
        expect(u == v * a);
    };

    test("RightDiv") = [] {
        constexpr auto a = 2;
        constexpr auto v = turtle::vector<N>{1, 2, 3};

        constexpr auto u = turtle::vector<N>{0, 1, 1};
        expect(u == v / a);
    };

    test("Negate") = [] {
        constexpr auto v = turtle::vector<N>{1, 2, 3};

        constexpr auto u = turtle::vector<N>{-1, -2, -3};
        expect(u == -v);
    };

    test("CompoundAdd") = [] {
        auto v = turtle::vector<N>{1, 2, 3};
        v += turtle::vector<N>{3, 2, 1};

        constexpr auto u = turtle::vector<N>{4, 4, 4};
        expect(u == v);
    };

    test("CompoundSubtract") = [] {
        auto v = turtle::vector<N>{1, 2, 3};
        v -= turtle::vector<N>{3, 2, 1};

        constexpr auto u = turtle::vector<N>{-2, 0, 2};
        expect(u == v);
    };

    test("CompoundMul") = [] {
        auto v = turtle::vector<N>{1, 2, 3};
        v *= 2;

        constexpr auto u = turtle::vector<N>{2, 4, 6};
        expect(u == v);
    };

    test("CompoundDiv") = [] {
        auto v = turtle::vector<N>{1, 2, 3};
        v /= 2;

        constexpr auto u = turtle::vector<N>{0, 1, 1};
        expect(u == v);
    };
}
