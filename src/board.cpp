#include <board.hpp>
#include <string.h>
#include <algorithm>
#include <cmath>
#include "agent.hpp"

token Board::operator()(int x, int y) const
{
    return grid[x % BOARD_SIZE_X][y % BOARD_SIZE_Y];
}

bool Board::play(token color, int x)
{
    if (x >= BOARD_SIZE_X || grid[x][BOARD_SIZE_Y - 1] != empty)
    {
        std::cout << "illegal move :(\n";
        return false;
    }

    int y = 0;
    for (int i = BOARD_SIZE_Y - 1; i >= 0; i--)
    {
        if (!i || grid[x][i - 1] != empty)
        {
            grid[x][i] = color;
            y = i;
            break;
        }
    }

    lastPlayedPos.first = x;
    lastPlayedPos.second = y;

    return true;
}

const std::vector<int> Board::getLegalMoves() const
{
    std::vector<int> res;

    for (int x = 0; x < BOARD_SIZE_X; x++)
        if (grid[x][BOARD_SIZE_Y - 1] == empty)
            res.push_back(x);

    return res;
}

void Board::clear() const
{
    memset((char *)grid, 0, BOARD_SIZE_X * BOARD_SIZE_Y);
}

bool iswinning(token t, token &color, int &count)
{
    if (t && t == color)
    {
        count++;
        if (count == 4)
            return true;
    }
    if (t && t != color)
    {
        count = 1;
        color = t;
    }

    return false;
}

token Board::checkVictory() const
{
    const int x = lastPlayedPos.first;
    const int y = lastPlayedPos.second;
    const token lastPlayedToken = grid[x][y];

    // Checks horizontally
    int bMin = std::max(0, x - 3);
    int bMax = std::min(BOARD_SIZE_X - 4, x);
    for (int i = bMin; i <= bMax; ++i)
        if (grid[i][y] == grid[i + 1][y] && grid[i][y] == grid[i + 2][y] && grid[i][y] == grid[i + 3][y])
            return lastPlayedToken;

    // Checks vertically (downwards only)
    if (y >= 3 && grid[x][y] == grid[x][y - 1] && grid[x][y] == grid[x][y - 2] && grid[x][y] == grid[x][y - 3])
        return lastPlayedToken;

    // Diagonal checking
    const int nX = BOARD_SIZE_X - x - 1;
    const int nY = BOARD_SIZE_Y - y - 1;

    // Checks diagonally (/)
    bMin = std::max(-3, -std::min(x, y));
    bMax = std::min(0, std::min(nX, nY));
    for (int i = bMin; i <= bMax; ++i) {
        const token startToken = grid[x + i][y + i];
        if (startToken == grid[x + i + 1][y + i + 1] && startToken == grid[x + i + 2][y + i + 2] && startToken == grid[x + i + 3][y + i + 3])
            return lastPlayedToken;
    }

    // Checks diagonally (\)
    bMin = std::max(-3, -std::min(x, nY));
    bMax = std::min(0, std::min(nX, y));
    for (int i = bMin; i <= bMax; ++i) {
        const token startToken = grid[x + i][y - i];
        if (startToken == grid[x + i + 1][y - i - 1] && startToken == grid[x + i + 2][y - i - 2] && startToken == grid[x + i + 3][y - i - 3])
            return lastPlayedToken;
    }

    return empty;
}

