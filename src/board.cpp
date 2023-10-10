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

    for (int x = 0; x < BOARD_SIZE_X; x++)
    {
        for (int y = 0; y < BOARD_SIZE_Y; y++)
        {
            if (grid[x][y] == empty)
                continue;
            // check horizontal
            token c = grid[x][y];
            if (x < BOARD_SIZE_X - 3 && c == grid[x + 1][y] && c == grid[x + 2][y] && c == grid[x + 3][y])
                return c;
            // check vertical
            if (y < BOARD_SIZE_Y - 3 && c == grid[x][y + 1] && c == grid[x][y + 2] && c == grid[x][y + 3])
                return c;
            // check diagonal
            if (x < BOARD_SIZE_X - 3 && y < BOARD_SIZE_Y - 3 && c == grid[x + 1][y + 1] && c == grid[x + 2][y + 2] && c == grid[x + 3][y + 3])
                return c;

            if (x > 2 && y < BOARD_SIZE_Y - 3 && c == grid[x - 1][y + 1] && c == grid[x - 2][y + 2] && c == grid[x - 3][y + 3])
                return c;
        }
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

#define VICTORY_NEXT_TURN 1000000
#define THREAT_HORIZONTAL 1000
#define THREAT_VERTICAL 2000
#define CENTRALITY_HIGH 100
#define CENTRALITY_LOW 0

float Board::evaluate(token color) const
{
    const token opponent = color == red ? yellow : red;
    float score = 0;
    // test if there can be a win in the next move
    for (int x = 0; x < BOARD_SIZE_X; x++)
    {
        Board tmp(*this);
        tmp.play(color, x);
        if (tmp.checkVictory() == color)
            return 1000000;
    }

    // test if the opponent can win in the next move
    for (int x = 0; x < BOARD_SIZE_X; x++)
    {
        Board tmp(*this);
        tmp.play(opponent, x);
        if (tmp.checkVictory() == opponent)
            return -1000000;
    }

    // test if there is a threat of 3 in a row with a free space on the left or right
    for (int x = 0; x < BOARD_SIZE_X; x++)
    {
        for (int y = 0; y < BOARD_SIZE_Y; y++)
        {
            if (grid[x][y] != empty)
            {
                int myColor = (grid[x][y] == color ? 1 : -1);
                // horizontal threat
                if (x < BOARD_SIZE_X - 3 && grid[x + 1][y] == color && grid[x + 2][y] == color && grid[x + 3][y] == empty)
                    score += THREAT_HORIZONTAL * myColor;
                if (x > 2 && grid[x - 1][y] == color && grid[x - 2][y] == color && grid[x - 3][y] == empty)
                    score += THREAT_HORIZONTAL * myColor;

                // vertical threat
                if (y < BOARD_SIZE_Y - 3 && grid[x][y + 1] == color && grid[x][y + 2] == color && grid[x][y + 3] == empty)
                    score += THREAT_VERTICAL * myColor;

                // diagonal threat
                if (x < BOARD_SIZE_X - 3 && y < BOARD_SIZE_Y - 3 && grid[x + 1][y + 1] == color && grid[x + 2][y + 2] == color && grid[x + 3][y + 3] == empty)
                    score += THREAT_HORIZONTAL * myColor;
            }
        }
    }

    // test if there is a threat of 2 in a row with a free space on the left AND right
    for (int x = 1; x < BOARD_SIZE_X; x++)
    {
        for (int y = 0; y < BOARD_SIZE_Y; y++)
        {
            if (grid[x][y] != empty)
            {
                int myColor = (grid[x][y] == color ? 1 : -1);
                // horizontal threat
                if (x < BOARD_SIZE_X - 2 && grid[x + 1][y] == color && grid[x + 2][y] == empty && grid[x - 1][y] == empty)
                    score += THREAT_HORIZONTAL * myColor;
            }
        }
    }

    // add score based on piece position
    for (int x = 0; x < BOARD_SIZE_X; x++)
    {
        for (int y = 0; y < BOARD_SIZE_Y; y++)
        {
            if (grid[x][y] != empty)
            {
                int myColor = (grid[x][y] == color ? 1 : -1);
                float cent = (x < BOARD_SIZE_X / 2 ? x : BOARD_SIZE_X - x) * CENTRALITY_HIGH / (BOARD_SIZE_X / 2) + CENTRALITY_LOW;
                score += cent * myColor;
            }
        }
    }

    return score;
}