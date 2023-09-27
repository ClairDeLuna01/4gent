#include <iostream>
#include "board.hpp"
#include "agent.hpp"

int main(void) {
    /*
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
    */

    std::vector<Board> states = getStates();
    // for(int i = 0; i < states.size(); i++) {
    //     std::cout << states[i];
    // }

    std::cout << "Number of states: " << states.size() << std::endl;

	return 0;
}
