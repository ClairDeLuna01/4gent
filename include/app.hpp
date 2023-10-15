#pragma once

#include "board.hpp"
#include "agent.hpp"

class App
{
public:
    App();

    void mainLoop();

private:
    void newGame();
};