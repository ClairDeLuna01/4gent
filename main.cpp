#include <iostream>
#include "app.hpp"

void playHorizontal(Board &board)
{
    board.play(yellow, 0);
    board.play(red, 4);
    board.play(yellow, 1);
    board.play(red, 5);
    board.play(yellow, 2);
    board.play(red, 6);
    board.play(yellow, 3);
}

void playVertical(Board &board)
{
    board.play(yellow, 0);
    board.play(red, 1);
    board.play(yellow, 0);
    board.play(red, 1);
    board.play(yellow, 0);
    board.play(red, 1);
    board.play(yellow, 0);
}

void playDiagonalBL2TR(Board &board)
{
    board.play(yellow, 0);
    board.play(red, 1);
    board.play(yellow, 1);
    board.play(red, 2);
    board.play(yellow, 3);
    board.play(red, 3);
    board.play(yellow, 2);
    board.play(red, 3);
    board.play(yellow, 2);
    board.play(red, 4);
    board.play(yellow, 3);
}

void playDiagonalBR2TL(Board &board)
{
    board.play(yellow, 4);
    board.play(red, 3);
    board.play(yellow, 3);
    board.play(red, 2);
    board.play(yellow, 1);
    board.play(red, 1);
    board.play(yellow, 2);
    board.play(red, 1);
    board.play(yellow, 2);
    board.play(red, 0);
    board.play(yellow, 1);
}

int main(void)
{
    App{}.mainLoop();

    // Board board;
    // board.play(red, 3);
    // board.play(yellow, 3);
    // board.play(red, 1);
    // board.play(yellow, 0);
    // board.play(red, 2);
    // // board.play(yellow, 0);

    // std::cout << board << std::endl;
    // // std::cout << "eval: " << board.evaluate(yellow) << std::endl;

    // MiniMaxAgent agent = MiniMaxAgent{yellow};
    // move mv = agent.getMove(board);

    // std::cout << "move: " << mv << std::endl;

    return 0;
}
