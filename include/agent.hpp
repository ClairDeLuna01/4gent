#ifndef AGENT_HPP
#define AGENT_HPP

typedef int move;

#include <board.hpp>

class Agent
{
    protected :
        token color;

    public : 
        Agent(token color);

        virtual move getMove(Board board);
};

class Player : Agent
{

};

class InteligentAgent : Agent
{

};

#endif