#include "turtle/meta.hpp"

#include "turtle/m2.hpp"

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
    using turtle::meta::are_unique;
    using turtle::meta::are_unique_v;
    using turtle::meta::flatten_t;
    using turtle::meta::list_size_v;
    using turtle::meta::path_t;
    using turtle::meta::tree_depth_v;
    using turtle::meta::type_list;
    using turtle::meta::type_tree;

    namespace m2 = turtle::m2;

    struct A {};
    struct B {};
    struct C {};
    struct D {};
    struct E {};
    struct F {};
    struct G {};

    test("flatten") = [] {
        static_assert(std::is_same_v<type_list<A, A>,
                                     flatten_t<type_list, type_tree<A, A>>>);

        static_assert(
            std::is_same_v<
                type_list<A, B, D, E, C>,
                flatten_t<type_list, type_tree<A, type_tree<B, D, E>, C>>>);

        static_assert(metal::list<A, B>{} == m2::flatten<m2::tree<A, B>>{});
        static_assert(metal::list<A, B, D, E, C>{} ==
                      m2::flatten<m2::tree<A, m2::tree<B, D, E>, C>>{});
    };

    test("unique types") = [] {
        static_assert(are_unique_v<>);
        static_assert(are_unique_v<A, B>);
        static_assert(not are_unique_v<A, A>);
    };

    test("type list size") = [] {
        expect(constant<0_i == list_size_v<type_list<>>>);
        expect(constant<1_i == list_size_v<type_list<A>>>);
        expect(constant<2_i == list_size_v<type_list<A, A>>>);
        expect(constant<2_i == list_size_v<type_list<A, B>>>);
    };

    test("type tree node path") = [] {
        using Tree = type_tree<A, type_tree<B, D, E>, C>;

        static_assert(std::is_same_v<type_list<A>, path_t<A, Tree>>);
        static_assert(std::is_same_v<type_list<A, B>, path_t<B, Tree>>);
        static_assert(std::is_same_v<type_list<A, C>, path_t<C, Tree>>);
        static_assert(std::is_same_v<type_list<A, B, D>, path_t<D, Tree>>);
        static_assert(std::is_same_v<type_list<A, B, E>, path_t<E, Tree>>);
        static_assert(std::is_same_v<type_list<>, path_t<F, Tree>>);
    };

    test("type tree depth") = [] {
        expect(constant<1_i == tree_depth_v<type_tree<A>>>);

        expect(constant<2_i == tree_depth_v<type_tree<A, B>>>);
        expect(constant<2_i == tree_depth_v<type_tree<A, B, C>>>);
        expect(constant<2_i == tree_depth_v<type_tree<A, type_tree<B>>>>);
        expect(constant<2_i == tree_depth_v<type_tree<A, type_tree<B>, C>>>);

        expect(constant<3_i == tree_depth_v<type_tree<A, type_tree<B, C>, D>>>);
        expect(constant<
               3_i ==
               tree_depth_v<type_tree<A, type_tree<B, C>, type_tree<D>>>>);
        expect(constant<
               3_i ==
               tree_depth_v<type_tree<A, type_tree<B, C>, type_tree<D, E>>>>);
        expect(constant<
               3_i == tree_depth_v<
                          type_tree<A, type_tree<B, C, F>, type_tree<D, E>>>>);
    };

    test("duplicate nodes in tree are detectable") = [] {
        static_assert(flatten_t<are_unique, type_tree<A>>::value);
        static_assert(flatten_t<are_unique, type_tree<A, B, C>>::value);
        static_assert(
            flatten_t<are_unique, type_tree<A, type_tree<B, C, D>, E>>::value);

        static_assert(not flatten_t<are_unique, type_tree<A, A>>::value);
        static_assert(not flatten_t<are_unique, type_tree<A, B, C, C>>::value);
        static_assert(
            not flatten_t<are_unique,
                          type_tree<A, type_tree<B, C, D, A>, E>>::value);
        static_assert(
            not flatten_t<are_unique,
                          type_tree<A, type_tree<B, C, D>, E, C>>::value);
    };

    test("check if tree contains node") = [] {
        using Tree = type_tree<A, type_tree<B, D, E>, C>;

        static_assert(Tree::contains_v<A>);
        static_assert(Tree::contains_v<B>);
        static_assert(Tree::contains_v<C>);
        static_assert(Tree::contains_v<D>);
        static_assert(Tree::contains_v<E>);

        static_assert(not Tree::contains_v<F>);
    };

    test("add branches to a tree") = [] {
        using T1 = type_tree<A>::add_branch_t<A, B>;
        static_assert(std::is_same_v<type_tree<A, B>, T1>);

        using T2 = T1::add_branch_t<A, C>;
        static_assert(std::is_same_v<type_tree<A, B, C>, T2>);

        using T3 = T2::add_branch_t<B, D>;
        static_assert(std::is_same_v<type_tree<A, type_tree<B, D>, C>, T3>);

        using T4 = T3::add_branch_t<B, E>;
        static_assert(std::is_same_v<type_tree<A, type_tree<B, D, E>, C>, T4>);

        using T5 = T4::add_branch_t<E, F>;
        static_assert(
            std::is_same_v<type_tree<A, type_tree<B, D, type_tree<E, F>>, C>,
                           T5>);

        using T6 = T5::add_branch_t<C, G>;
        static_assert(
            std::is_same_v<
                type_tree<A, type_tree<B, D, type_tree<E, F>>, type_tree<C, G>>,
                T6>);
    };
}
