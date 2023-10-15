#include <iostream>
#include <array>
#include <algorithm>
#include <queue>
#include "board.hpp"
#include "agent.hpp"
#include <string.h>
#include <float.h>
#include <cmath>

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

MiniMaxAgent::MiniMaxAgent(token color) : Agent(color)
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

#define DEPTH 1

// recursively explore children
// at the end, return the sequence of moves where the first one is the one you should take as well as the score
void exploreChildren(BoardHashPair &state, std::array<int, DEPTH> &bestMoves, float &bestScore, token agentColor, token color, int depth = 0)
{
    if (depth == DEPTH)
    {
        bestScore = state.score;
        return;
    }

    if (state.score == INFINITY)
    {
        bestScore = INFINITY;
        return;
    }

    if (state.score == -INFINITY)
    {
        bestScore = -INFINITY;
        return;
    }

    if (color == agentColor) // Maximizing player
    {
        bestScore = -FLT_MAX;
        for (int i = 0; i < state.children.size(); i++)
        {
            float score = -FLT_MAX;
            std::array<int, DEPTH> moves(bestMoves);
            moves[depth] = i;
            exploreChildren(state.children[i], moves, score, agentColor, color == yellow ? red : yellow, depth + 1);
            if (score > bestScore)
            {
                bestMoves = moves;
                bestScore = score;
            }
        }
    }
    else // Minimizing player
    {
        float minScore = FLT_MAX;
        for (int i = 0; i < state.children.size(); i++)
        {
            float score = FLT_MAX;
            std::array<int, DEPTH> moves(bestMoves);
            moves[depth] = i;
            exploreChildren(state.children[i], moves, score, agentColor, color == yellow ? red : yellow, depth + 1);
            if (score < minScore)
            {
                bestMoves = moves;
                minScore = score;
            }
        }
        bestScore = minScore; // Propagate the minScore to bestScore
    }
}

move MiniMaxAgent::getMove(Board board)
{
    BoardHashPair state = {board, board.getHash(), board.evaluate(color)};
    std::vector<BoardHashPair> states;
    states.push_back(state);

    board.getChildren(color == red ? yellow : red, states, state, DEPTH);

    // for (auto state : states)
    // {
    //     std::cout << state.board << std::endl;
    //     std::cout << state.score << std::endl;
    // }

    float bestScore = -FLT_MAX;

    // explore state.children
    std::array<int, DEPTH> moves = {0};
    exploreChildren(state, moves, bestScore, color, color);

    // std::cout << "best score: " << bestScore << std::endl;
    // for (auto i : moves)
    // {
    //     std::cout << i << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "children" << std::endl;
    // for (auto i : state.children)
    // {
    //     std::cout << i.board << std::endl;
    //     std::cout << i.score << std::endl;
    // }

    return moves[0];
}
