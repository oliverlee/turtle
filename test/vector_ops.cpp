#include "turtle/vector_ops.hpp"

#include "turtle/frame.hpp"
#include "turtle/quaternion.hpp"
#include "turtle/util/ulp_diff.hpp"
#include "turtle/vector.hpp"

#include "boost/ut.hpp"

#include <bit>
#include <tuple>

auto main() -> int
{
    using namespace boost::ut;

    test("dot product") = []<class T>() {
        using N = turtle::frame<"N", T>;

        constexpr auto v = turtle::vector<N>{};
        constexpr auto u = turtle::vector<N>{1, 0, 0};
        constexpr auto w = turtle::vector<N>{1, 1, 0};

        expect(eq(T{}, dot_product(v, v)));
        expect(eq(T{1}, dot_product(u, u)));
        expect(eq(T{2}, dot_product(w, w)));

        expect(eq(T{}, dot_product(v, u)));
        expect(eq(T{}, dot_product(u, v)));

        expect(eq(T{}, dot_product(v, w)));
        expect(eq(T{}, dot_product(w, v)));

        expect(eq(T{1}, dot_product(u, w)));
        expect(eq(T{1}, dot_product(w, u)));
    } | std::tuple<float, double>{};

    test("cross product") = []<class T>() {
        using N = turtle::frame<"N", T>;
        using V = typename N::vector;

        constexpr auto v = V{};
        constexpr auto u = V{1, 0, 0};
        constexpr auto w = V{0, 1, 0};

        expect(eq(V{}, cross_product(v, v)));
        expect(eq(V{}, cross_product(v, u)));
        expect(eq(V{}, cross_product(v, w)));

        expect(eq(V{}, cross_product(u, u)));
        expect(eq(V{}, cross_product(w, w)));

        expect(eq(V{0, 0, 1}, cross_product(u, w)));
        expect(eq(V{0, 0, -1}, cross_product(w, u)));
    } | std::tuple<float, double>{};

    test("norm") = []<class T>() {
        using N = turtle::frame<"N", T>;
        using V = typename N::vector;

        expect(eq(T{}, norm(V{})));

        expect(eq(T{1}, norm(V{1, 0, 0})));
        expect(eq(T{1}, norm(V{0, 1, 0})));
        expect(eq(T{1}, norm(V{0, 0, 1})));

        expect(eq(T{2}, norm(V{2, 0, 0})));

        expect(eq(T{5}, norm(V{3, 4, 0})));
    } | std::tuple<float, double>{};

    test("normalized magnitude") = []<class T>() {
        using N = turtle::frame<"N", T>;
        using V = typename N::vector;

        const auto f = [](auto x, auto y, auto z) {
            return norm(normalized(V{T(x), T(y), T(z)}));
        };

        for (const auto& v : {f(1, 0, 0), f(1, 1, 0), f(1, 1, 1)}) {
            expect(eq(T{1}, v) or (turtle::util::ulp_diff(T{1}, v) <= 4_i));
        }
    } | std::tuple<float, double>{};
}
