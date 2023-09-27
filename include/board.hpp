#ifndef BOARD_HPP
#define BOARD_HPP

constexpr int BOARD_SIZE_X = 7;
constexpr int BOARD_SIZE_Y = 6;

#include <vector>
#include <iostream>

#include "xxh3.h"

enum token : char
{
    empty,
    red, // red always plays first
    yellow
};

class Board
{
private:
    token grid[BOARD_SIZE_X][BOARD_SIZE_Y] = {{empty}};
    std::pair<int, int> lastPlayedPos;

public:
    token operator()(int x, int y) const;

    const std::vector<int> getLegalMoves() const;

    bool play(token color, int x);

    token checkVictory() const;

    void clear() const;

    XXH128_hash_t getHash() const;

    void getChildren(token, std::vector<Board> &, int limit = 2) const;
};

std::ostream &operator<<(std::ostream &os, const Board &b);

#endif