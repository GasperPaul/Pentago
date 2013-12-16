#if defined(_PENTAGO_CONSOLE_) && defined(__MainUserInterfaceFile)

#include <iostream>
#include <string>
#include <stdio.h>
#include <climits>

using std::cout;
using std::cin;
using std::endl;
using std::string;

UserInterface::UserInterface(const Board * board) {
	_board = board;
}
UserInterface::~UserInterface() {
}

Player::Step UserInterface::GetPlayerStep(const Player* player) {
	short x, y, q, r;
	bool invalid = true;
	cout << player->GetName() << ", make your move: " << endl
			<< "row | column | quadrant | left=0, right=1 " << endl << "End game - any value < 0."
			<< endl;
	do {
		cout << "> ";
		cin >> x >> y >> q >> r;
		if (x < 0 || y < 0 || q < 0 || r < 0) {
			return {-1, -1, -1, Board::RotateDirection(-1)};
		}
		invalid = x >= 6 && y >= 6 && q >= 4 && r >= 2;
		if (invalid)
			cout << "Invalid input, try again." << endl;
	} while (invalid);
	return {x, y, q, Board::RotateDirection(r)};
}

void UserInterface::PaintBoard() {
	Board &board = *const_cast<Board*>(_board);
	cout << endl << "-------------------------" << endl;
	for (short i = 0; i < 6; i++) {
		cout << "|";
		for (short j = 0; j < 6; j++)
			cout << (board[i][j] < 0 ? "" : " ") << board[i][j] << (board[i][j] > 9 ? "|" : " |");
		cout << endl;
	}
	cout << "-------------------------" << endl << endl;
}

void UserInterface::ShowWinner(winstatus status) {
	if (status == Draw)
		cout << "There is a Draw!" << endl;
	if (status == First)
		cout << Game::Instance()->GetPlayer(Game::Player1)->GetName() << " wins a game!" << endl;
	if (status == Second)
		cout << Game::Instance()->GetPlayer(Game::Player2)->GetName() << " wins a game!" << endl;
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

std::string UserInterface::InputPlayerName(const std::string& who) {
	std::string name;
	do {
		cout << "Enter " << who << " name: ";
		std::getline(cin, name);
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
		getline(cin, addr->addr);
	}
	cout << "Enter host port (-1 for default): ";
	while (addr->port == "") {
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


void UserInterface::Show_CanNotConnect(const Network::RemoteAddress* addr) {
	if (addr->port==PentagoServer::DEFAULT_PORT) {
		cout << endl << "Can't connect to: " << addr->addr << ":" << addr->port << "." << endl;
	} else {
		cout << endl << "Can't connect to: " << addr->addr << "." << endl;
	}
}

void UserInterface::Show_CanNotStartServer() {
	cout << endl << "Server can't start. Check your firewall settings or check if the port is available." << endl;
}

#ifdef DEBUG
void UserInterface::ShowDebugInfo(const char* info){
	cout << endl << "DEBUG: " << info << endl;
}
#endif

#endif // _PENTAGO_CONSOLE_
