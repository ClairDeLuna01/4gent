#ifndef AGENT_HPP
#define AGENT_HPP

typedef int move;

#include <board.hpp>

class Agent
{
protected:
    token color;

public:
    Agent(token color);

    virtual move getMove(Board board);
};

class Player : Agent
{
public:
    Player(token color);

    move getMove(Board board);
};

class MiniMaxAgent : Agent
{
public:
    MiniMaxAgent(token color);

    move getMove(Board board);

};

std::vector<Board> getStates();

#endif