#ifndef GAMEWRAPPER_HPP
#define GAMEWRAPPER_HPP

#include <vector>
#include "Node.hpp"
#include "node_utils.hpp"

using namespace std;

class GameWrapper
{
    //Width/height measured in number of trees in direction given.
    const size_t width, height, tree_depth;

    //All game trees valid for the current generation
    vector<vector<shared_ptr<Node>>> trees;

    const shared_ptr<Node> empty_node;

public:
    GameWrapper(const size_t width_, const size_t height_, const size_t tree_depth_) : width(width_),
                                                                                       height(height_),
                                                                                       tree_depth(tree_depth_),
                                                                                       trees(width_, vector<shared_ptr<Node>>(height_, get_empty_node(tree_depth_))),
                                                                                       empty_node(get_empty_node(tree_depth)) {}
    //Compute a single step, either using hashlife or using regular game of life algorhith with hashed quadtrees
    //depending on the Context of the application
    //Because create_updated_center_node returns a node, which lies on the center of 4 nodes of level k,
    //those 4 nodes need to be shifted appropiately to top left direction by (k-1) level node to make sure the new node 
    //is representing the position of the old node
    void update_all()
    {
        auto temp(trees);
        for (size_t i = 0; i < width; i++)
        {
            for (size_t j = 0; j < height; j++)
            {
                temp[i][j] = create_updated_center_node(
                    get_half_level_shifted_top_left(i, j),
                    get_half_level_shifted_top_left(i + 1, j),
                    get_half_level_shifted_top_left(i, j + 1),
                    get_half_level_shifted_top_left(i + 1, j + 1));
            }
        }
        trees = move(temp);
    }

    //Given coordinates x, y, change the value recursively
    void change_at(size_t x, size_t y, bool value)
    {
        auto square_size = 1 << tree_depth;
        trees[x / square_size][y / square_size] = change_at(x % square_size, y % square_size, value, *trees[x / square_size][y / square_size]);
    }

    //Recursive function for changing node at level 0 at coordinates given
    shared_ptr<Node> change_at(size_t x, size_t y, bool value, const Node &node)
    {
        if (node.level == 1)
        {
            if (x == 0 && y == 0)
            {
                return make_shared<Node>(
                    value ? Node::alive_node : Node::dead_node,
                    node.NE,
                    node.SW,
                    node.SE,
                    node.level);
            }
            if (x == 1 && y == 0)
            {
                return make_shared<Node>(
                    node.NW,
                    value ? Node::alive_node : Node::dead_node,
                    node.SW,
                    node.SE,
                    node.level);
            }
            if (x == 0 && y == 1)
            {
                return make_shared<Node>(
                    node.NW,
                    node.NE,
                    value ? Node::alive_node : Node::dead_node,
                    node.SE,
                    node.level);
            }
            if (x == 1 && y == 1)
            {
                return make_shared<Node>(
                    node.NW,
                    node.NE,
                    node.SW,
                    value ? Node::alive_node : Node::dead_node,
                    node.level);
            }
            throw invalid_argument("no suitable cell found " + to_string(x) + ' ' + to_string(y));
        }
        size_t square_size = 1 << (node.level - 1);
        if (x < square_size)
        {
            if (y < square_size)
            {
                return make_shared<Node>(
                    change_at(x % square_size, y % square_size, value, *node.NW),
                    node.NE,
                    node.SW,
                    node.SE,
                    node.level);
            }
            else
            {
                return make_shared<Node>(
                    node.NW,
                    node.NE,
                    change_at(x % square_size, y % square_size, value, *node.SW),
                    node.SE,
                    node.level);
            }
        }
        else
        {
            if (y < square_size)
            {
                return make_shared<Node>(
                    node.NW,
                    change_at(x % square_size, y % square_size, value, *node.NE),
                    node.SW,
                    node.SE,
                    node.level);
            }
            else
            {
                return make_shared<Node>(
                    node.NW,
                    node.NE,
                    node.SW,
                    change_at(x % square_size, y % square_size, value, *node.SE),
                    node.level);
            }
        }
    }

    //Getter function for picking the correct tree from trees and recursively using the  get_at function
    const shared_ptr<Node> &get_at(const size_t x, const size_t y) const
    {
        size_t square_size = 1 << tree_depth;
        const auto &tree = trees[x / square_size][y / square_size];
        return get_at(x % square_size, y % square_size, *tree);
    }

    //Recursive function for getting level 0 node at coordinates given
    const shared_ptr<Node> &get_at(const size_t x, const size_t y, const Node &node) const
    {
        if (node.level == 1)
        {
            if (x == 0 && y == 0)
                return node.NW;
            if (x == 0 && y == 1)
                return node.SW;
            if (x == 1 && y == 0)
                return node.NE;
            if (x == 1 && y == 1)
                return node.SE;

            throw invalid_argument("no suitable cell found " + to_string(x) + ' ' + to_string(y));
        }
        size_t square_size = 1 << (node.level - 1);
        if (x < square_size)
        {
            if (y < square_size)
            {
                return get_at(x % square_size, y % square_size, *node.NW);
            }
            else
            {
                return get_at(x % square_size, y % square_size, *node.SW);
            }
        }
        else
        {
            if (y < square_size)
            {
                return get_at(x % square_size, y % square_size, *node.NE);
            }
            else
            {
                return get_at(x % square_size, y % square_size, *node.SE);
            }
        }
    }
    Node get_half_level_shifted_top_left(const size_t x, const size_t y)
    {
        auto &node_to_shift = x < width && y < height ? trees[x][y] : empty_node;
        auto &left_node = x > 0 && x <= width && y < height ? trees[x - 1][y] : empty_node;
        auto &top_node = y > 0 && x < width && y <= height ? trees[x][y - 1] : empty_node;
        auto &top_left_node = x > 0 && y > 0 && x <= width && y <= height ? trees[x - 1][y - 1] : empty_node;
        return Node(top_left_node->SE, top_node->SW, left_node->NE, node_to_shift->NW, node_to_shift->level);
    }

    //Helper function for getting the level 0 node value
    bool alive_at(const size_t x, const size_t y) const
    {
        return get_at(x, y)->alive;
    }

    //Toggling helper function, toggling level 0 node at coordinates given
    void toggle_at(const size_t x, const size_t y)
    {
        change_at(x, y, !alive_at(x, y));
    }
};

#endif
