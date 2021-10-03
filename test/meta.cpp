#include "turtle/meta.hpp"

#include "boost/ut.hpp"

#include <type_traits>

namespace metal {

// Only define equality so we get a decent error message on failure
template <class... Args>
constexpr auto operator==(list<Args...>, list<Args...>) noexcept -> bool
{
    return true;
}

}  // namespace metal

auto main() -> int
{
    using namespace boost::ut;

    namespace meta = turtle::meta;

    struct A {};
    struct B {};
    struct C {};
    struct D {};
    struct E {};
    struct F {};
    struct G {};

    test("flatten") = [] {
        static_assert(metal::list<A, B>{} == meta::flatten<meta::tree<A, B>>{});
        static_assert(metal::list<A, B, D, E, C>{} ==
                      meta::flatten<meta::tree<A, meta::tree<B, D, E>, C>>{});
    };

    test("type tree node path") = [] {
        using Tree2 = meta::tree<A, meta::tree<B, D, E>, C>;

        static_assert(metal::list<A>{} == meta::path_to<Tree2, A>{});
        static_assert(metal::list<A, C>{} == meta::path_to<Tree2, C>{});
        static_assert(metal::list<A, B>{} == meta::path_to<Tree2, B>{});
        static_assert(metal::list<A, B, D>{} == meta::path_to<Tree2, D>{});
        static_assert(metal::list<A, B, E>{} == meta::path_to<Tree2, E>{});
        static_assert(metal::list<>{} == meta::path_to<Tree2, F>{});
    };

    test("check if tree contains node") = [] {
        using Tree2 = meta::tree<A, meta::tree<B, D, E>, C>;

        static_assert(Tree2::contains_v<A>);
        static_assert(Tree2::contains_v<B>);
        static_assert(Tree2::contains_v<C>);
        static_assert(Tree2::contains_v<D>);
        static_assert(Tree2::contains_v<E>);

        static_assert(not Tree2::contains_v<F>);
    };

    test("add branches to a tree") = [] {
        static_assert(
            meta::tree<A, B>{} == meta::insert<meta::tree<A>, A, B>{});

        static_assert(
            meta::tree<A, B, C>{} == meta::insert<meta::tree<A, B>, A, C>{});

        static_assert(meta::tree<A, meta::tree<B, D>, C>{} ==
                      meta::insert<meta::tree<A, B, C>, B, D>{});

        static_assert(meta::tree<A, meta::tree<B, meta::tree<D, E>>, C>{} ==
                      meta::insert<meta::tree<A, meta::tree<B, D>, C>, D, E>{});

        static_assert(meta::tree<A, meta::tree<B, D, E>, C>{} ==
                      meta::insert<meta::tree<A, meta::tree<B, D>, C>, B, E>{});

        static_assert(
            meta::tree<A, meta::tree<B, D, meta::tree<E, F>>, C>{} ==
            meta::insert<meta::tree<A, meta::tree<B, D, E>, C>, E, F>{});

        static_assert(
            meta::tree<A, meta::tree<B, D, E>, meta::tree<C, G>>{} ==
            meta::insert<meta::tree<A, meta::tree<B, D, E>, C>, C, G>{});
    };

    test("can't add branches to tree if frame is duplicated") = [] {
        using T = meta::tree<A, B>;

        auto const new_tree = []<class From, class To>(From, To)
            -> T::template add_branch_t<From, To> { return {}; };

        static_assert(not std::is_invocable_v<decltype(new_tree), A, B>);

        static_assert(std::is_invocable_v<decltype(new_tree), B, C>);
    };
}
