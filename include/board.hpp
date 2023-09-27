#ifndef BOARD_HPP
#define BOARD_HPP

constexpr int BOARD_SIZE_X = 7;
constexpr int BOARD_SIZE_Y = 6;

#include <vector>
#include <iostream>

#include "xxhash.h"

enum token : char
{
    empty,
    red,
    yellow
};

class Board
{
private:
    token grid[BOARD_SIZE_X][BOARD_SIZE_Y] = {{empty}};

public:
    token operator()(int x, int y) const;

    const std::vector<int> getLegalMoves() const;

    bool play(token color, int x);

    token checkVictory() const;

    void clear() const;

    XXH128_hash_t getHash() const;
};

std::ostream &operator<<(std::ostream &os, const Board &b);

#endif