#ifndef NODE_UTILS_HPP
#define NODE_UTILS_HPP

#include <memory>
#include "Node.hpp"

const shared_ptr<Node> &get_leaf_node_by_value(bool is_alive, size_t surrounding_sum);

array<array<bool, 4>, 4> create_surrounding_bitmap(const Node &NW_, const Node &NE_, const Node &SW_, const Node &SE_) noexcept;

//Tuple representing 4 nodes, which are considered as argument of update function
using node_update_args = tuple<Node, Node, Node, Node>;
namespace std
{
    //Hash overload, allowing the tuple to be hashed effectively
    template <>
    struct hash<node_update_args>
    {
        size_t operator()(node_update_args const &args) const
        {
            auto [NW, NE, SW, SE] = args;
            return calc_hash(NW.level + 1, NW.hash, NE.hash, SW.hash, SE.hash);
        }
    };
}

Node get_vertical_centered_sub_node_regular(const Node &n, const Node &s) noexcept;
Node get_horizontal_centered_sub_node_regular(const Node &w, const Node &e) noexcept;
Node get_centered_subnode_regular(const Node &node) noexcept;
Node get_centered_subsubnode_regular(const Node &NW, const Node &NE, const Node &SW, const Node &SE) noexcept;

Node get_vertical_centered_sub_node_hashlife(const Node &n, const Node &s) noexcept;
Node get_horizontal_centered_sub_node_hashlife(const Node &w, const Node &e) noexcept;
Node get_centered_subnode_hashlife(const Node &node) noexcept;
Node get_centered_subsubnode_hashlife(const Node &NW, const Node &NE, const Node &SW, const Node &SE) noexcept;

shared_ptr<Node> create_updated_center_node(const Node &NW_, const Node &NE_, const Node &SW_, const Node &SE_) noexcept;
shared_ptr<Node> get_empty_node(size_t level) noexcept;

#endif
