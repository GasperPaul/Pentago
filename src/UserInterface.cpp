#include "UserInterface.h"

#include "Game.h"
#include "PentagoServer.h"
#include "Player.h"

#include <iostream>
#include <string>
#include <stdio.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;

UserInterface::UserInterface() {
}

Player::Step UserInterface::GetPlayerStep(const Player* player) {
	short x, y, q, r;
	bool invalid = true;
	cout << player->GetName() << ", make your move: " << endl
			<< "row | column | quadrant | left=0, right=1 " << endl << "End game - any value < 0."
			<< endl;
	do {
		cout << "> ";
		cin.clear();
		cin >> x >> y >> q >> r;
		if (x < 0 || y < 0 || q < 0 || r < 0) {
			Player::Step result;
			result.i = -1;
			result.j = -1;
			result.quarter = -1;
			result.direction = Board::RotateDirection(-1);
			return result;
		}
		invalid = x >= 6 && y >= 6 && q >= 4 && r >= 2;
		if (invalid)
			cout << "Invalid input, try again." << endl;
	} while (invalid);
	Player::Step result;
	result.i = x;
	result.j = y;
	result.quarter = q;
	result.direction = Board::RotateDirection(r);
	return result;
}

void UserInterface::PaintBoard(Board& board) {
	cout << endl << "-------------------------" << endl;
	for (short i = 0; i < 6; i++) {
		cout << "|";
		for (short j = 0; j < 6; j++)
			cout << (board[i][j] < 0 ? "" : " ") << board[i][j] << (board[i][j] > 9 ? "|" : " |");
		cout << endl;
	}
	cout << "-------------------------" << endl << endl;
}

void UserInterface::ShowWinner(winstatus status, Player* players[2]) {
	if (status == Draw)
		cout << "There is a Draw!" << endl;
	if (status == First)
		cout << players[0]->GetName() << " wins a game!" << endl;
	if (status == Second)
		cout << players[1]->GetName() << " wins a game!" << endl;
	cout << "Game ended." << endl;
}

void UserInterface::Show_StepIsNotAllowed() {
	cout << endl << "This step is not allowed!" << endl;
}

void UserInterface::Show_GameBegins() {
	cout << "Game begins." << endl;
}

UserInterface::MenuItem UserInterface::MenuDialog() {
	char getInput;
	cout << "\tMake your choice:" << endl << "1. Local game." << endl << "2. Start game host."
			<< endl << "3. Connect to host." << endl << "0. Exit game." << endl;
	do {
		getInput = getchar();
	} while ((getInput < '0') || (getInput > '3'));
	getchar();
	return (UserInterface::MenuItem) (getInput - '0');
}

string UserInterface::InputPlayerName(string who) {
	string name;
	do {
		cin.clear();
		cout << "Enter " << who << " name: ";
		getline(cin, name);
	} while (name == "");
	return name;
}

void UserInterface::Show_WaitForConnection() {
	cout << "Waiting for oponent..." << endl;
}

void UserInterface::_PlayerConnected(const Player* player) {
	cout << "Player \"" << player->GetName() << "\" connected." << endl;
}

bool UserInterface::GetHostAddress(Network::RemoteAddress* addr) {
	cout << "Enter host address: ";
	addr->addr = "";
	addr->port = "";
	while (addr->addr == "") {
		cin.clear();
		getline(cin, addr->addr);
	}
	cout << "Enter host port (-1 for default): ";
	while (addr->port == "") {
		cin.clear();
		getline(cin, addr->port);
	}
	if (addr->port == "-1") {
		addr->port = PentagoServer::DEFAULT_PORT;
	}
	return true;
}

void UserInterface::Show_WaitingForOponentsStep() {
	cout << "Waiting until player "<< Game::Instance()->GetCurrentPlayer()->GetName() << " makes his move..." << endl;
}

void UserInterface::Show_PlayerDisconnected(const Player* player) {
	cout << "Player " << player->GetName() << " left the game." << endl;
}

#ifdef _DEBUG
	void UserInterface::ShowDebugInfo(const char* info){
		cout << endl << "DEBUG: " << info << endl;
	}
#endif
