#ifndef CONSTANTS_CPP
#define CONSTANTS_CPP

constexpr auto TREE_DEPTH = 5;
constexpr auto TREE_SIDE_SIZE = 1 << TREE_DEPTH;

//Number of trees in a game (width)
constexpr auto GAME_PLAN_TREE_WIDTH = 4;

//Number of trees in a game (height)
constexpr auto GAME_PLAN_TREE_HEIGHT = 3;
constexpr auto ONE_POINT_PIXEL_SIZE = 10;

//Number of level 0 nodes (width)
constexpr auto BIT_WIDTH = TREE_SIDE_SIZE * GAME_PLAN_TREE_WIDTH;
constexpr auto WINDOW_WIDTH = BIT_WIDTH * ONE_POINT_PIXEL_SIZE;

//Number of level 0 nodes (height)
constexpr auto BIT_HEIGHT = TREE_SIDE_SIZE * GAME_PLAN_TREE_HEIGHT;
constexpr auto WINDOW_HEIGHT = BIT_HEIGHT * ONE_POINT_PIXEL_SIZE;

constexpr auto LIVE_TRESHOLD_MIN = 2;
constexpr auto LIVE_TRESHOLD_MAX = 3;
constexpr auto REBORN_TRESHOLD = 3;

constexpr auto VALUE_PRIME_1 = 2;
constexpr auto VALUE_PRIME_2 = 7;

constexpr auto SUBNODE_PRIME_1 = 1073741827;
constexpr auto SUBNODE_PRIME_2 = 8589934609;
constexpr auto SUBNODE_PRIME_3 = 68719476767;
constexpr auto SUBNODE_PRIME_4 = 274877906951;

#endif