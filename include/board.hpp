#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <iostream>

/// @brief The width of the board
constexpr int BOARD_SIZE_X = 7;

/// @brief The height of the board
constexpr int BOARD_SIZE_Y = 6;

struct BoardHashPair;

/// @brief The token enumeration determines the player by whom a specific spot on the board was taken
enum token : char
{
    empty,
    red, // red always plays first
    yellow
};

/// @brief Stores a hash of the board
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

/// @brief Results evaluation data structure
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

    /// @brief Write the evaluation results to the standard output
    friend std::ostream &operator<<(std::ostream &os, const evaluateResults &rslt)
    {
        return rslt.operator<<(os);
    }
};

/// @brief The Board class hosts most of the logic of the game
class Board
{
private:
    /// @brief The grid defining the board
    token grid[BOARD_SIZE_X][BOARD_SIZE_Y] = {{empty}};

    /// @brief The last played position by either the intelligent agent or the human player
    std::pair<int, int> lastPlayedPos;

public:
    /// @brief Returns the token on the board at position (x, y) on the board
    /// @param x The x position of the token, between 0 and `BOARD_SIZE_X - 1`
    /// @param y The y position of the token, between 0 and `BOARD_SIZE_Y - 1`
    /// @return The token at position (x, y)
    token operator()(int x, int y) const;

    /// @brief Returns all legal moves on the board
    /// @return A vector containing all possible moves from an agent
    const std::vector<int> getLegalMoves() const;

    /// @brief Adds a token to a column
    /// @param color The agent's token color
    /// @param x The column in which to insert the token
    /// @return `true` is the token was successfully inserted, `false` otherwise
    bool play(token color, int x);

    /// @brief Checks if a victory occurred on the board by one of the playing agents on the last turn
    /// @return The color of the winning tokens, or the `empty` token
    token checkVictory() const;

    /// @brief Clears the entire board
    void clear() const;

    // XXH128_hash_t getHash() const;

    /// @brief Returns a hash of the board
    /// @return A hash of the board
    BoardHash getHash() const;

    void getChildren(token, std::vector<BoardHashPair> &, int limit = 2) const;

    float evaluate(token color, evaluateResults &rslt) const;

    // functions to check for 3 sized threats, used in evaluate, no erroneous input checking

    // returns 0 if there is no threat, 1 if there is a threat but not immediate (empty slot but no valid emplacement), 2 if there is a threat and immediate
    // 3 if there is a row of 3 surrounded by empty slots (victory next turn)
    int check3HorizontalPositive(int x, int y, token color) const;
    int check3HorizontalNegative(int x, int y, token color) const;

    // returns true if there is a threat, false otherwise
    bool check3Vertical(int x, int y, token color) const;

    // returns 0 if there is no threat, 1 if there is a threat but not immediate (empty slot but no valid emplacement), 2 if there is a threat and immediate
    // 3 if there is a row of 3 surrounded by empty slots (victory next turn)
    int check3DiagonalPositive(int x, int y, token color) const;
    int check3DiagonalNegative(int x, int y, token color) const;

    // returns true if there is a column of 2 filled slots and an empty slot above.
    bool check2Vertical(int x, int y, token color) const;

    // returns 0 if there is no threat,
    // 1 if there are two filled slots and two empty slots,
    // 2 if there is two consecutive empty slots and two filled slots
    int check2HorizontalPositive(int x, int y, token color) const;
    int check2HorizontalNegative(int x, int y, token color) const;
    int check2DiagonalPositive(int x, int y, token color) const;
    int check2DiagonalNegative(int x, int y, token color) const;
};

struct BoardHashPair
{
    Board board;
    BoardHash hash;
};

/// @brief Write the board to the standard output
std::ostream &operator<<(std::ostream &os, const Board &b);

#endif