#include <iostream>
#include <array>
#include <algorithm>
#include <queue>
#include "board.hpp"
#include "agent.hpp"
#include <string.h>
#include <float.h>

Agent::Agent(token color)
{
    this->color = color;
}

move Agent::getMove(Board board)
{
    return board.getLegalMoves()[0];
}

Player::Player(token color) : Agent(color)
{
}

move Player::getMove(Board board)
{
    std::vector<move> legalMoves = board.getLegalMoves();
    std::cout << board << std::endl;
    move mv;
    while (1)
    {
        std::cout << "Enter a move: ";
        std::cin >> mv;
        if (std::find(legalMoves.begin(), legalMoves.end(), mv) != legalMoves.end())
        {
            return mv;
        }
        std::cout << "Invalid move!" << std::endl;
    }

    return mv;
}

std::vector<BoardHashPair> getStates()
{

    Board board;
    board.clear();

    /* This variable stores who is currently playing.
     * true     -> player
     * false    -> agent
     * The agent always play in second.
     */
    // bool turn = true;

    std::vector<BoardHashPair> states;
    states.push_back({board, board.getHash(), board.evaluate(red)});

    board.getChildren(red, states, states[0], 0);

    return states;
}

#define DEPTH 5

// recursively explore children
// at the end, return the sequence of moves with the best score as well as the score
void exploreChildren(BoardHashPair &state, std::array<int, DEPTH> &bestMoves, float &bestScore, int depth = 0)
{
    if (depth == DEPTH)
    {
        bestScore = state.score;
        return;
    }

    for (int i = 0; i < state.children.size(); i++)
    {
        float score = -FLT_MAX;
        std::array<int, DEPTH> moves(bestMoves);
        moves[depth] = i;
        exploreChildren(state.children[i], moves, score, depth + 1);
        if (score > bestScore)
        {
            bestMoves = moves;
            bestScore = score;
        }
    }
}

move MiniMaxAgent::getMove(Board board)
{
    BoardHashPair state = {board, board.getHash(), board.evaluate(color)};
    std::vector<BoardHashPair> states;
    states.push_back(state);

    board.getChildren(color, states, state, DEPTH);

    float bestScore = -FLT_MAX;

    // explore state.children
    std::array<int, DEPTH> moves = {0};
    exploreChildren(state, moves, bestScore);

    std::cout << "best score: " << bestScore << std::endl;
    for (auto i : moves)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return moves[0];
}
