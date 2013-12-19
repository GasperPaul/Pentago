#if defined(_PENTAGO_OPENGL_) && defined(__MainUserInterfaceFile)

#include "Player.h"
#include "UserInterface.h"

#include "GLInterface/GLRenderer.h"

#include "Game.h"
#include "CrossThreadMutex.h"

#include <iostream>
using namespace std;

using std::thread;

UserInterface::UserInterface(bool start) {
	if (start) {
		CrossThreadMutex mutex = CrossThreadMutex(true);
		GLthread = new thread(GLmain, &mutex);
		mutex.wait();
	} else {
		GLthread = NULL;
	}
}

UserInterface::~UserInterface() {
	//TODO: fix it
//	if (GLthread && GLthread->joinable())
//		GLthread->join();
}

Player::Step UserInterface::GetPlayerStep(const Player* player) {
	cout << endl << "\t" << player->GetName() << ", make your move: " << endl
	<< "Your color is " << ((Game::GetInstance()->GetPlayer(Game::Player1) == player) ? "white." : "black.")
	<< endl;
	_mutex.try_lock();
	SetMode(PlayerStep);
	_mutex.wait();
	cout <<  "Your step is: " << step.i << " " << step.j << " " << step.quarter << " " << (int)step.direction << endl;
	return {step.i, step.j, step.quarter, (Board::RotateDirection)step.direction};
}

void UserInterface::PaintBoard() {
	SetMode(OponentStep);
	RefreshStones();
}

void UserInterface::ShowWinner(winstatus status) {
	if (status == Draw)
	cout << "There is a Draw!" << endl;
	if (status == First)
	cout << Game::GetInstance()->GetPlayer(Game::Player1)->GetName() << " wins a game!" << endl;
	if (status == Second)
	cout << Game::GetInstance()->GetPlayer(Game::Player2)->GetName() << " wins a game!" << endl;
	cout << "Game ended." << endl;
}

void UserInterface::Show_StepIsNotAllowed() {
	cout << endl << "This step is not allowed!" << endl;
}

void UserInterface::Show_GameBegins() {
	cout << "Game begins." << endl;
}

UserInterface::MenuItem UserInterface::MenuDialog() {
	cout << "\n\tMake your choice in main menu." << endl;
	_mutex.try_lock();
	SetMode(MainMenu);
	_mutex.wait();
	return menuPressed;
	//char getInput;
	//cout << "\tMake your choice:" << endl << "1. Local game." << endl << "2. Start game host."
	//<< endl << "3. Connect to host." << endl << "0. Exit game." << endl;
	//do {
	//	getInput = getchar();
	//}while ((getInput < '0') || (getInput > '3'));
	//getchar();
	//return (UserInterface::MenuItem) (getInput - '0');
}

std::string UserInterface::InputPlayerName(const std::string& who) {
	SetMode(Waiting);
	std::string name;
	do {
		cout << "Enter " << who << " name: ";
		std::getline(cin, name);
	}while (name == "");
	return name;
}

void UserInterface::Show_WaitForConnection() {
	SetMode(Waiting);
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
	cout << "Waiting until player "<< Game::GetInstance()->GetCurrentPlayer()->GetName() << " makes his move..." << endl;
}

void UserInterface::Show_PlayerDisconnected(const Player* player) {
	cout << "Player " << player->GetName() << " left the game." << endl;
}

void UserInterface::Show_CanNotStartServer() {
	cout << endl << "Server can't start. Check your firewall settings or check if the port is available." << endl;
}

void UserInterface::Show_CanNotConnect(const Network::RemoteAddress* addr) {
	if (addr->port==PentagoServer::DEFAULT_PORT) {
		cout << endl << "Can't connect to: " << addr->addr << ":" << addr->port << "." << endl;
	} else {
		cout << endl << "Can't connect to: " << addr->addr << "." << endl;
	}
}
#endif // _PENTAGO_OPENGL_
