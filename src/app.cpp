#include "app.hpp"

// for sleeping (zzz)
#include <thread>
#include <chrono>

App::App() : board{}
{
}

void App::newGame(Agent *firstAgent, Agent *secondAgent)
{
    Board board;

    token currentToken = red;
    std::cout << board << '\n';

    bool isBoardFull = false;
    do
    {
        Agent *currentAgent = currentToken == red ? firstAgent : secondAgent;
        std::cout << "It's player " << (currentToken == red ? "red" : "yellow") << "'s turn\nSelect a column to insert your token (0-6) :\n";
        auto start = std::chrono::high_resolution_clock::now();
        while (!board.play(currentToken, currentAgent->getMove(board)))
            ;
        auto end = std::chrono::high_resolution_clock::now();
        if (currentAgent->type != PLAYER)
        {
            // wait for 2 to 5 seconds
            auto sleepTime = std::chrono::milliseconds(1000 + (rand() % 2000));
            sleepTime -= std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::this_thread::sleep_for(sleepTime);
        }
        std::cout << board << '\n';
        currentToken = currentToken == red ? yellow : red;
        isBoardFull = board.isFull();
    } while (board.checkVictory() == empty && !isBoardFull);

    if (isBoardFull)
        std::cout << "Tie : nobody won!\n";
    else
        std::cout << (currentToken == yellow ? "Red" : "Yellow") << " player won!\n";
}

void App::mainLoop()
{
    char doRestart;
    do
    {
        std::cout << "\033[2J\033[1;1H"; // Clears terminal
        board.clear();
        char gameMode;
        std::cout << "Select a game mode :\n\t- 1 : Player vs Player\n\t- 2 : Player vs AI\n";
        std::cin >> gameMode;

        std::unique_ptr<Agent> agent1 = std::make_unique<Player>(Player{red});
        std::unique_ptr<Agent> agent2;

        if (gameMode == '1')
            agent2 = std::make_unique<Player>(Player{yellow});
        else
            agent2 = std::make_unique<MiniMaxAgent>(MiniMaxAgent{yellow});

        newGame(agent1.get(), agent2.get());

        std::cout << "Do you want to play again (y/n)?\n";
        std::cin >> doRestart;
    } while (doRestart == 'y' || doRestart == 'Y');
}