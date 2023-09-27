#include <iostream>
#include <board.hpp>

int main(void) {
	Board board;
	board.play(red, 1);
	board.play(red, 1);
	board.play(red, 1);
	board.play(red, 1);
	board.play(red, 1);
	board.play(red, 1);
	board.play(red, 1);
	board.play(red, 6);
	board.play(yellow, 2);
	std::cout << board;

	std::cout << "\nlegal moves :";
	std::vector<int> test = board.getLegalMoves();
	for(size_t i  = 0; i < test.size(); i++)
		std::cout << test[i] << " ";

	return 0;
}

// End of file
