#include "turtle/vector.hpp"

#include "boost/ut.hpp"

#include <algorithm>
#include <functional>
#include <type_traits>

void test_disabled_constructor()
{
    using V = turtle::vector<int>;

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
    using V = turtle::vector<int>;

    static_assert(not std::is_invocable_v<std::less<>, V, V>);
    static_assert(not std::is_invocable_v<std::less_equal<>, V, V>);
    static_assert(not std::is_invocable_v<std::greater<>, V, V>);
    static_assert(not std::is_invocable_v<std::greater_equal<>, V, V>);
};

auto main() -> int
{
    using namespace boost::ut;

    test("DefaultConstruct") = [] {
        constexpr auto v = turtle::vector<int>{};

        expect(0_i == v.x());
        expect(0_i == v.y());
        expect(0_i == v.z());
    };

    test("ConstructFromValues") = [] {
        constexpr auto v = turtle::vector{1, 2, 3};

        expect(1_i == v.x());
        expect(2_i == v.y());
        expect(3_i == v.z());
    };

    test("SetValues") = [] {
        auto v = turtle::vector<int>{};

        v.x() = 1;
        v.y() = 2;
        v.z() = 3;

        expect(1_i == v.x());
        expect(2_i == v.y());
        expect(3_i == v.z());
    };

    test("IsConstRange") = [] {
        constexpr auto v = turtle::vector<int>{};

        expect(3_l == std::ranges::count(v, 0));
    };

    test("IsMutableRange") = [] {
        auto v = turtle::vector<int>{};
        std::ranges::for_each(v, [](auto& x) { ++x; });

        expect(3_l == std::ranges::count(v, 1));
    };

    test("EqualityComparable") = [] {
        constexpr auto v = turtle::vector<int>{};
        constexpr auto u = turtle::vector<int>{};

        expect(u == v);

        constexpr auto w = turtle::vector{1, 0, 0};
        expect(u != w);
    };
}
