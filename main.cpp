#include <iostream>
#include "board.hpp"
#include "agent.hpp"

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
    Board board;

    board.play(red, 0);
    board.play(yellow, 1);
    board.play(red, 2);
    board.play(yellow, 3);
    board.play(red, 4);
    board.play(yellow, 5);
    board.play(red, 6);

    board.play(red, 0);
    board.play(yellow, 1);
    board.play(red, 2);
    board.play(yellow, 3);
    board.play(red, 4);
    board.play(yellow, 5);
    board.play(red, 6);

    board.play(yellow, 0);
    board.play(red, 1);
    board.play(yellow, 2);
    board.play(red, 3);
    board.play(yellow, 4);
    board.play(red, 5);
    board.play(yellow, 6);

    board.play(yellow, 0);
    board.play(red, 1);
    board.play(yellow, 2);
    board.play(red, 3);
    board.play(yellow, 4);
    board.play(red, 5);
    board.play(yellow, 6);

    std::cout << board << std::endl;
    // std::cout << "score: " << board.evaluate(yellow) << std::endl;

    token tok = board.checkVictory();
    std::cout << "win: " << (tok == red ? "red" : (tok == yellow ? "yellow" : "none")) << std::endl;

    // std::vector<BoardHashPair> states = getStates();

    // std::cout << "Number of states: " << states.size() << std::endl;

    return 0;
}
