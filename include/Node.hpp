#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include "constants.hpp"
#include "utils.hpp"

using namespace std;

//A base quadtree node representing the state of the game
struct Node
{
    inline static auto const alive_node = make_shared<Node>(nullptr, nullptr, nullptr, nullptr, 0, true);
    inline static auto const dead_node = make_shared<Node>(nullptr, nullptr, nullptr, nullptr, 0, false);
    const shared_ptr<Node> NW;
    const shared_ptr<Node> NE;
    const shared_ptr<Node> SW;
    const shared_ptr<Node> SE;

    //A level of the node in the quadtree structure, level 0 cells represents a single cell,
    //node of level k represents a square of size 2^k*2^k
    const size_t level = 0;

    const bool alive = false;

    //Hash is precalculated, so it can be used later in hashing
    size_t hash = 0;

    Node() = default;
    Node(const shared_ptr<Node> &NW_, const shared_ptr<Node> &NE_, const shared_ptr<Node> &SW_, const shared_ptr<Node> &SE_, size_t level_, bool alive_) : NW(NW_), //Mozno odstranit move, idk
                                                                                                                                                           NE(NE_),
                                                                                                                                                           SW(SW_),
                                                                                                                                                           SE(SE_),
                                                                                                                                                           level(level_),
                                                                                                                                                           alive(alive_)
    {
        if (!NW_) //true for 0/1 leaves
        {
            hash = alive ? VALUE_PRIME_1 : VALUE_PRIME_2;
        }
        else
        {
            //hash should be mod 2^64 by default
            hash = calc_hash(level, NW->hash, NE->hash, SW->hash, SE->hash);
        }
    }
    //Constructor mostly used in initialization of level 1 and above nodes
    Node(const shared_ptr<Node> &NW_, const shared_ptr<Node> &NE_, const shared_ptr<Node> &SW_, const shared_ptr<Node> &SE_, size_t level_)
        : Node(NW_, NE_, SW_, SE_, level_, false) {}
  
    bool operator==(const Node &second) const
    {
        if (level == 0)
            return alive == second.alive;
        return level == second.level && *NW == *second.NW && *NE == *second.NE && *SW == *second.SW && *SE == *second.SE;
    }
};

#endif
