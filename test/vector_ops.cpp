#include "turtle/vector_ops.hpp"

#include "boost/ut.hpp"
#include "turtle/frame.hpp"
#include "turtle/vector.hpp"

#include <tuple>

auto main() -> int
{
    using namespace boost::ut;

    test("DotProduct") = []<class T>() {
        using N = turtle::frame<"N", T>;

        constexpr auto v = turtle::vector<N>{};
        constexpr auto u = turtle::vector<N>{T{1}, T{}, T{}};
        constexpr auto w = turtle::vector<N>{T{1}, T{1}, T{}};

        expect(T{} == dot_product(v, v));
        expect(T{1} == dot_product(u, u));
        expect(T{2} == dot_product(w, w));

        expect(T{} == dot_product(v, u));
        expect(T{} == dot_product(u, v));

        expect(T{} == dot_product(v, w));
        expect(T{} == dot_product(w, v));

        expect(T{1} == dot_product(u, w));
        expect(T{1} == dot_product(w, u));
    } | std::tuple<int, float, double>{};
}
