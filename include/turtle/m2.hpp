#pragma once

#include "metal.hpp"

/// @brief Temporary namespace until metaprogramming switches over to use metal
namespace turtle::m2 {

/// @brief A metatype where the first is a root and the rest are branches
template <class Root, class... Branches>
struct tree {
    using type = tree;

    using root = Root;
    using branches = metal::list<Branches...>;
};

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

}  // namespace turtle::m2
