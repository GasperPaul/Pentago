/*
 * Game.cpp
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#include "Game.h"
#include <iostream>
#include "Utils.h"

Game* Game::instance = 0;

Game* Game::Instance() {
	if (!instance) instance = new Game();
	return instance;
}

void Game::Run() {
	short currentPlayer = 0;

	std::cout << "Game begins." << std::endl;
	while (true) {
		short* step = players[currentPlayer]->Step();
		if (step[0] < 0) {
			std::cout << players[currentPlayer]->Name() << " left the game." << std::endl;
			break;
		}
		while (!board.putStone(step[0], step[1], currentPlayer)) {
			std::cout << "This step is not allowed. Try again." << std::endl;
			step = players[currentPlayer]->Step();
		}
		board.Rotate(step[2], (Board::RotateDirection)step[3]);
		delete step;
		currentPlayer = currentPlayer ? 0 : 1;

		// displaying the board
		board.Display();

		// checking win condition
		if (short win = Utils::Check(board, 6, 6, 5)) {
			std::cout << players[win>0 ? 0 : 1]->Name() << " wins." << std::endl;
			break;
		}
	}
	std::cout << "Game ended." << std::endl;
}

Game::Game() {
	players[0] = new Player("Player1");
	players[1] = new Player("Player2");
}

Game::~Game() {
//	delete[] players;
}

void Game::TempTestReferee(){
	Referee referee;
	//pull board:
	for(int i=0;i<6;i++){
		board[i][i]=-2;
		board[5][i]=2;
	}
	//display board:
	board.Display();
	//testing Referee methods:
	std::cout << "WinStatus=" << referee.UpdateWinState(board) << std::endl;
	std::cout << referee.WinnerIs() << " is winner (-1=Second, 0=NoOne, 1=First, 2 = Draw)" << std::endl;
	referee.ShowCombinations();
	return;
}






