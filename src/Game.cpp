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

const Player* Game::GetPlayer(PlayersNum who) const {
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
			Network::RemoteAddress * param = new Network::RemoteAddress;
			param->addr = "127.0.0.1";
			param->port = PentagoServer::DEFAULT_PORT;
			network.Connect(param, players, (char) PlayerBoth);
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
			players[Player2] = new PlayerNetwork("");
			Network::RemoteAddress * param = new Network::RemoteAddress;
			param->addr = "127.0.0.1";
			param->port = PentagoServer::DEFAULT_PORT;
			iResult = network.Connect(param, players, Player1);
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
				//!important to be ""
				players[Player1] = new PlayerNetwork("");
				players[Player2] = new PlayerLocal(myName);
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
	board = Board();
	currentPlayer = Player1;
	userInterface.Show_GameBegins();
	userInterface.PaintBoard(board);
	while (referee.UpdateWinState(board) == NoOne) {
		bool flag;
		Player::Step* step;
		do {
			step = players[currentPlayer]->MakeStep();
			if (step->i < 0) {
				if (step->i < 0) {
					userInterface.Show_PlayerDisconnected(players[currentPlayer]);
					break;
				}
			}
			flag = board.putStone(step->i, step->j, currentPlayer);
			if (!flag) {
				userInterface.Show_StepIsNotAllowed();
				delete step;
			}
		} while (!flag);
		if (step->i < 0) {
			break;
		}
		board.Rotate(step->quarter, step->direction);
		delete step;
		currentPlayer = currentPlayer ? Player1 : Player2;

		// displaying the board
		userInterface.PaintBoard(board);
	}

	userInterface.ShowWinner(referee.WinnerIs(), *players);
}

const Player * Game::GetCurrentPlayer() const {
	return players[currentPlayer];
}

Game::Game(){
	players[0]=NULL;
	players[1]=NULL;
	server = NULL;
	currentPlayer = Player1;
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

