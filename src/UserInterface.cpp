#include "UserInterface.h"

#include "Game.h"

#include <iostream>
#include <string>
#include "Player.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

UserInterface::UserInterface() {
}

Player::Step* UserInterface::GetPlayerStep(const Player* player) {
	short x, y, q, r;
	bool invalid = true;
	cout << player->Name() << ", make your move: " << endl
			<< "row | column | quadrant | left=0, right=1 " << endl
			<< "End game — any value < 0."
			<< endl;
	do {
		cout << "> ";
		cin >> x >> y >> q >> r;
		if (x < 0 || y < 0 || q < 0 || r < 0)
			return new Player::Step { -1, -1, -1, Board::RotateDirection(-1) };
		invalid = x >= 6 && y >= 6 && q >= 4 && r >= 2;
		if (invalid)
			cout << "Invalid input, try again." << endl;
	} while (invalid);

	return new Player::Step { x, y, q, Board::RotateDirection(r) };
}

void UserInterface::PaintBoard(Board& board) {
	cout << endl << "-------------------------" << endl;
	for(short i = 0; i<6; i++) {
		cout << "|";
		for(short j = 0; j<6; j++)
			cout << (board[i][j]<0 ? "" : " ") << board[i][j]
					  << (board[i][j]>9 ? "|" : " |");
		cout << endl;
	}
	cout << "-------------------------" << endl << endl;
}

void UserInterface::ShowWinner(winstatus status, Player players[2]) {
	if (status == Draw)
		cout << "There is a Draw!" << endl;
	if (status == First)
		cout << players[0].Name() <<" wins a game!" << endl;
	if (status == Second)
		cout << players[1].Name() <<" wins a game!" << endl;
	cout << "Game ended." << endl;
}

void UserInterface::Show_StepIsNotAllowed() {

}

void UserInterface::Show_GameBegin() {
	cout << "Game begins." << endl;
}

UserInterface::MenuItem UserInterface::ShowMenu() {
	char getInput;
	cout << "\tMake your choice:" << endl << "1. Local game." << endl << "2. Start game host."
			<< endl << "3. Connect to host." << endl;
	do {
		cin.get(getInput);
	} while ((getInput >= '0') && (getInput <= '3'));
	return (UserInterface::MenuItem) (getInput - '0');
}
