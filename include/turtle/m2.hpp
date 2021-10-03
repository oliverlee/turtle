#pragma once

#include "metal.hpp"

#include <type_traits>

/// @brief Temporary namespace until metaprogramming switches over to use metal
namespace turtle::m2 {

template <class R, class... Bs>
struct tree;

/// @brief Helper for a recursive flatten metafunction
template <class Tree>
struct flatten_helper;

/// @brief Flatten a tree into a metal::list
template <class Tree>
using flatten =
    metal::join<metal::list<typename Tree::root>,
                metal::flatten<metal::transform<metal::lazy<flatten_helper>,
                                                typename Tree::branches>>>;

template <class Leaf>
struct flatten_helper {
    using type = metal::list<Leaf>;
};
template <class R, class... Bs>
struct flatten_helper<tree<R, Bs...>> {
    using type = flatten<tree<R, Bs...>>;
};

/// @brief Helper for a recursive path_to metafunction
template <class Tree, class Node>
struct path_to_helper;

/// @brief Obtain the path from root to node
/// @return List from root to node if node is in the tree, otherwise an empty
/// list.
template <class Tree, class Node>
using path_to = metal::if_<
    metal::contains<flatten<Tree>, Node>,
    metal::join<metal::list<typename Tree::root>,
                metal::if_<metal::same<typename Tree::root, Node>,
                           metal::list<>,
                           metal::flatten<metal::transform<
                               metal::bind<metal::lazy<path_to_helper>,
                                           metal::_1,
                                           metal::always<Node>>,
                               typename Tree::branches>>>>,
    metal::list<>>;

template <class R, class... Bs, class N>
struct path_to_helper<tree<R, Bs...>, N> {
    using type = path_to<tree<R, Bs...>, N>;
};
template <class Leaf, class N>
struct path_to_helper {
    using type =
        metal::if_<metal::same<Leaf, N>, metal::list<N>, metal::list<>>;
};

/// @brief Helper for a recursive insert metafunction
template <class Tree, class From, class To>
struct insert_helper;

/// @brief Insert a branch into a tree
/// @return Tree with new branch inserted on success, the same tree on failure.
template <class Tree, class From, class To>
using insert = metal::if_<
    metal::and_<metal::contains<flatten<Tree>, From>,
                metal::not_<metal::contains<flatten<Tree>, To>>>,
    metal::apply<
        metal::as_lambda<Tree>,
        metal::if_<metal::same<From, typename Tree::root>,
                   metal::append<metal::as_list<Tree>, To>,
                   metal::transform<metal::bind<metal::lazy<insert_helper>,
                                                metal::_1,
                                                metal::always<From>,
                                                metal::always<To>>,
                                    metal::as_list<Tree>>>>,
    Tree>;

template <class R, class... Bs, class From, class To>
struct insert_helper<tree<R, Bs...>, From, To> {
    using type = insert<tree<R, Bs...>, From, To>;
};
template <class Leaf, class From, class To>
struct insert_helper {
    using type = metal::if_<metal::same<Leaf, From>, tree<Leaf, To>, Leaf>;
};

/// @brief A metatype where the first element is the root and the rest are
/// branches
template <class Root, class... Branches>
struct tree {
    using type = tree;

    using root = Root;
    using branches = metal::list<Branches...>;

    template <class Node>
    using contains =
        std::is_same<metal::true_, metal::contains<flatten<type>, Node>>;

    template <class Node>
    static constexpr auto contains_v = contains<Node>::value;

    template <class Node>
    using path_to_t = std::enable_if_t<contains_v<Node>, path_to<tree, Node>>;

    template <class From, class To>
    using add_branch_t =
        std::enable_if_t<contains_v<From> && not contains_v<To>,
                         insert<tree, From, To>>;

    friend constexpr auto operator==(tree, tree) noexcept -> bool
    {
        return true;
    }
};

}  // namespace turtle::m2
