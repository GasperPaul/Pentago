/*
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#include "Game.h"
#include "Utils.h"
#include <iostream>

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
		std::cout << std::endl << "-------------------------" << std::endl;
		for(short i = 0; i<6; i++) {
			std::cout << "|";
			for(short j = 0; j<6; j++)
				std::cout << (board[i][j]<0 ? "" : " ") << board[i][j]
						  << (board[i][j]>9 ? "|" : " |");
			std::cout << std::endl;
		}
		std::cout << "-------------------------" << std::endl << std::endl;

		// checking win condition
		if (short win = Utils::Check(board, 6, 6, 5)) {
			std::cout << players[win>0 ? 0 : 1]->Name() << " wins." << std::endl;
			break;
		}
	}
	std::cout << "Game ended." << std::endl;
}

Game::Game() {
	players = { new Player("Player1"), new Player("Player2") };
}

Game::~Game() {
	delete[] players;
}

