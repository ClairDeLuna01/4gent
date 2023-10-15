#include "app.hpp"

App::App() {
}

void App::newGame() {
    Board board;
    std::cout << "Select a game mode :\n\t- 1 : Player vs Player\n\t- 2 : Player vs AI\n";
    int gameMode;
    std::cin >> gameMode;

    Agent* agent1 = new Player{red};
    Agent* agent2 = gameMode == 1
        ? static_cast<Agent*>(new Player{yellow})
        : static_cast<Agent*>(new MiniMaxAgent{yellow});
        
    token currentToken = red;
    std::cout << board << '\n';

    bool isBoardFull = false;
    do {
        Agent* currentAgent = currentToken == red ? agent1 : agent2;
        std::cout << "It's player " << (currentToken == red ? "red" : "yellow") << "'s turn\nSelect a column to insert your token (0-6) :\n";
        while (!board.play(currentToken, currentAgent->getMove(board)));
        std::cout << board << '\n';
        currentToken = currentToken == red ? yellow : red;
        isBoardFull = board.isFull();
    } while (board.checkVictory() == empty && !isBoardFull);

    if (isBoardFull)
        std::cout << "Tie : nobody won!\n";
    else
        std::cout << (currentToken == yellow ? "Red" : "Yellow") << " player won!\n";

    delete agent1;
    delete agent2;
}

void App::mainLoop() {
    char restart;
    do {
        newGame();
        std::cout << "Do you want to play again (y/n)?\n";
        std::cin >> restart;
    } while (restart == 'y' || restart == 'Y');
}