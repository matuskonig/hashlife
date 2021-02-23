#include "utils.hpp"
#include "constants.hpp"

using namespace std;

//Helper function for calculating hash of level k >= 1 nodes
size_t calc_hash(size_t level, size_t NW_HASH, size_t NE_hash, size_t SW_hash, size_t SE_hash) noexcept
{
    return level + 2 + NW_HASH * SUBNODE_PRIME_1 + NE_hash * SUBNODE_PRIME_2 + SW_hash * SUBNODE_PRIME_3 + SE_hash * SUBNODE_PRIME_4;
}

//Considering a bool sum of live cell, returns whether the cell should survive into next generation
bool should_live(size_t bool_sum) noexcept
{
    return bool_sum >= LIVE_TRESHOLD_MIN && bool_sum <= LIVE_TRESHOLD_MAX;
}

//Considering a bool sum of dead cell, returns whether the cell should be reborn into next generation
bool should_reborn(size_t value_sum) noexcept
{
    return value_sum == REBORN_TRESHOLD;
}

//Given a surrounding of size 4x4 and x,y in <1,2> range, calculates the sum of all values in distance of size one excluding the point [x,y] 
int get_bool_surrounding_sum(int x, int y, const array<array<bool, 4>, 4> &surrounding_bitmap) noexcept
{
    int accumulator = 0;
    for (auto i = y - 1; i <= y + 1; i++)
    {
        for (auto j = x - 1; j <= x + 1; j++)
        {
            if (surrounding_bitmap[i][j])
                accumulator++;
        }
    }
    return accumulator - surrounding_bitmap[y][x];
}