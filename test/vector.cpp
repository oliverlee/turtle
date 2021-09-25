#include "turtle/vector.hpp"

#include "boost/ut.hpp"

#include <algorithm>

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
}
