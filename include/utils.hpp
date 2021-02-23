#ifndef UTILS_HPP
#define UTILS_HPP

#include <stddef.h>
#include <array>

using namespace std;

size_t calc_hash(size_t level, size_t NW_HASH, size_t NE_hash, size_t SW_hash, size_t SE_hash) noexcept;
bool should_live(size_t value_sum) noexcept;
bool should_reborn(size_t bool_sum) noexcept;
int get_bool_surrounding_sum(int x, int y, const array<array<bool, 4>, 4> &surrounding_bitmap) noexcept;

#endif
