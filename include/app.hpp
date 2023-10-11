#pragma once

#include "board.hpp"
#include "agent.hpp"
#include <memory>
#include <iostream>

/// @brief The main application class which contains the main loop
class App {
public:

    /// @brief Instantiates the application
    App();

    /// @brief The main loop of the game
    void mainLoop();

private:

    /// @brief the board which holds the players' tokens
    Board board;

    /// @brief Starts a new game
    void newGame(Agent* firstAgent, Agent* secondAgent);

};