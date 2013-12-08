/*
 * Game.cpp
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#include "Game.h"
#include "Player.h"

Game* Game::instance = 0;

Game* Game::Instance() {
	if (!instance)
		instance = new Game();
	return instance;
}

void Game::Run() {
	//TODO:menu here
	players[0] = new PlayerLocal("Player1");
	players[1] = new PlayerLocal("Player2");
	PlayGame();
}

void Game::PlayGame() {
	short currentPlayer = 0;

	userInterface.Show_GameBegin();
	userInterface.PaintBoard(board);
	while (referee.UpdateWinState(board) == NoOne) {
		Player::Step* step = players[currentPlayer]->MakeStep();
		if (step->i < 0) {
//			std::cout << players[currentPlayer]->Name() << " left the game." << std::endl;
			break;
		}
		while (!board.putStone(step->i, step->j, currentPlayer)) {
			userInterface.Show_StepIsNotAllowed();
			step = players[currentPlayer]->MakeStep();
		}
		board.Rotate(step->quarter, step->direction);
		delete step;
		currentPlayer = currentPlayer ? 0 : 1;

		// displaying the board
		userInterface.PaintBoard(board);
	}

	userInterface.ShowWinner(referee.WinnerIs(), *players);
}

Game::Game() :
		players { NULL, NULL } {
}

Game::~Game() {
	if (players[0])
		delete players[0];
	if (players[1])
		delete players[1];
}

/*void Game::TempTestReferee() {
 Referee referee;
 //pull board:
 for (int i = 0; i < 6; i++) {
 board[i][i] = -2;
 board[5][i] = 2;
 }
 //display board:
 board.DisplayTemp();
 //testing Referee methods:
 std::cout << "WinStatus=" << referee.UpdateWinState(board) << std::endl;
 std::cout << referee.WinnerIs() << " is winner (-1=Second, 0=NoOne, 1=First, 2 = Draw)"
 << std::endl;
 referee.ShowCombinationsTemp();
 return;
 }*/

