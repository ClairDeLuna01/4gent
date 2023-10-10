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

    token color = grid[lastPlayedPos.first][lastPlayedPos.second];

    int count = 0;
    for(int x = lastPlayedPos.first; x < BOARD_SIZE_X; x ++)
    {
        if(grid[x][lastPlayedPos.second] == color)
            count++;
        else
            break;
    }

    for(int x = lastPlayedPos.first; x >= 0; x --)
    {
        if(grid[x][lastPlayedPos.second] == color)
            count++;
        else
            break;
    }
    
    if(count >= 4)
        return color;
    count = 0;

    for(int y = lastPlayedPos.second; y < BOARD_SIZE_Y; y ++)
    {
        if(grid[lastPlayedPos.first][y] == color)
            count++;
        else
            break;
    }

    for(int y = lastPlayedPos.second; y >= 0; y --)
    {
        if(grid[lastPlayedPos.first][y] == color)
            count++;
        else
            break;
    }

    if(count >= 4)
        return color;
    count = 0;

    int x = lastPlayedPos.first;
    int y = lastPlayedPos.second;
    for(;x < BOARD_SIZE_X && y < BOARD_SIZE_Y; x++, y++)
    {
        if(grid[x][y] == color)
            count++;
        else
            break;
    }

    x = lastPlayedPos.first;
    y = lastPlayedPos.second;
    for(;x >= 0 && y >= 0; x--, y--)
    {
        if(grid[x][y] == color)
            count++;
        else
            break;
    }

    if(count >= 4)
        return color;
    count = 0;

    x = lastPlayedPos.first;
    y = lastPlayedPos.second;
    for(;x >= 0 && y < BOARD_SIZE_Y; x--, y++)
    {
        if(grid[x][y] == color)
            count++;
        else
            break;
    }

    x = lastPlayedPos.first;
    y = lastPlayedPos.second;
    for(;x < BOARD_SIZE_X && y >= 0; x++, y--)
    {
        if(grid[x][y] == color)
            count++;
        else
            break;
    }

    if(count >= 4)
        return color;
    count = 0;

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
