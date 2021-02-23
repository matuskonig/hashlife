#include <array>
#include <unordered_map>
#include "utils.hpp"
#include "node_utils.hpp"
#include "Context.hpp"

//Given a surrounding sum and flag, whether the flag is alive, return either alive or dead level 0 node
const shared_ptr<Node> &get_leaf_node_by_value(bool is_alive, size_t surrounding_sum)
{
    if (is_alive)
    {
        return should_live(surrounding_sum) ? Node::alive_node : Node::dead_node;
    }
    return should_reborn(surrounding_sum) ? Node::alive_node : Node::dead_node;
}

//Considering 4 level 1 nodes, creates a surrounding bitmap of values at appropriate positions
array<array<bool, 4>, 4> create_surrounding_bitmap(const Node &NW_, const Node &NE_, const Node &SW_, const Node &SE_) noexcept
{
    array<array<bool, 4>, 4> bitmap = {{{
                                            NW_.NW->alive,
                                            NW_.NE->alive,
                                            NE_.NW->alive,
                                            NE_.NE->alive,
                                        },
                                        {
                                            NW_.SW->alive,
                                            NW_.SE->alive,
                                            NE_.SW->alive,
                                            NE_.SE->alive,
                                        },
                                        {
                                            SW_.NW->alive,
                                            SW_.NE->alive,
                                            SE_.NW->alive,
                                            SE_.NE->alive,
                                        },
                                        {
                                            SW_.SW->alive,
                                            SW_.SE->alive,
                                            SE_.SW->alive,
                                            SE_.SE->alive,
                                        }}};
    return bitmap;
}

//Cache for regular GOL hashed quartree algorithm
unordered_map<node_update_args, shared_ptr<Node>> cache_regular;

//Cache for fast hashlife algorithm, different from regular cache, to prevent cache poisoning
unordered_map<node_update_args, shared_ptr<Node>> cache_hashlife;

//Given 2 nodes of level k in vertical position, get node lying on the center of those nodes of level k-1
Node get_vertical_centered_sub_node_regular(const Node &n, const Node &s) noexcept
{
    return Node(n.SW->SE, n.SE->SW, s.NW->NE, s.NE->NW, n.level - 1);
}

//Given 2 nodes of level k in horizontal position, get node lying on the center of those nodes of level k-1
Node get_horizontal_centered_sub_node_regular(const Node &w, const Node &e) noexcept
{
    return Node(w.NE->SE, e.NW->SW, w.SE->NE, e.SW->NW, w.level - 1);
}

//Given 1 node of level k, get node lying on the center of those nodes of level k-1
Node get_centered_subnode_regular(const Node &node) noexcept
{
    return Node(node.NW->SE, node.NE->SW, node.SW->NE, node.SE->NW, node.level - 1);
}

//Given 4 nodes of level k, get node lying on the center of those nodes of level k-1
Node get_centered_subsubnode_regular(const Node &NW, const Node &NE, const Node &SW, const Node &SE) noexcept
{
    return Node(NW.SE->SE, NE.SW->SW, SW.NE->NE, SE.NW->NW, NW.level - 1);
}

//Given 2 nodes of level k in vertical position, get node lying on the center of those nodes of level k-1
Node get_vertical_centered_sub_node_hashlife(const Node &n, const Node &s) noexcept
{
    return *create_updated_center_node(*n.SW, *n.SE, *s.NW, *s.NE);
}

//Given 2 nodes of level k in horizontal position, get node lying on the center of those nodes of level k-1
Node get_horizontal_centered_sub_node_hashlife(const Node &w, const Node &e) noexcept
{
    return *create_updated_center_node(*w.NE, *e.NW, *w.SE, *e.SW);
}

//Given 1 node of level k, get node lying on the center of those nodes of level k-1
Node get_centered_subnode_hashlife(const Node &node) noexcept
{
    return *create_updated_center_node(*node.NW, *node.NE, *node.SW, *node.SE);
}

