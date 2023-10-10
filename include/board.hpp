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

struct evaluateResults
{
    int victoryNextTurn = 0;
    int threatHorizontal = 0;
    int threatVertical = 0;
    int threatDiagonal = 0;
    int threat2Horizontal = 0;
    int centrality = 0;

    std::ostream &operator<<(std::ostream &os) const
    {
        os << "victoryNextTurn: " << victoryNextTurn << std::endl;
        os << "threatHorizontal: " << threatHorizontal << std::endl;
        os << "threatVertical: " << threatVertical << std::endl;
        os << "threatDiagonal: " << threatDiagonal << std::endl;
        os << "threat2Horizontal: " << threat2Horizontal << std::endl;
        os << "centrality: " << centrality << std::endl;
        return os;
    }

    friend std::ostream &operator<<(std::ostream &os, const evaluateResults &rslt)
    {
        return rslt.operator<<(os);
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

    float evaluate(token color, evaluateResults &rslt) const;

    // functions to check for 3 sized threats, used in evaluate, no erroneous input checking
    bool check3HorizontalPositive(int x, int y, token color) const;
    bool check3HorizontalNegative(int x, int y, token color) const;
    bool check3Vertical(int x, int y, token color) const;
    bool check3DiagonalPositive(int x, int y, token color) const;
    bool check3DiagonalNegative(int x, int y, token color) const;

    // functions to check for 2 sized threats, used in evaluate, no erroneous input checking
    // returns the number of sides that are free, zero if no threat
    int check2HorizontalPositive(int x, int y, token color) const;
    int check2HorizontalNegative(int x, int y, token color) const;
    int check2Vertical(int x, int y, token color) const;
    int check2DiagonalPositive(int x, int y, token color) const;
    int check2DiagonalNegative(int x, int y, token color) const;
};

struct BoardHashPair
{
    Board board;
    BoardHash hash;
};

std::ostream &operator<<(std::ostream &os, const Board &b);

#endif