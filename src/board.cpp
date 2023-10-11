#include <board.hpp>
#include <string.h>
#include <algorithm>
#include <cmath>
#include <memory>
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
    for (int i = bMin; i <= bMax; ++i)
    {
        const token startToken = grid[x + i][y + i];
        if (startToken == grid[x + i + 1][y + i + 1] && startToken == grid[x + i + 2][y + i + 2] && startToken == grid[x + i + 3][y + i + 3])
            return lastPlayedToken;
    }

    // Checks diagonally (\)
    bMin = std::max(-3, -std::min(x, nY));
    bMax = std::min(0, std::min(nX, y));
    for (int i = bMin; i <= bMax; ++i)
    {
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

void Board::getChildren(token color, std::vector<BoardHashPair> &states, BoardHashPair &thisBoard, int limit) const
{
    std::vector<BoardHashPair> children;

    std::vector<int> moves = getLegalMoves();
    int movesCount = moves.size();
    for (int i = 0; i < movesCount; i++)
    {

        Board tmp(*this);

        tmp.play(color, moves[i]);

        const BoardHash newHash = tmp.getHash();
        // std::cout << newHash.low << " " << newHash.high << std::endl;
        if (std::find_if(states.begin(), states.end(), [&](BoardHashPair pair)
                         { return newHash == pair.hash; }) == states.end())
        {
            float score = tmp.evaluate(color);
            BoardHashPair child = {tmp, newHash, score};
            states.push_back(child);
            if (limit > 0)
                tmp.getChildren(color == red ? yellow : red, states, child, limit - 1);
            children.push_back(child);
        }
    }

    thisBoard.children = children;
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
            int bit = (x * BOARD_SIZE_Y + y) * 2;
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

    // std::cout << low << " " << high << std::endl;

    return hash;
}

bool Board::check3Vertical(int x, int y, token color) const
{
    return grid[x][y + 1] == color && grid[x][y + 2] == color && grid[x][y + 3] == empty;
}

int Board::check3HorizontalPositive(int x, int y, token color) const
{
    int count = 0;
    int countValidEmpty = 0;

    bool countIndices[5] = {false, true, false, false, false};

    if (x > 0)
    {
        if (grid[x - 1][y] == color)
        {
            count++;
            countIndices[0] = true;
        }
        else if (grid[x - 1][y] == empty && (x == 0 || grid[x - 1][y - 1] != empty))
            countValidEmpty++;
    }

    if (grid[x + 1][y] == color)
    {
        count++;
        countIndices[1] = true;
    }
    else if (grid[x + 1][y] == empty && (x == 0 || grid[x + 1][y - 1] != empty))
        countValidEmpty++;

    if (grid[x + 2][y] == color)
    {
        count++;
        countIndices[2] = true;
    }
    else if (grid[x + 2][y] == empty && (x == 0 || grid[x + 2][y - 1] != empty))
        countValidEmpty++;

    if (grid[x + 3][y] == color)
    {
        count++;
        countIndices[3] = true;
    }
    else if (grid[x + 3][y] == empty && (x == 0 || grid[x + 3][y - 1] != empty))
        countValidEmpty++;

    if (countValidEmpty == 2 && count == 3 && countIndices[1] && countIndices[2] && countIndices[3])
    {
        return 3;
    }

    return (count == 3) + (countValidEmpty >= 1);
}

int Board::check3HorizontalNegative(int x, int y, token color) const
{
    int count = 0;
    int countValidEmpty = 0;

    bool countIndices[5] = {false, true, false, false, false};

    if (x < BOARD_SIZE_Y - 1)
    {
        if (grid[x + 1][y] == color)
        {
            count++;
            countIndices[0] = true;
        }
        else if (grid[x + 1][y] == empty && (x == 0 || grid[x + 1][y - 1] != empty))
            countValidEmpty++;
    }

    if (grid[x - 1][y] == color)
    {
        count++;
        countIndices[1] = true;
    }
    else if (grid[x - 1][y] == empty && (x == 0 || grid[x - 1][y - 1] != empty))
        countValidEmpty++;

    if (grid[x - 2][y] == color)
    {
        count++;
        countIndices[2] = true;
    }
    else if (grid[x - 2][y] == empty && (x == 0 || grid[x - 2][y - 1] != empty))
        countValidEmpty++;

    if (grid[x - 3][y] == color)
    {
        count++;
        countIndices[3] = true;
    }
    else if (grid[x - 3][y] == empty && (x == 0 || grid[x - 3][y - 1] != empty))
        countValidEmpty++;

    if (countValidEmpty == 2 && count == 3 && countIndices[1] && countIndices[2] && countIndices[3])
    {
        return 3;
    }

    return (count == 3) + (countValidEmpty >= 1);
}

int Board::check3DiagonalPositive(int x, int y, token color) const
{
    int count = 0;
    int countValidEmpty = 0;

    bool countIndices[5] = {false, true, false, false, false};

    if (x > 0 && y > 0)
    {
        if (grid[x - 1][y - 1] == color)
        {
            count++;
            countIndices[0] = true;
        }
        else if (grid[x - 1][y - 1] == empty && (x == 0 || grid[x - 1][y - 2] != empty))
            countValidEmpty++;
    }

    if (grid[x + 1][y + 1] == color)
    {
        count++;
        countIndices[1] = true;
    }
    else if (grid[x + 1][y + 1] == empty && (x == 0 || grid[x + 1][y] != empty))
        countValidEmpty++;

    if (grid[x + 2][y + 2] == color)
    {
        count++;
        countIndices[2] = true;
    }
    else if (grid[x + 2][y + 2] == empty && (x == 0 || grid[x + 2][y + 1] != empty))
        countValidEmpty++;

    if (grid[x + 3][y + 3] == color)
    {
        count++;
        countIndices[3] = true;
    }
    else if (grid[x + 3][y + 3] == empty && (x == 0 || grid[x + 3][y + 2] != empty))
        countValidEmpty++;

    if (countValidEmpty == 2 && count == 3 && countIndices[1] && countIndices[2] && countIndices[3])
    {
        return 3;
    }

    return (count == 3) + (countValidEmpty >= 1);
}

int Board::check3DiagonalNegative(int x, int y, token color) const
{
    int count = 0;
    int countValidEmpty = 0;

    bool countIndices[5] = {false, true, false, false, false};

    if (x < BOARD_SIZE_X - 1 && y > 0)
    {
        if (grid[x + 1][y - 1] == color)
        {
            count++;
            countIndices[0] = true;
        }
        else if (grid[x + 1][y - 1] == empty && (x == 0 || grid[x + 1][y - 2] != empty))
            countValidEmpty++;
    }

    if (grid[x - 1][y + 1] == color)
    {
        count++;
        countIndices[1] = true;
    }
    else if (grid[x - 1][y + 1] == empty && (x == 0 || grid[x - 1][y] != empty))
        countValidEmpty++;

    if (grid[x - 2][y + 2] == color)
    {
        count++;
        countIndices[2] = true;
    }
    else if (grid[x - 2][y + 2] == empty && (x == 0 || grid[x - 2][y + 1] != empty))
        countValidEmpty++;

    if (grid[x - 3][y + 3] == color)
    {
        count++;
        countIndices[3] = true;
    }
    else if (grid[x - 3][y + 3] == empty && (x == 0 || grid[x - 3][y + 2] != empty))
        countValidEmpty++;

    if (countValidEmpty == 2 && count == 2 && countIndices[1] && countIndices[2] && countIndices[3])
    {
        return 3;
    }

    return (count == 2) + (countValidEmpty >= 1);
}

bool Board::check2Vertical(int x, int y, token color) const
{
    return grid[x][y + 1] == color && grid[x][y + 2] == empty;
}

int Board::check2HorizontalPositive(int x, int y, token color) const
{
    int count = 0;
    int countActualEmpty = 0;

    bool countIndices[4] = {false, true, false, false};

    if (x > 0)
    {
        if (grid[x - 1][y] == color)
        {
            count++;
            countIndices[0] = true;
        }
        else if (grid[x - 1][y] == empty)
            countActualEmpty++;
    }

    if (grid[x + 1][y] == color)
    {
        count++;
        countIndices[1] = true;
    }
    else if (grid[x + 1][y] == empty)
        countActualEmpty++;

    if (grid[x + 2][y] == color)
    {
        count++;
        countIndices[2] = true;
    }
    else if (grid[x + 2][y] == empty)
        countActualEmpty++;

    if (countActualEmpty == 2 && count == 1 && countIndices[1] && countIndices[2])
    {
        return 2;
    }

    return count == 1 && countActualEmpty >= 2;
}

int Board::check2HorizontalNegative(int x, int y, token color) const
{
    int count = 0;
    int countActualEmpty = 0;

    bool countIndices[4] = {false, true, false, false};

    if (x < BOARD_SIZE_X - 1)
    {
        if (grid[x + 1][y] == color)
        {
            count++;
            countIndices[0] = true;
        }
        else if (grid[x + 1][y] == empty)
            countActualEmpty++;
    }

    if (grid[x - 1][y] == color)
    {
        count++;
        countIndices[1] = true;
    }
    else if (grid[x - 1][y] == empty)
        countActualEmpty++;

    if (grid[x - 2][y] == color)
    {
        count++;
        countIndices[2] = true;
    }
    else if (grid[x - 2][y] == empty)
        countActualEmpty++;

    if (countActualEmpty == 2 && count == 1 && countIndices[1] && countIndices[2])
    {
        return 2;
    }

    return count == 1 && countActualEmpty >= 2;
}

int Board::check2DiagonalPositive(int x, int y, token color) const
{
    int count = 0;
    int countActualEmpty = 0;

    bool countIndices[4] = {false, true, false, false};

    if (x > 0 && y > 0)
    {
        if (grid[x - 1][y - 1] == color)
        {
            count++;
            countIndices[0] = true;
        }
        else if (grid[x - 1][y - 1] == empty)
            countActualEmpty++;
    }

    if (grid[x + 1][y + 1] == color)
    {
        count++;
        countIndices[1] = true;
    }
    else if (grid[x + 1][y + 1] == empty)
        countActualEmpty++;

    if (grid[x + 2][y + 2] == color)
    {
        count++;
        countIndices[2] = true;
    }
    else if (grid[x + 2][y + 2] == empty)
        countActualEmpty++;

    if (countActualEmpty == 2 && count == 1 && countIndices[1] && countIndices[2])
    {
        return 2;
    }

    return count == 1 && countActualEmpty >= 2;
}

int Board::check2DiagonalNegative(int x, int y, token color) const
{
    int count = 0;
    int countActualEmpty = 0;

    bool countIndices[4] = {false, true, false, false};

    if (x < BOARD_SIZE_X - 1 && y > 0)
    {
        if (grid[x + 1][y - 1] == color)
        {
            count++;
            countIndices[0] = true;
        }
        else if (grid[x + 1][y - 1] == empty)
            countActualEmpty++;
    }

    if (grid[x - 1][y + 1] == color)
    {
        count++;
        countIndices[1] = true;
    }
    else if (grid[x - 1][y + 1] == empty)
        countActualEmpty++;

    if (grid[x - 2][y + 2] == color)
    {
        count++;
        countIndices[2] = true;
    }
    else if (grid[x - 2][y + 2] == empty)
        countActualEmpty++;

    if (countActualEmpty == 2 && count == 1 && countIndices[1] && countIndices[2])
    {
        return 2;
    }

    return count == 1 && countActualEmpty >= 2;
}

#define VICTORY INFINITY

#define THREAT3_HORIZONTAL 1000
#define THREAT3_HORIZONTAL_BIG 100000
#define THREAT3_HORIZONTAL_SMALL 500

#define THREAT3_DIAGONAL 1000
#define THREAT3_DIAGONAL_BIG 100000
#define THREAT3_DIAGONAL_SMALL 500

#define THREAT3_VERTICAL 2000

#define THREAT2_HORIZONTAL 1000
#define THREAT2_HORIZONTAL_BIG 1500

#define THREAT2_DIAGONAL 1000
#define THREAT2_DIAGONAL_BIG 1500

#define THREAT2_VERTICAL 200

#define CENTRALITY_HIGH 100
#define CENTRALITY_LOW 0
float Board::evaluate(token color) const
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
                if (x < BOARD_SIZE_X - 3)
                {
                    int test = check3HorizontalPositive(x, y, tileColor);

                    switch (test)
                    {
                    case 1:
                        score += THREAT3_HORIZONTAL_SMALL * myColor;
                        break;
                    case 2:
                        score += THREAT3_HORIZONTAL * myColor;
                        break;
                    case 3:
                        score += THREAT3_HORIZONTAL_BIG * myColor;
                        break;
                    default:
                        break;
                    }
                }
                if (x > 2)
                {
                    int test = check3HorizontalNegative(x, y, tileColor);

                    switch (test)
                    {
                    case 1:
                        score += THREAT3_DIAGONAL_SMALL * myColor;
                        break;
                    case 2:
                        score += THREAT3_HORIZONTAL * myColor;
                        break;
                    case 3:
                        score += THREAT3_HORIZONTAL_BIG * myColor;
                        break;
                    default:
                        break;
                    }
                }

                // vertical threat
                if (y < BOARD_SIZE_Y - 3 && check3Vertical(x, y, tileColor))
                {
                    score += THREAT3_VERTICAL * myColor;
                }

                // diagonal threat
                if (x < BOARD_SIZE_X - 3 && y < BOARD_SIZE_Y - 3)
                {
                    int test = check3DiagonalPositive(x, y, tileColor);

                    switch (test)
                    {
                    case 1:
                        score += THREAT3_DIAGONAL_SMALL * myColor;
                        break;
                    case 2:
                        score += THREAT3_DIAGONAL * myColor;
                        break;
                    case 3:
                        score += THREAT3_DIAGONAL_BIG * myColor;
                        break;
                    default:
                        break;
                    };
                }

                if (x > 2 && y < BOARD_SIZE_Y - 3)
                {
                    int test = check3DiagonalNegative(x, y, tileColor);

                    switch (test)
                    {
                    case 1:
                        score += THREAT3_DIAGONAL_SMALL * myColor;
                        break;
                    case 2:
                        score += THREAT3_DIAGONAL * myColor;
                        break;
                    case 3:
                        score += THREAT3_DIAGONAL_BIG * myColor;
                        break;
                    default:
                        break;
                    };
                }

                // test if there is a threat of 2 in a row with a free space on the left AND right
                if (grid[x][y] != empty)
                {
                    int test;

                    test = x >= 1 && x <= BOARD_SIZE_X - 3 ? check2HorizontalPositive(x, y, tileColor) : 0;
                    switch (test)
                    {
                    case 1:
                        score += THREAT2_HORIZONTAL * myColor;
                        break;
                    case 2:
                        score += THREAT2_HORIZONTAL_BIG * myColor;
                        break;
                    default:
                        break;
                    };

                    test = x >= 2 && x <= BOARD_SIZE_X - 2 ? check2HorizontalNegative(x, y, tileColor) : 0;
                    switch (test)
                    {
                    case 1:
                        score += THREAT2_HORIZONTAL * myColor;
                        break;
                    case 2:
                        score += THREAT2_HORIZONTAL_BIG * myColor;
                        break;
                    default:
                        break;
                    };

                    test =
                        x >= 1 && x <= BOARD_SIZE_X - 3 && y >= 1 && y <= BOARD_SIZE_Y - 3 ? check2DiagonalPositive(x, y, tileColor) : 0;

                    switch (test)
                    {
                    case 1:
                        score += THREAT2_DIAGONAL * myColor;
                        break;
                    case 2:
                        score += THREAT2_DIAGONAL_BIG * myColor;
                        break;
                    default:
                        break;
                    };

                    test =
                        x >= 2 && x <= BOARD_SIZE_X - 2 && y >= 2 && y <= BOARD_SIZE_Y - 2
                            ? check2DiagonalNegative(x, y, tileColor)
                            : 0;
                    switch (test)
                    {
                    case 1:
                        score += THREAT2_DIAGONAL * myColor;
                        break;
                    case 2:
                        score += THREAT2_DIAGONAL_BIG * myColor;
                        break;
                    default:
                        break;
                    };
                }

                // add score based on piece position
                if (grid[x][y] != empty)
                {
                    int myColor = (grid[x][y] == color ? 1 : -1);
                    float cent = (float)CENTRALITY_LOW + (float)CENTRALITY_HIGH * ((1.0f - (float)std::abs(x - 3) / 3.0f));
                    score += cent * myColor;
                }
            }
        }
    }

    return score;
}

bool Board::isFull() const {
    for (int i = 0; i < BOARD_SIZE_X; ++i)
        if (grid[i][BOARD_SIZE_Y - 1] == empty)
            return false;
    return true;
}