std::ostream &operator<<(std::ostream &os, const token t)
{
    char tyellow[] = "\033[93m";
    char tred[] = "\033[31m";
    char treset[] = "\033[0m";

    switch (t)
    {
    case empty:
        os << " ";
        break;
    case red:
        os << tred << "O" << treset;
        break;
    case yellow:
        os << tyellow << "O" << treset;
        break;
    default:
        os << treset << "$";
        break;
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const Board &b)
{
    os << "\n";
    for (int y = BOARD_SIZE_Y - 1; y >= 0; y--)
    {
        os << "|";
        for (int x = 0; x < BOARD_SIZE_X; x++)
        {
            os << b(x, y) << " ";
        }

        os << "|\n";
    }

    os << "================\n";

    return os;
}

void Board::getChildren(token color, std::vector<BoardHashPair> &states, int limit) const
{
    std::vector<Board> children;

    std::vector<int> moves = getLegalMoves();
    int movesCount = moves.size();
    for (int i = 0; i < movesCount; i++)
    {

        Board tmp(*this);
        // memcpy(&tmp, this, sizeof(Board));

        tmp.play(color, moves[i]);
        const BoardHash newHash = tmp.getHash();
        if (std::find_if(states.begin(), states.end(), [&](BoardHashPair pair)
                         { return newHash == pair.hash; }) == states.end())
        {
            states.push_back({tmp, newHash});
            if (limit > 0)
                tmp.getChildren(color == red ? yellow : red, states, limit - 1);
        }
    }
}

BoardHash Board::getHash() const
{
    BoardHash hash;

    uint64_t low = 0;
    uint64_t high = 0;

    constexpr unsigned int empty = 0b00;
    constexpr unsigned int red = 0b01;
    constexpr unsigned int yellow = 0b10;

    for (int x = 0; x < BOARD_SIZE_X; x++)
    {
        for (int y = 0; y < BOARD_SIZE_Y; y++)
        {
            int bit = x * BOARD_SIZE_Y + y;
            unsigned int color = grid[x][y] == empty ? empty : grid[x][y] == red ? red
                                                                                 : yellow;

            if (bit < 64)
            {
                low |= color << bit;
            }
            else
            {
                high |= color << (bit - 64);
            }
        }
    }

    hash.low = low;
    hash.high = high;

    return hash;
}

#define VICTORY_NEXT_TURN INFINITY
#define THREAT_HORIZONTAL 1000
#define THREAT_VERTICAL 2000
#define CENTRALITY_HIGH 100
#define CENTRALITY_LOW 0

bool Board::check3Vertical(int x, int y, token color) const
{
    return grid[x][y + 1] == color && grid[x][y + 2] == color && grid[x][y + 3] == empty;
}
bool Board::check3DiagonalPositive(int x, int y, token color) const
{
    return grid[x + 1][y + 1] == color && grid[x + 2][y + 2] == color && grid[x + 3][y + 3] == empty && (x == 0 || grid[x + 3][y + 4] != empty);
}
bool Board::check3DiagonalNegative(int x, int y, token color) const
{
    return grid[x - 1][y + 1] == color && grid[x - 2][y + 2] == color && grid[x - 3][y + 3] == empty && (x == 0 || grid[x - 3][y + 4] != empty);
}

bool Board::check3HorizontalPositive(int x, int y, token color) const
{
    int count = 0;
    int countValidEmpty = 0;

    if (grid[x + 1][y] == color)
        count++;
    else if (grid[x + 1][y] == empty && (x == 0 || grid[x + 1][y - 1] != empty))
        countValidEmpty++;

    if (grid[x + 2][y] == color)
        count++;
    else if (grid[x + 2][y] == empty && (x == 0 || grid[x + 2][y - 1] != empty))
        countValidEmpty++;

    if (grid[x + 3][y] == color)
        count++;
    else if (grid[x + 3][y] == empty && (x == 0 || grid[x + 3][y - 1] != empty))
        countValidEmpty++;

    return count == 2 && countValidEmpty == 1;
}

bool Board::check3HorizontalNegative(int x, int y, token color) const
{
    int count = 0;
    int countValidEmpty = 0;

    if (grid[x - 1][y] == color)
        count++;
    else if (grid[x - 1][y] == empty && (x == 0 || grid[x - 1][y - 1] != empty))
        countValidEmpty++;

    if (grid[x - 2][y] == color)
        count++;
    else if (grid[x - 2][y] == empty && (x == 0 || grid[x - 2][y - 1] != empty))
        countValidEmpty++;

    if (grid[x - 3][y] == color)
        count++;
    else if (grid[x - 3][y] == empty && (x == 0 || grid[x - 3][y - 1] != empty))
        countValidEmpty++;

    return count == 2 && countValidEmpty == 1;
}

float Board::evaluate(token color, evaluateResults &rslt) const
{
    float score = 0;
    // test if there is a threat of 3 in a row with a free space on the left or right
    for (int x = 0; x < BOARD_SIZE_X; x++)
    {
        for (int y = 0; y < BOARD_SIZE_Y; y++)
        {
            if (grid[x][y] != empty)
            {
                int myColor = (grid[x][y] == color ? 1 : -1);
                token tileColor = grid[x][y];
                // horizontal threat
                if (x < BOARD_SIZE_X - 3 && check3HorizontalPositive(x, y, tileColor))
                {
                    score += VICTORY_NEXT_TURN * myColor;
                    rslt.threatHorizontal++;
                }
                if (x > 2 && check3HorizontalNegative(x, y, tileColor))
                {
                    score += VICTORY_NEXT_TURN * myColor;
                    rslt.threatHorizontal++;
                }

                // vertical threat
                if (y < BOARD_SIZE_Y - 3 && check3Vertical(x, y, tileColor))
                {
                    score += VICTORY_NEXT_TURN * myColor;
                    rslt.threatVertical++;
                }

                // diagonal threat
                if (x < BOARD_SIZE_X - 3 && y < BOARD_SIZE_Y - 3 && check3DiagonalPositive(x, y, tileColor))
                {
                    score += VICTORY_NEXT_TURN * myColor;
                    rslt.threatDiagonal++;
                }

                if (x > 2 && y < BOARD_SIZE_Y - 3 && check3DiagonalNegative(x, y, tileColor))
                {
                    score += VICTORY_NEXT_TURN * myColor;
                    rslt.threatDiagonal++;
                }

                // test if there is a threat of 2 in a row with a free space on the left AND right
                if (grid[x][y] != empty)
                {
                    int myColor = (grid[x][y] == color ? 1 : -1);
                    token tileColor = grid[x][y];
                    // horizontal threat
                    if (grid[x + 1][y] == tileColor && grid[x + 2][y] == empty && grid[x - 1][y] == empty)
                    {
                        score += THREAT_HORIZONTAL * myColor;
                        rslt.threat2Horizontal++;
                    }
                }

                // add score based on piece position
                if (grid[x][y] != empty)
                {
                    int myColor = (grid[x][y] == color ? 1 : -1);
                    float cent = (float)CENTRALITY_LOW + (float)CENTRALITY_HIGH * ((1.0f - (float)std::abs(x - 3) / 3.0f));
                    score += cent * myColor;
                    rslt.centrality += cent * myColor;
                }
            }
        }
    }

    return score;
}