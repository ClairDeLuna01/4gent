#include <board.hpp>
#include <string.h>
#include <algorithm>
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
    // // check y lines
    // int count = 0;
    // token color = empty;
    // for(int x = 0; x < BOARD_SIZE_X; x++)
    // {
    //     count = 0;
    //     color = empty;
    //     for(int y = 0; y < BOARD_SIZE_Y; y++)
    //         if(iswinning(grid[x][y], color, count))
    //             return color;
    // }

    // // check x lines
    // for(int y = 0; y < BOARD_SIZE_Y; y++)
    // {
    //     count = 0;
    //     color = empty;
    //     for(int x = 0; x < BOARD_SIZE_X; x++)
    //         if(iswinning(grid[x][y], color, count))
    //             return color;
    // }

    // // checking diagonal +x -y
    // int count2 = 0;
    // token color2 = empty;
    // for(int y = 3; y < BOARD_SIZE_Y; y++)
    // {
    //     count = 0;
    //     color = empty;
    //     count2 = 0;
    //     color2 = empty;
    //     int ytmp = y;
    //     for(int x = 0; x < BOARD_SIZE_X && ytmp > 0; x++, ytmp--)
    //     {
    //         if(iswinning(grid[x][ytmp], color, count))
    //             return color;
    //         if(iswinning(grid[BOARD_SIZE_X-1-x][ytmp], color2, count2))
    //             return color;
    //     }
    // }

    // // checking diagonal +x +y
    // for(int x = 1; x < 4; x++)
    // {
    //     count = 0;
    //     color = empty;
    //     count2 = 0;
    //     color2 = empty;
    //     int xtmp = x;
    //     for(int y = BOARD_SIZE_Y-1; xtmp < BOARD_SIZE_X && y > 0; xtmp++, y--)
    //     {
    //         if(iswinning(grid[xtmp][y], color, count))
    //             return color;
    //         if(iswinning(grid[BOARD_SIZE_X-1-xtmp][y], color2, count2))
    //             return color;
    //     }
    // }

    // return empty;

    // for(int x = 0; x < )

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

void Board::getChildren(token color, std::vector<XXH128_hash_t> &states, int limit) const
{
    std::vector<Board> children;

    std::vector<int> moves = getLegalMoves();
    int movesCount = moves.size();
    for (int i = 0; i < movesCount; i++)
    {

        Board tmp;
        memcpy(&tmp, this, sizeof(Board));

        tmp.play(color, moves[i]);
        if (std::find_if(states.begin(), states.end(), [&](XXH128_hash_t hash)
                         { 
                            XXH128_hash_t hash = tmp.getHash();
                            return hash.low64 == hash.low64 && hash.high64 == hash.high64; }) == states.end())
        {
            states.push_back(tmp.getHash());
            if (limit > 0)
                tmp.getChildren(color == red ? yellow : red, states, limit - 1);
        }
    }
}

XXH128_hash_t Board::getHash() const
{
    return XXH128((char *)grid, BOARD_SIZE_X * BOARD_SIZE_Y, 0);
}