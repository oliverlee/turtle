#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace turtle::meta {

/// @brief Determines of a type is a specialization of a template
/// @see wg21.link/p2098
///@{
template <class T, template <class...> class Primary>
struct is_specialization_of : std::false_type {};

template <template <class...> class Primary, class... Args>
struct is_specialization_of<Primary<Args...>, Primary> : std::true_type {};
///@}

/// @brief Returns the type with the maximum value
///@{
template <class...>
struct max;

template <class V1>
struct max<V1> : V1 {};

template <class V1, class V2, class... Vn>
struct max<V1, V2, Vn...>
    : std::conditional_t<(V1::value > V2::value), max<V1, Vn...>, max<V2, Vn...>> {};

template <class... Vn>
inline constexpr auto max_v = max<Vn...>::value;
///@}

/// @brief Obtains the number of elements in a type list
///@{
template <class TypeList>
struct list_size;

template <template <class...> class List, class... Types>
struct list_size<List<Types...>> {
    static constexpr std::size_t value = sizeof...(Types);
};

template <class TypeList>
inline constexpr auto list_size_v = list_size<TypeList>::value;
///@}

/// @brief Concatenates multiple type lists
///@{
template <class List1, class... Lists>
struct concat;

template <template <class...> class List, class... Types1, class... Types2>
struct concat<List<Types1...>, List<Types2...>> {
    using type = List<Types1..., Types2...>;
};

template <class List1, class List2, class... Lists>
struct concat<List1, List2, Lists...> : concat<typename concat<List1, List2>::type, Lists...> {};

template <class List1>
struct concat<List1> {
    using type = List1;
};

template <class... Lists>
using concat_t = typename concat<Lists...>::type;
///@}

/// @brief Flatten a type tree to a type list
///@{
template <class List, class Tree>
struct flatten;

// template <template <class...> class List, template <class...> class Tree, class... Nodes>
template <template <class...> class List, class Tree>
using flatten_t = typename flatten<List<>, Tree>::type;

template <template <class...> class List,
          class... Rs,
          template <class...>
          class Tree,
          class Parent,
          class... Children>
struct flatten<List<Rs...>, Tree<Parent, Children...>>
    : concat<List<Rs..., Parent>, flatten_t<List, Children>...> {};

template <template <class...> class List, class... Rs, class Leaf>
struct flatten<List<Rs...>, Leaf> {
    using type = List<Rs..., Leaf>;
};
///@}

/// @brief A container for types
template <class... Types>
struct type_list {
    using type = type_list;
};
template <class... Types>
using type_list_t = typename type_list<Types...>::type;

/// @brief Associates an index with a type
template <std::size_t I, class T>
struct indexed_type : std::type_identity<T> {};

/// @brief A class that inherits from all provided types
template <class... Types>
struct inherit : Types... {};

/// @brief Determine if all types are unique
///
///@{
namespace detail {
template <class, class, class = void>
struct are_unique : std::false_type {};

template <std::size_t... Is, class... Ts>
struct are_unique<std::index_sequence<Is...>,
                  type_list<Ts...>,
                  std::void_t<decltype(

                      static_cast<std::type_identity<Ts>>(inherit<indexed_type<Is, Ts>...>{})

                          )...>> : std::true_type {};
}  // namespace detail

template <class... Types>
struct are_unique : detail::are_unique<std::index_sequence_for<Types...>, type_list<Types...>> {};

template <class... Types>
inline constexpr auto are_unique_v = are_unique<Types...>::value;
///@}

/// @brief A container for types where the first is a parent and the rest are
///     children
template <class Parent, class... Children>
struct type_tree {
    static_assert(sizeof...(Children) != 0);
    using type = type_tree;

    using parent_type = Parent;
    using children_types = type_list<Children...>;
};

template <class Parent, class... Children>
using type_tree_t = typename type_tree<Parent, Children...>::type;

/// @brief Returns the depth of a type_tree type
///@{
template <class T>
struct tree_depth {
    static constexpr std::size_t value = 1;
};
template <class Parent>
struct tree_depth<type_tree<Parent>> {
    static constexpr std::size_t value = 1;
};
template <class Parent, class Child1, class... Children>
struct tree_depth<type_tree<Parent, Child1, Children...>> {
    static constexpr std::size_t value = 1 + max_v<tree_depth<Child1>, tree_depth<Children>...>;
};
template <class T>
inline constexpr auto tree_depth_v = tree_depth<T>::value;
///@}

/// @brief Obtains the path from the tree root to a node
///@{
template <class Node, class Tree, class = void>
struct path {
    using type = type_list<>;
};

template <class Node, class Tree>
using path_t = typename path<Node, Tree>::type;


template <class Node, class... Children>
struct path<Node, type_tree<Node, Children...>> {
    using type = type_list<Node>;
};

template <class Node, class Parent, class... Children>
struct path<Node,
            type_tree<Parent, Children...>,
            std::enable_if_t<std::disjunction_v<std::is_same<Node, Children>...>>> {
    using type = type_list<Parent, Node>;
};

template <class Node, class Parent, class... Children>
struct path<Node,
            type_tree<Parent, Children...>,
            std::enable_if_t<not std::is_same_v<Node, Parent> &&
                             std::disjunction_v<is_specialization_of<Children, type_tree>...> &&
                             not std::disjunction_v<std::is_same<Node, Children>...>>> {
    using subtree_path = concat_t<path_t<Node, Children>...>;
    using type = std::conditional_t<list_size_v<subtree_path> == 0,
                                    type_list<>,
                                    concat_t<type_list<Parent>, subtree_path>>;
};
///@}

}  // namespace turtle::meta
