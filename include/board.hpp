#ifndef BOARD_HPP
#define BOARD_HPP

constexpr int BOARD_SIZE_X = 7;
constexpr int BOARD_SIZE_Y = 6;

#include <vector>
#include <iostream>

struct BoardHashPair;

enum token : char
{
    empty,
    red, // red always plays first
    yellow
};

struct BoardHash
{
    uint64_t low;
    uint64_t high;

    bool operator==(const BoardHash &other) const
    {
        return low == other.low && high == other.high;
    }

    bool operator!=(const BoardHash &other) const
    {
        return !(*this == other);
    }
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

    // XXH128_hash_t getHash() const;

    BoardHash getHash() const;

    void getChildren(token, std::vector<BoardHashPair> &, int limit = 2) const;
};

struct BoardHashPair
{
    Board board;
    BoardHash hash;
};

const int i = sizeof(BoardHashPair);

std::ostream &operator<<(std::ostream &os, const Board &b);

#endif