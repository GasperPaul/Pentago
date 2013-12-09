/*
 * Game.cpp
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#include "Game.h"
#include "Player.h"
#include "PentagoServer.h"

Game* Game::instance = 0;

Game* Game::Instance() {
	if (!instance)
		instance = new Game();
	return instance;
}

Player* Game::GetPlayer(PlayersNum who) const {
	if (who != PlayerBoth)
		return players[who];
	return NULL;
}

void Game::Run() {
	int iResult;
	std::string myName = userInterface.InputPlayerName("your");
	for (;;)
		switch (userInterface.MenuDialog()) {
		case UserInterface::ExitGame: {
			return;
		}
		case UserInterface::LocalGame: {
			server = new PentagoServer(PentagoServer::DEFAULT_PORT);
			delete players[Player1];
			delete players[Player2];
			players[Player1] = new PlayerLocal(myName);
			players[Player2] = new PlayerLocal(userInterface.InputPlayerName("player 2"));
			network.Connect(new Network::RemoteAddress { "127.0.0.1", PentagoServer::DEFAULT_PORT },
					players, (char)PlayerBoth);
			PlayGame();
			delete server;
			server = 0;
			break;
		}
		case UserInterface::StartHost: {
			server = new PentagoServer(PentagoServer::DEFAULT_PORT);
			delete players[Player1];
			delete players[Player2];
			players[Player1] = new PlayerLocal(myName);
			players[Player2] = new PlayerNetwork("Player");
			iResult = network.Connect(new Network::RemoteAddress { "127.0.0.1",
					PentagoServer::DEFAULT_PORT }, players, Player1);
			if (iResult == 0) {
				if (network.WaitForConnection()) {
					PlayGame();
				}
			}
			delete server;
			server = 0;
			break;
		}
		case UserInterface::ConnectToServer: {
			Network::RemoteAddress* addr = new Network::RemoteAddress;
			if (userInterface.GetHostAddress(addr)) {
				delete players[Player2];
				delete players[Player1];
				players[Player2] = new PlayerLocal(myName);
				players[Player1] = new PlayerNetwork("Player1");
				iResult = network.Connect(addr, players, Player2);
				if (iResult == 0) {
					PlayGame();
				}
			}
			delete addr;
			break;
		}
		}
}

void Game::SetPlayerName(PlayersNum playerNum, const string& name) {
	if (playerNum != PlayerBoth) {
		players[playerNum]->SetName(name);
	} else {
		players[Player1]->SetName(name);
		players[Player2]->SetName(name);
	}
}

void Game::PlayGame() {
	short currentPlayer = 0;

	userInterface.Show_GameBegins();
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
		players { NULL, NULL }, server(NULL) {
}

Game::~Game() {
	delete players[Player1];
	delete players[Player2];
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