//Given 4 nodes of level k, get node lying on the center of those nodes of level k-1
Node get_centered_subsubnode_hashlife(const Node &NW, const Node &NE, const Node &SW, const Node &SE) noexcept
{
    return *create_updated_center_node(*NW.SE, *NE.SW, *SW.NE, *SE.NW);
}

//Main function for node updating.
//Given 4 nodes of level k, return updated node of level k lying on the center of those nodes.
shared_ptr<Node> create_updated_center_node(const Node &NW_, const Node &NE_, const Node &SW_, const Node &SE_) noexcept
{
    auto &context = Context::instance();
    auto args_tuple = node_update_args(NW_, NE_, SW_, SE_);

    //Correct cache depending on the algorithm used, preventing cache poisoning
    auto &cache = context.hash_life ? cache_hashlife : cache_regular;
    auto hashmap_iterator = cache.find(args_tuple);

    //Returning cached result
    if (hashmap_iterator != cache.end())
    {
        return hashmap_iterator->second;
    }

    //Base case for recursion, bruteforce approach, where alive_state of center is calculated explicitly from 4x4 square of values
    if (NW_.level == 1)
    {
        auto bitmap = create_surrounding_bitmap(NW_, NE_, SW_, SE_);
        auto &NW = get_leaf_node_by_value(NW_.SE->alive, get_bool_surrounding_sum(1, 1, bitmap));
        auto &NE = get_leaf_node_by_value(NE_.SW->alive, get_bool_surrounding_sum(2, 1, bitmap));
        auto &SW = get_leaf_node_by_value(SW_.NE->alive, get_bool_surrounding_sum(1, 2, bitmap));
        auto &SE = get_leaf_node_by_value(SE_.NW->alive, get_bool_surrounding_sum(2, 2, bitmap));
        auto result = make_shared<Node>(NW, NE, SW, SE, 1, 0);
        return cache[args_tuple] = result;
    }

    //Choosing of correct functions based on application context
    auto get_centered_subnode = context.hash_life ? get_centered_subnode_hashlife : get_centered_subnode_regular;
    auto get_horizontal_centered_sub_node = context.hash_life ? get_horizontal_centered_sub_node_hashlife : get_horizontal_centered_sub_node_regular;
    auto get_vertical_centered_sub_node = context.hash_life ? get_vertical_centered_sub_node_hashlife : get_vertical_centered_sub_node_regular;
    auto get_centered_subsubnode = context.hash_life ? get_centered_subsubnode_hashlife : get_centered_subsubnode_regular;

    //Considering node at level k, get 9 k-1 nodes lying on the centre
    //nXY
    const auto n00 = get_centered_subnode(NW_);
    const auto n10 = get_horizontal_centered_sub_node(NW_, NE_);
    const auto n20 = get_centered_subnode(NE_);
    const auto n01 = get_vertical_centered_sub_node(NW_, SW_);
    const auto n11 = get_centered_subsubnode(NW_, NE_, SW_, SE_);
    const auto n21 = get_vertical_centered_sub_node(NE_, SE_);
    const auto n02 = get_centered_subnode(SW_);
    const auto n12 = get_horizontal_centered_sub_node(SW_, SE_);
    const auto n22 = get_centered_subnode(SE_);

    //Recursively compute the updated node by combining 9 nodes
    auto result = make_shared<Node>(
        create_updated_center_node(n00, n10, n01, n11),
        create_updated_center_node(n10, n20, n11, n21),
        create_updated_center_node(n01, n11, n02, n12),
        create_updated_center_node(n11, n21, n12, n22),
        NW_.level);

    return cache[args_tuple] = result;
}

//Recursive function for generating empty nodes
shared_ptr<Node> get_empty_node(size_t level) noexcept
{
    if (!level)
        return Node::dead_node;

    auto one_level_below = get_empty_node(level - 1);
    return make_shared<Node>(
        one_level_below,
        one_level_below,
        one_level_below,
        one_level_below,
        level);
}