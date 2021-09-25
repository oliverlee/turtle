#include "boost/ut.hpp"
#include "turtle/frame.hpp"

using N = turtle::frame<"N">;

void test_frame_trait()
{
    static_assert(std::is_frame_v<N>);
    static_assert(std::is_frame_v<turtle::frame<"A">>);

    static_assert(not std::is_frame_v<int>);
    static_assert(not std::is_frame_v<turtle::vector<N, int>>);

    static_assert(std::is_same_v<N, turtle::frame<"N">>);
    static_assert(not std::is_same_v<N, turtle::frame<"A">>);
};

auto main() -> int
{
    using namespace boost::ut;

    test("Construct") = [] {
        constexpr auto n = turtle::frame<"N">;
        (void)n;
    };

    test("Name") = [] {
        constexpr auto n = turtle::frame<"N">;

        expect("N" = n.name);
    };
}
