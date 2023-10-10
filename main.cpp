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
	// Board board;
	// board.play(red, 1);
	// board.play(red, 1);
	// board.play(red, 1);
	// board.play(red, 1);
	// board.play(red, 1);
	// board.play(red, 1);
	// board.play(red, 1);
	// board.play(red, 6);
	// board.play(yellow, 2);
	// playDiagonalBL2TR(board);

	// std::cout << "\nlegal moves :";
	// std::vector<int> test = board.getLegalMoves();
	// for(size_t i  = 0; i < test.size(); i++)
	// 	std::cout << test[i] << " ";

	std::vector<BoardHashPair> states = getStates();
	// for(int i = 0; i < states.size(); i++) {
	//     std::cout << states[i];
	// }
	std::cout << "Number of states: " << states.size() << std::endl;

	// playHorizontal(board);
	// std::cout << board;
	// std::cout << "winner : " << (int)board.checkVictory() << "\n";
	// board.clear();

	// playVertical(board);
	// std::cout << board;
	// std::cout << "winner : " << (int)board.checkVictory() << "\n";
	// board.clear();

	// playDiagonalBL2TR(board);
	// std::cout << board;
	// std::cout << "winner : " << (int)board.checkVictory() << "\n";
	// board.clear();

	// playDiagonalBR2TL(board);
	// std::cout << board;
	// std::cout << "winner : " << (int)board.checkVictory() << "\n";
	// board.clear();

	return 0;
}
