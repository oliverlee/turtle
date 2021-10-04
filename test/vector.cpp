#include "turtle/vector.hpp"

#include "turtle/frame.hpp"
#include "turtle/orientation.hpp"
#include "test/util/within.hpp"

#include "boost/ut.hpp"

#include <algorithm>
#include <functional>
#include <numbers>
#include <string_view>
#include <type_traits>

using N = turtle::frame<"N", int>;

void test_disabled_constructor()
{
    using V = turtle::vector<N>;

    static_assert(std::is_constructible_v<V>);
    static_assert(std::is_constructible_v<V, int, int, int>);

    //    static_assert(not std::is_constructible_v<V, int>);
    //    static_assert(not std::is_constructible_v<V, int, int>);
    //    static_assert(not std::is_constructible_v<V, int, int, unsigned>);
    //    static_assert(not std::is_constructible_v<V, int, bool, int>);
    //    static_assert(not std::is_constructible_v<V, short, int, int>);
    //    static_assert(not std::is_constructible_v<V, short, short, short>);
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

    static_assert(
        not std::is_invocable_v<std::plus<>, decltype(v), decltype(u)>);
};

void test_vector_trait()
{
    static_assert(turtle::is_vector_v<N::vector>);
    static_assert(turtle::is_vector_v<turtle::vector<N>>);

    static_assert(not turtle::is_vector_v<int>);
    static_assert(not turtle::is_vector_v<N>);
}

auto main() -> int
{
    using namespace boost::ut;
    using turtle::test::within;

    test("vector is default constructible") = [] {
        constexpr auto v = turtle::vector<N>{};

        expect(0_i == v.x());
        expect(0_i == v.y());
        expect(0_i == v.z());
    };

    test("vector constructible from x, y, z values") = [] {
        constexpr auto v = N::vector{1, 2, 3};

        expect(1_i == v.x());
        expect(2_i == v.y());
        expect(3_i == v.z());
    };

    test("vector components are assignable") = [] {
        auto v = turtle::vector<N>{};

        v.x() = 1;
        v.y() = 2;
        v.z() = 3;

        expect(1_i == v.x());
        expect(2_i == v.y());
        expect(3_i == v.z());
    };

    test("vector is a const range") = [] {
        constexpr auto v = turtle::vector<N>{};

        expect(3_l == std::ranges::count(v, 0));
    };

    test("vector is a mutable range") = [] {
        auto v = turtle::vector<N>{};
        std::ranges::for_each(v, [](auto& x) { ++x; });

        expect(3_l == std::ranges::count(v, 1));
    };

    test("vector is equality comparable") = [] {
        constexpr auto v = turtle::vector<N>{};
        constexpr auto u = turtle::vector<N>{};

        expect(eq(u, v));

        constexpr auto w = turtle::vector<N>{1, 0, 0};
        expect(neq(u, w));
    };

    test("vector addition") = [] {
        constexpr auto v = turtle::vector<N>{1, 2, 3};
        constexpr auto u = turtle::vector<N>{3, 2, 1};

        constexpr auto w = turtle::vector<N>{4, 4, 4};
        expect(eq(w, (v + u)));
    };

    test("vector subtraction") = [] {
        constexpr auto v = turtle::vector<N>{1, 2, 3};
        constexpr auto u = turtle::vector<N>{3, 2, 1};

        constexpr auto w = turtle::vector<N>{-2, 0, 2};
        expect(eq(w, (v - u)));
    };

    test("left multiplication by a scalar") = [] {
        constexpr auto a = 2;
        constexpr auto v = turtle::vector<N>{1, 2, 3};

        constexpr auto u = turtle::vector<N>{2, 4, 6};
        expect(eq(u, a * v));
    };

    test("right multiplication by a scalar") = [] {
        constexpr auto a = 2;
        constexpr auto v = turtle::vector<N>{1, 2, 3};

        constexpr auto u = turtle::vector<N>{2, 4, 6};
        expect(eq(u, v * a));
    };

    test("right division by a scalar") = [] {
        constexpr auto a = 2;
        constexpr auto v = turtle::vector<N>{1, 2, 3};

        constexpr auto u = turtle::vector<N>{0, 1, 1};
        expect(eq(u, v / a));
    };

    test("vector negation") = [] {
        constexpr auto v = turtle::vector<N>{1, 2, 3};

        constexpr auto u = turtle::vector<N>{-1, -2, -3};
        expect(eq(u, -v));
    };

    test("vector add-assign") = [] {
        auto v = turtle::vector<N>{1, 2, 3};
        v += turtle::vector<N>{3, 2, 1};

        constexpr auto u = turtle::vector<N>{4, 4, 4};
        expect(eq(u, v));
    };

    test("vector subtract-assign") = [] {
        auto v = turtle::vector<N>{1, 2, 3};
        v -= turtle::vector<N>{3, 2, 1};

        constexpr auto u = turtle::vector<N>{-2, 0, 2};
        expect(eq(u, v));
    };

    test("scalar multiply-assign") = [] {
        auto v = turtle::vector<N>{1, 2, 3};
        v *= 2;

        constexpr auto u = turtle::vector<N>{2, 4, 6};
        expect(eq(u, v));
    };

    test("scalar divide-assign") = [] {
        auto v = turtle::vector<N>{1, 2, 3};
        v /= 2;

        constexpr auto u = turtle::vector<N>{0, 1, 1};
        expect(eq(u, v));
    };

    test("vector string format") = [] {
        using namespace std::literals::string_view_literals;
        using A = turtle::frame<"A">;
        constexpr auto v = A::vector{1., 2., 3.};

        expect(eq("[A] (1, 2, 3)"sv, fmt::format("{}", v)));
        expect(eq("[A] (1.00, 2.00, 3.00)"sv, fmt::format("{:.2f}", v)));
    };

    test("vector expressed in different frame") = [] {
        using std::numbers::pi;

        using M = turtle::frame<"M">;
        using A = turtle::frame<"A">;

        constexpr auto v = M::vector{1., 2., 3.};
        const auto ori =
            turtle::orientation<M, A>{pi / 2., M::vector{1., 0., 0.}};

        expect(within<1e-9>(A::vector{1., 3., -2.}, v.in<A>(ori)));
    };
}
