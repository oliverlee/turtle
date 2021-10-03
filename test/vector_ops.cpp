#include "turtle/vector_ops.hpp"

#include "turtle/frame.hpp"
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
        constexpr auto u = turtle::vector<N>{T{1}, T{}, T{}};
        constexpr auto w = turtle::vector<N>{T{1}, T{1}, T{}};

        expect(eq(T{}, dot_product(v, v)));
        expect(eq(T{1}, dot_product(u, u)));
        expect(eq(T{2}, dot_product(w, w)));

        expect(eq(T{}, dot_product(v, u)));
        expect(eq(T{}, dot_product(u, v)));

        expect(eq(T{}, dot_product(v, w)));
        expect(eq(T{}, dot_product(w, v)));

        expect(eq(T{1}, dot_product(u, w)));
        expect(eq(T{1}, dot_product(w, u)));
    } | std::tuple<int, float, double>{};

    test("magnitude") = []<class T>() {
        using N = turtle::frame<"N", T>;
        using V = typename N::vector;

        expect(eq(T{}, magnitude(V{T{}, T{}, T{}})));

        expect(eq(T{1}, magnitude(V{T{1}, T{0}, T{0}})));
        expect(eq(T{1}, magnitude(V{T{0}, T{1}, T{0}})));
        expect(eq(T{1}, magnitude(V{T{0}, T{0}, T{1}})));

        expect(eq(T{2}, magnitude(V{T{2}, T{0}, T{0}})));

        expect(eq(T{5}, magnitude(V{T{3}, T{4}, T{0}})));
    } | std::tuple<float, double>{};

    test("normalized magnitude") = []<class T>() {
        using N = turtle::frame<"N", T>;
        using V = typename N::vector;

        const auto f = [](auto x, auto y, auto z) {
            return magnitude(normalized(V{T(x), T(y), T(z)}));
        };

        for (const auto& v : {f(1, 0, 0), f(1, 1, 0), f(1, 1, 1)}) {
            expect(eq(T{1}, v) or (turtle::util::ulp_diff(T{1}, v) <= 4_i));
        }
    } | std::tuple<float, double>{};
}
