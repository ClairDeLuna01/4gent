#include <iostream>
#include <algorithm>
#include "agent.hpp"

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
