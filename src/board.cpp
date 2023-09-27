#include <board.hpp>
#include <string.h>
#include <algorithm>
#include "agent.hpp"

token Board::operator()(int x, int y) const
{
    return grid[x%BOARD_SIZE_X][y%BOARD_SIZE_Y];
}

bool Board::play(token color, int x)
{
    if(x >= BOARD_SIZE_X || grid[x][BOARD_SIZE_Y-1] != empty)
    {
        std::cout << "illegal move :(\n";
        return false;
    }

    for(int i = BOARD_SIZE_Y-1; i >= 0; i--)
    {
        if(!i || grid[x][i-1] != empty)
        {
            grid[x][i] = color;
            break;
        }
    }

    return true;
}

const std::vector<int> Board::getLegalMoves() const
{
    std::vector<int> res;

    for(int x = 0; x < BOARD_SIZE_X; x++)
        if(grid[x][BOARD_SIZE_Y-1] == empty)
            res.push_back(x);
    
    return res;
}

void Board::clear() const
{
    memset((char *)grid, 0, BOARD_SIZE_X*BOARD_SIZE_Y);
}

std::ostream& operator<<(std::ostream& os, const token t)
{
    char tyellow[] = "\033[93m";
    char tred[] = "\033[31m";
    char treset[] = "\033[0m";
    
    switch (t)
    {
        case empty: os << " "; break;
        case red: os << tred <<"O" << treset; break;
        case yellow: os << tyellow <<"O" << treset; break;
        default: os << treset << "$"; break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const Board& b)
{
    os << "\n";
    for(int y = BOARD_SIZE_Y-1; y >= 0; y--)
    {
        os << "|";
        for(int x = 0; x < BOARD_SIZE_X; x++)
        {
            os << b(x, y) << " ";
        }

        os << "|\n";
    }

    os << "================\n";

    return os;
}

void Board::getChildren(token color, std::vector<Board> &states, int limit) const 
{
    std::vector<Board> children;

    std::vector<int> moves = getLegalMoves();
    int movesCount = moves.size();
    for(int i = 0; i < movesCount; i++) {

        Board tmp;
        memcpy(&tmp, this, sizeof(Board));

        tmp.play(color, moves[i]);
        if (std::find_if(states.begin(), states.end(), [&](auto elem) {
            return memcmp(&elem, &tmp, sizeof(Board)) == 0;
        }) == states.end()) {
            states.push_back(tmp);
            if(limit > 0) 
                tmp.getChildren(color == red ? yellow : red, states, limit-1);

        }

    }

}